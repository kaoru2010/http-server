#include <signal.h>

#include <boost/asio.hpp>
#include <boost/asio/coroutine.hpp>
#include <network/http/server.hpp>
#include <network/http/http_request.hpp>

namespace network { namespace http {

#include <boost/asio/yield.hpp>
template <typename AsyncReadStream, typename Handler>
class connection_context : boost::asio::coroutine
{
    AsyncReadStream& istream_;
    Handler handler_;
    boost::shared_ptr<boost::asio::streambuf> buffer_;
    boost::shared_ptr<http_request_t> http_request_;

public:
    connection_context(AsyncReadStream& istream, Handler&& handler)
    :   istream_(istream)
    ,   handler_(handler)
    ,   buffer_()
    ,   http_request_()
    {}

    void operator()(boost::system::error_code ec = {}, std::size_t size = 0) {
        run(ec, size);

        if (is_complete())
            handler_();
    }

private:
    void run(boost::system::error_code ec, std::size_t size) {  // bytes_transferred
        reenter(this) {
            if (ec)
                return;

            buffer_.reset(new boost::asio::streambuf());
            http_request_.reset(new http_request_t());

            // Read header
            yield boost::asio::async_read_until(istream_, *buffer_, "\r\n\r\n", *this);
            if (boost::system::error_code ret = parse_request_header(boost::asio::buffer_cast<const char *>(buffer_->data()), size, *http_request_)) {
                return;
            }
        }
    }
};
#include <boost/asio/unyield.hpp>

class connection_context_creator
{
public:
    template <typename AsyncReadStream, typename Handler>
    auto create(AsyncReadStream& istream, Handler&& handler) -> connection_context<AsyncReadStream, Handler> {
        return {istream, handler};
    }
};

}} // namespace network { namespace http {

int main()
{
    using namespace network::http;

    connection_context_creator creator;

    boost::asio::io_service io_service;
    network::http::server<connection_context_creator> server{io_service, creator};

    boost::asio::signal_set signals(io_service);
    signals.add(SIGINT);
    signals.add(SIGTERM);

#if defined(SIGQUIT)
    signals.add(SIGQUIT);
#endif // defined(SIGQUIT)

    signals.async_wait([&](const boost::system::error_code& error, int signal_number) {
        io_service.stop();
    });

    server.listen("127.0.0.1", "10080");

    io_service.run();

    return 0;
}
