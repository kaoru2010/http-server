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

            // Read status line
            yield boost::asio::async_read_until(istream_, *buffer_, "\r\n", *this);
            if (boost::system::error_code ret = parse_status_line(boost::asio::buffer_cast<const char *>(buffer_->data()), size, *http_request_)) {
                return;
            }

            // Read header
            yield boost::asio::async_read_until(istream_, *buffer_, "\r\n\r\n", *this);
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

    server.listen();

    return 0;
}
