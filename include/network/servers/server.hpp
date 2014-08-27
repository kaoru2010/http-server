#pragma once

#include <forward_list>
#include <memory>
#include <boost/asio.hpp>

namespace network { namespace servers {

class connection_t;

using connection_list_t = std::forward_list<std::unique_ptr<connection_t>>;

class context_creator_t
{
public:
    virtual ~context_creator_t() {}
    virtual std::unique_ptr<connection_t> create(boost::asio::io_service& io_service) = 0;
};

class server_base
{
    boost::asio::io_service& io_service_;
    context_creator_t& context_creator_;
    boost::asio::ip::tcp::acceptor acceptor_;
    connection_list_t connection_list_;

public:
    server_base(boost::asio::io_service& io_service, context_creator_t& context_creator);
    boost::system::error_code listen(const std::string& address, const std::string& port);
    boost::system::error_code stop(boost::system::error_code &ec);

protected:
    ~server_base();
};

template <typename ProtocolHandler>
class server : public server_base
{
public:
    explicit server(boost::asio::io_service& io_service) // , context_creator_t& context_creator
    :   server_base(io_service, ProtocolHandler::get_context_creator())
    {}
};

}} // namespace network { namespace servers {
