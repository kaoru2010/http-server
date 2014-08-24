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

    void listen() {
        socket_context<ConnectionContextCreator> context(acceptor_, creator_);
        context();
    }

    boost::system::error_code stop(boost::system::error_code &ec) {
        return acceptor_.close(ec);
    }
};

}} // namespace network { namespace http {
