#include <iostream>
#include <functional>
#include <boost/asio.hpp>
#include <boost/asio/coroutine.hpp>
#include <network/http/connection.hpp>
#include <network/http/http_protocol.hpp>
#include <network/http/http_request.hpp>

namespace network { namespace http {

namespace {

class http_protocol_context_t
    :   public connection_t
    ,   private boost::asio::coroutine
{
    boost::asio::ip::tcp::socket stream_;
    boost::asio::streambuf request_buffer_;
    boost::asio::streambuf response_buffer_;
    http_request_t http_request_;

public:
    http_protocol_context_t(boost::asio::io_service& io_service)
    :   stream_(io_service)
    {}

    virtual ~http_protocol_context_t() {
    }

    virtual boost::asio::ip::tcp::socket& socket() {
        return stream_;
    }

    virtual void handle_connection() {
        (*this)();
    }

    void operator()(boost::system::error_code ec = {}, std::size_t size = 0);
};

#include <boost/asio/yield.hpp>
void http_protocol_context_t::operator()(boost::system::error_code ec, std::size_t size) {  // bytes_transferred
    using std::ref;

    if (ec)
        return;

    reenter(this) {
        // Read header
        yield boost::asio::async_read_until(stream_, request_buffer_, "\r\n\r\n", ref(*this));

        parse_request_header(boost::asio::buffer_cast<const char *>(request_buffer_.data()), size, http_request_, ec);

        {
            std::ostream response_stream(&response_buffer_);
            response_stream << "HTTP/1.0 " << ec.value() << " " << ec.message() << "\r\n";
            response_stream << "Content-Type: text/html\r\n";
            response_stream << "\r\n";
            response_stream << "<html>\n";
            response_stream << "<head><title>" << ec.value() << " " << ec.message() << "</title></head>\n";
            response_stream << "<body><h1>" << ec.value() << " " << ec.message() << "</h1></body>\n";
            response_stream << "</html>\n";
        }

        yield boost::asio::async_write(stream_, response_buffer_, ref(*this));

        boost::system::error_code ec;
        stream_.close(ec);
    }
}
#include <boost/asio/unyield.hpp>

} // annonymous namespace


auto http_protocol_context_creator::create(boost::asio::io_service& io_service) -> std::unique_ptr<network::http::connection_t> {
    return std::unique_ptr<network::http::connection_t>(new http_protocol_context_t(io_service));
}

auto http_protocol::get_context_creator() -> context_creator_t& {
    static http_protocol_context_creator creator;
    return creator;
}

}} // namespace network { namespace http {