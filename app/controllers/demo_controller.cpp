#include <network/protocols/http/http_status.hpp>
#include <controllers/demo_controller.hpp>

using namespace network::protocols::http;
using boost::system::make_error_code;

namespace controllers {

network::protocols::http::http_status_error demo_controller::get_index() const
{
    return HTTP_OK;
}

network::protocols::http::http_status_error demo_controller::get_list() const
{
    return HTTP_OK;
}

} // namespace app { namespace controllers {
