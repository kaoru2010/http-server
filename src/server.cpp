#include <forward_list>
#include <memory>
#include <functional>
#include <boost/asio.hpp>
#include <network/servers/server.hpp>
#include <network/servers/connection.hpp>
#include <boost/asio/coroutine.hpp>

using namespace std;

namespace network { namespace servers {

#include <boost/asio/yield.hpp>
class server_base::server_context_t : boost::asio::coroutine
{
    using self_t = server_context_t;
    server_base& server_;

public:
    server_context_t(server_base& server)
    :   server_(server)
    {}

    void operator()(boost::system::error_code const& ec = {})
    {
        if (ec)
            return;

        reenter(this) {
            do {
                yield {
                    unique_ptr<connection_t> ptr(server_.create_connection_context(server_.io_service_));
                    server_.acceptor_.async_accept(ptr->socket(), *this);
                    server_.connection_list_.push_front(move(ptr));
                }
                fork self_t(*this)();
            } while (is_parent());

            server_.connection_list_.front()->handle_connection();
        }
    }
};
#include <boost/asio/unyield.hpp>

server_base::server_base(boost::asio::io_service& io_service)
:   io_service_(io_service)
,   acceptor_(io_service)
,   connection_list_()
{}

server_base::~server_base()
{
}

boost::system::error_code server_base::listen(const std::string& address, const std::string& port)
{
    using tcp = boost::asio::ip::tcp;

    boost::system::error_code ec;

    tcp::resolver resolver(io_service_);
    tcp::resolver::query query(address, port);
    tcp::endpoint endpoint = *resolver.resolve(query);

    acceptor_.open(endpoint.protocol(), ec);
    if (ec)
        return ec;

    acceptor_.set_option(boost::asio::socket_base::reuse_address(true));

    acceptor_.bind(endpoint, ec);
    if (ec)
        return ec;

    int listen_backlog = 3;// The maximum length of the queue of pending connections.
    acceptor_.listen(listen_backlog, ec);
    if (ec)
        return ec;

    server_context_t context(*this);
    context();

    return ec;
}

boost::system::error_code server_base::stop(boost::system::error_code &ec)
{
    return acceptor_.close(ec);
}

}} // namespace network { namespace servers {
