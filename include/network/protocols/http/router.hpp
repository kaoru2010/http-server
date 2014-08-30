#pragma once

#include <string>
#include <boost/system/error_code.hpp>

namespace network { namespace protocols { namespace http {

class http_request_t;

class router
{
public:
    virtual boost::system::error_code dispatch_controller(std::string const& url, http_request_t& request) = 0;
    virtual ~router() = 0;
};

inline
router::~router()
{
}

}}} // namespace network { namespace protocols { namespace http {
