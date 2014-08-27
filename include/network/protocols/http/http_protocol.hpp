#pragma once
#include <network/servers/connection.hpp>
#include <network/servers/server.hpp>

namespace network { namespace protocols { namespace http {

// これはここになくてもOK
struct http_protocol_context_creator : public servers::context_creator_t
{
    virtual std::unique_ptr<servers::connection_t> create(boost::asio::io_service&);
};

struct http_protocol
{
    static servers::context_creator_t& get_context_creator();
};

}}} // namespace network { namespace protocols { namespace http {
