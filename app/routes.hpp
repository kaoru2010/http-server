#include <string>
#include <boost/system/error_code.hpp>

namespace network { namespace protocols { namespace http {

class http_request_t;

boost::system::error_code dispatch_controller(std::string const& url, http_request_t& request);

}}} // namespace network { namespace protocols { namespace http {
