#pragma once

#include <boost/system/error_code.hpp>

namespace network { namespace http {

enum http_status_error
{
    NO_ERROR         = 0,
    HTTP_BAD_REQUEST = 400,
    HTTP_METHOD_NOT_ALLOWED = 405,
    HTTP_INTERNAL_SERVER_ERROR = 500,
};

}} // namespace network { namespace http {

namespace boost { namespace system {

class http_status_error_category : public error_category
{
public:
    virtual const char* name() const noexcept {
        return "http_status";
    }

    virtual std::string message(int ev) const {
        using namespace network::http;

        switch (ev) {
            default:
            case HTTP_BAD_REQUEST: return "Bad Request";
            case HTTP_METHOD_NOT_ALLOWED: return "Method Not Allowed";
            case HTTP_INTERNAL_SERVER_ERROR: return "Internal Server Error";
        }
    }
};

template<>
struct is_error_condition_enum<network::http::http_status_error>
{
    static const bool value = true;
};

error_condition make_error_condition(network::http::http_status_error error) noexcept {
    return error_condition(error, http_status_error_category());
}

error_code make_error_code(network::http::http_status_error ev) {
    return error_code(ev, http_status_error_category());
}

}} // namespace boost { namespace system {
