#pragma once

#include <forward_list>
#include <memory>
#include <boost/asio.hpp>

namespace network { namespace servers {

class connection_t;

using connection_list_t = std::forward_list<std::unique_ptr<connection_t>>;

class server_base
{
    boost::asio::io_service& io_service_;
    boost::asio::ip::tcp::acceptor acceptor_;
    connection_list_t connection_list_;

public:
    server_base(boost::asio::io_service& io_service);
    boost::system::error_code listen(const std::string& address, const std::string& port);
    boost::system::error_code stop(boost::system::error_code &ec);
    virtual std::unique_ptr<connection_t> create_connection_context(boost::asio::io_service& io_service) = 0;

protected:
    ~server_base();

private:
    class server_context_t;
};

template <typename ProtocolHandler>
class server : public server_base, public ProtocolHandler
{
public:
    explicit server(boost::asio::io_service& io_service) // , context_creator_t& context_creator
    :   server_base(io_service)
    {}

    virtual std::unique_ptr<connection_t> create_connection_context(boost::asio::io_service& io_service) override
    {
        return ProtocolHandler::create_connection_context(io_service);
    }
};

}} // namespace network { namespace servers {
