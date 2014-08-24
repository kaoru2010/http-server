#pragma once

#include <boost/asio.hpp>

namespace network { namespace http {

class server
{
    boost::asio::io_service& io_service_;

public:
    server(boost::asio::io_service& io_service)
    :   io_service_(io_service)
    {}
};

}} // namespace network { namespace http {
