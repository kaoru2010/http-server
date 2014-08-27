#include <forward_list>
#include <memory>
#include <functional>
#include <boost/asio.hpp>
#include <network/servers/server.hpp>
#include <network/servers/connection.hpp>
#include <boost/asio/coroutine.hpp>

using namespace std;

namespace network { namespace servers {

namespace {

#include <boost/asio/yield.hpp>
class server_context_t : boost::asio::coroutine
{
    using self_t = server_context_t;

    boost::asio::io_service &io_service_;
    boost::asio::ip::tcp::acceptor& acceptor_;
    context_creator_t& context_creator_;
    connection_list_t& connection_list_;

public:
    server_context_t(boost::asio::io_service &io_service, boost::asio::ip::tcp::acceptor& acceptor, context_creator_t& context_creator, connection_list_t& connection_list)
    :   io_service_(io_service)
    ,   acceptor_(acceptor)
    ,   context_creator_(context_creator)
    ,   connection_list_(connection_list)
    {}

    void operator()(boost::system::error_code const& ec = {})
    {
        if (ec)
            return;

        reenter(this) {
            do {
                yield {
                    unique_ptr<connection_t> ptr(context_creator_.create(io_service_));
                    acceptor_.async_accept(ptr->socket(), *this);
                    connection_list_.push_front(move(ptr));
                }
                fork self_t(*this)();
            } while (is_parent());

            connection_list_.front()->handle_connection();
        }
    }
};
#include <boost/asio/unyield.hpp>

} // anonymous namespace


server_base::server_base(boost::asio::io_service& io_service, context_creator_t& context_creator)
:   io_service_(io_service)
,   context_creator_(context_creator)
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

    server_context_t context(io_service_, acceptor_, context_creator_, connection_list_);
    context();

    return ec;
}

boost::system::error_code server_base::stop(boost::system::error_code &ec)
{
    return acceptor_.close(ec);
}

}} // namespace network { namespace servers {
