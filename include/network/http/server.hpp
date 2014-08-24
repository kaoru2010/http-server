#pragma once

#include <boost/asio.hpp>
#include <network/http/context.hpp>

namespace network { namespace http {

template <typename ConnectionContextCreator>
class server
{
    boost::asio::io_service& io_service_;
    boost::asio::ip::tcp::acceptor acceptor_;
    ConnectionContextCreator& creator_;

public:
    server(boost::asio::io_service& io_service, ConnectionContextCreator& creator)
    :   io_service_(io_service)
    ,   acceptor_(io_service)
    ,   creator_(creator)
    {}

    boost::system::error_code listen(const std::string& address, const std::string& port) {
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

        socket_context<ConnectionContextCreator> context(acceptor_, creator_);
        context();

        return ec;
    }

    boost::system::error_code stop(boost::system::error_code &ec) {
        return acceptor_.close(ec);
    }
};

}} // namespace network { namespace http {
