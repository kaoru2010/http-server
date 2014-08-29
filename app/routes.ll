%option noyywrap nounput noinput nodefault
%option stack
%option reentrant
%option extra-type="http_request_t *"
%option prefix="routes"
%{
#include <boost/system/error_code.hpp>
#include <network/protocols/http/http_request.hpp>
#include <network/protocols/http/http_status.hpp>

using namespace ::network::protocols::http;

http_status_error invoke_controller() { return HTTP_NOT_FOUND; }
%}

%%

"/index" return invoke_controller();
.|\n return HTTP_NOT_FOUND;

%%

namespace network { namespace protocols { namespace http {

boost::system::error_code parse_request_header(std::string const& url, http_request_t& request)
{
    using boost::system::make_error_code;

    yyscan_t scanner;
    yylex_init_extra(&request, &scanner);
    YY_BUFFER_STATE buffer_state = yy_scan_string(url.c_str(), scanner);

    int ret = yylex(scanner);

    yy_delete_buffer(buffer_state, scanner);
    yylex_destroy(scanner);

    return make_error_code(static_cast<http_status_error>(ret));
}

}}} // namespace network { namespace protocols { namespace http {

