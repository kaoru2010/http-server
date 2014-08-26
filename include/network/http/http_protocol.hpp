#pragma once
#include <network/http/connection.hpp>
#include <network/http/server.hpp>

namespace network { namespace http {

// これはここになくてもOK
struct http_protocol_context_creator : public context_creator_t
{
    virtual std::unique_ptr<connection_t> create(boost::asio::io_service&);
};

struct http_protocol
{
    static context_creator_t& get_context_creator();
};

}} // namespace network { namespace http {
