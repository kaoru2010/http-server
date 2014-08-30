#pragma once

#include <string>
#include <boost/system/error_code.hpp>
#include <network/protocols/http/router.hpp>

class demo_router : public network::protocols::http::router
{
public:
    demo_router() {}
    ~demo_router() override {}
    boost::system::error_code dispatch_controller(const std::string&, network::protocols::http::http_request_t&) override;
};
