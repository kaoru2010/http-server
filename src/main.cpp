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
    boost::shared_ptr<boost::asio::streambuf> request_buffer_;
    boost::shared_ptr<boost::asio::streambuf> response_buffer_;
    boost::shared_ptr<http_request_t> http_request_;

public:
    connection_context(AsyncReadStream& istream, Handler&& handler)
    :   istream_(istream)
    ,   handler_(handler)
    ,   request_buffer_()
    ,   response_buffer_()
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

            request_buffer_.reset(new boost::asio::streambuf());
            http_request_.reset(new http_request_t());

            // Read header
            yield boost::asio::async_read_until(istream_, *request_buffer_, "\r\n\r\n", *this);
            ec = parse_request_header(boost::asio::buffer_cast<const char *>(request_buffer_->data()), size, *http_request_);
            if (ec) {
                response_buffer_.reset(new boost::asio::streambuf());
                {
                    std::ostream response_stream(response_buffer_.get());
                    response_stream << "HTTP/1.0 " << ec.value() << " " << ec.message() << "\r\n";
                    response_stream << "Content-Type: text/html\r\n";
                    response_stream << "\r\n";
                    response_stream << "<html>\n";
                    response_stream << "<head><title>" << ec.value() << " " << ec.message() << "</title></head>\n";
                    response_stream << "<body><h1>" << ec.value() << " " << ec.message() << "</h1></body>\n";
                    response_stream << "</html>\n";
                }

                yield boost::asio::async_write(istream_, *response_buffer_, *this);
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
