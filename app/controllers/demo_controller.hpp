#pragma once

#include <network/protocols/http/http_status.hpp>

namespace controllers {

class demo_controller
{
public:
    boost::system::error_code get_index() const
    {
        return HTTP_OK;
    }
};

} // namespace app { namespace controllers {
