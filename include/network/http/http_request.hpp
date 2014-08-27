#pragma once

#include <string>

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
    method_type_t type_;
    std::string path_;
    protocol_t protocol_;

public:
    method_type_t get_method() const { return type_; }
    std::string get_path() const { return path_; }
    const char *get_path_c() const { return path_.c_str(); }
    protocol_t get_protocol() const { return protocol_; }

    void set_method(method_type_t type) {
        type_ = type;
    }

    void set_path(const char *path) {
        BOOST_ASSERT( path != NULL );
        path_ = path;
    }

    void set_protocol(protocol_t protocol) {
        protocol_ = protocol;
    }

    void set_error() {}
};

boost::system::error_code parse_request_header(const char *buf, std::size_t length, http_request_t& request);

}} // namespace network { namespace http {
