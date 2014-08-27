#pragma once

#include <boost/system/error_code.hpp>

namespace network { namespace protocols { namespace http {

enum http_status_error
{
    NO_ERROR = 0,

    HTTP_CONTINUE = 100,
    HTTP_SWITCHING_PROTOCOLS = 101,

    HTTP_OK = 200,
    HTTP_CREATED = 201,
    HTTP_ACCEPTED = 202,
    HTTP_PARTIAL_CONTENT = 206,

    HTTP_MOVED_PERMANENTLY = 301,
    HTTP_MOVED_TEMPORARILY = 302,
    HTTP_NOT_MODIFIED = 304,

    HTTP_BAD_REQUEST = 400,
    HTTP_UNAUTHORIZED = 401,
    HTTP_FORBIDDEN = 403,
    HTTP_NOT_FOUND = 404,
    HTTP_METHOD_NOT_ALLOWED = 405,
    HTTP_REQUEST_TIME_OUT = 408,
    HTTP_REQUEST_ENTITY_TOO_LARGE = 413,
    HTTP_REQUEST_URI_TOO_LARGE = 414,

    HTTP_INTERNAL_SERVER_ERROR = 500,
    HTTP_NOT_IMPLEMENTED = 501,
    HTTP_VERSION_NOT_SUPPORTED = 505,
};

}}} // namespace network { namespace protocols { namespace http {

namespace boost { namespace system {

class http_status_error_category : public error_category
{
public:
    virtual const char* name() const noexcept {
        return "http_status";
    }

    virtual std::string message(int ev) const {
        using namespace network::protocols::http;

        switch (ev) {
            case HTTP_CONTINUE: return "Continue";
            case HTTP_SWITCHING_PROTOCOLS: return "Switching Protocols";

            case NO_ERROR:
            case HTTP_OK: return "OK";

            case HTTP_CREATED: return "Created";
            case HTTP_ACCEPTED: return "Accepted";
            case HTTP_PARTIAL_CONTENT: return "Partial Content";

            case HTTP_MOVED_PERMANENTLY: return "Moved Permanently";
            case HTTP_MOVED_TEMPORARILY: return "Moved Temporary";
            case HTTP_NOT_MODIFIED: return "Not Modified";

            case HTTP_BAD_REQUEST: return "Bad Request";
            case HTTP_UNAUTHORIZED: return "Unauthorized";
            case HTTP_FORBIDDEN: return "Forbidden";
            case HTTP_NOT_FOUND: return "Not Found";
            case HTTP_METHOD_NOT_ALLOWED: return "Method Not Allowed";
            case HTTP_REQUEST_TIME_OUT: return "Request Timeout";
            case HTTP_REQUEST_ENTITY_TOO_LARGE: return "Request Entity Too Large";
            case HTTP_REQUEST_URI_TOO_LARGE: return "Request-URI Too Long";

            default:
            case HTTP_INTERNAL_SERVER_ERROR: return "Internal Server Error";
            case HTTP_NOT_IMPLEMENTED: return "Not Implemented";
            case HTTP_VERSION_NOT_SUPPORTED: return "HTTP Version Not Supported";
        }
    }
};

inline
http_status_error_category& get_http_status_error_category() {
    static http_status_error_category instance;
    return instance;
}

template<>
struct is_error_condition_enum<network::protocols::http::http_status_error>
{
    static const bool value = true;
};

error_condition make_error_condition(network::protocols::http::http_status_error error) noexcept {
    return error_condition(error, get_http_status_error_category());
}

error_code make_error_code(network::protocols::http::http_status_error ev) {
    return error_code(ev, get_http_status_error_category());
}

}} // namespace boost { namespace system {
