#pragma once

#include <network/protocols/http/http_status.hpp>

namespace controllers {

class demo_controller
{
public:
    network::protocols::http::http_status_error get_index() const;
    network::protocols::http::http_status_error get_list() const;
};

} // namespace app { namespace controllers {
