#pragma once

#include <memory>
#include <boost/asio.hpp>

namespace network { namespace http {

class connection_t
{
public:
    virtual ~connection_t() = 0;
    virtual boost::asio::ip::tcp::socket& socket() = 0;
    virtual void handle_connection() = 0;
};

inline
connection_t::~connection_t()
{
}

}} // namespace network { namespace http {
