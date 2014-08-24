#pragma once

#include <boost/system/error_code.hpp>

namespace network { namespace http {

enum method_type_t {
    GET, POST, HEAD,
};

enum protocol_t {
    HTTP_1_0, HTTP_1_1,
};

class http_request_t
{
public:
    void set_method(method_type_t type) {
    }

    void set_path(const char *path) {
    }

    void set_protocol(protocol_t protocol) {
    }

    void set_error() {}
};

boost::system::error_code parse_request_header(const char *buf, std::size_t length, http_request_t& request);

}} // namespace network { namespace http {
