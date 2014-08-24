%option noyywrap nounput noinput nodefault
%option stack
%option reentrant
%option extra-type="http_request_t *"
%{
#include <boost/system/error_code.hpp>
#include <network/http/http_request.hpp>
#include <network/http/http_status.hpp>

using namespace ::network::http;
%}

%x path protocol

%%

GET     { BEGIN(path); yyextra->set_method(GET); }
POST    { BEGIN(path); yyextra->set_method(POST); }
HEAD    { BEGIN(path); yyextra->set_method(HEAD); }
.+      { yyextra->set_error(); return HTTP_BAD_REQUEST; }
\n      { yyextra->set_error(); return HTTP_BAD_REQUEST; }

<path>{
    " "         ;
    [^ ]+       { BEGIN(protocol); yyextra->set_path(yytext); }
    \n          { yyextra->set_error(); return HTTP_BAD_REQUEST; }
}

<protocol>{
    " "         ;
    HTTP/1.0    { yyextra->set_protocol(HTTP_1_0); }
    HTTP/1.1    { yyextra->set_protocol(HTTP_1_1); }
    .+          { yyextra->set_error(); return HTTP_BAD_REQUEST; }
    \n          { yyextra->set_error(); return HTTP_BAD_REQUEST; }
}

%%

namespace network { namespace http {

boost::system::error_code parse_status_line(const char *buf, std::size_t length, http_request_t& request) {
    using boost::system::make_error_code;

    yyscan_t scanner;
    yylex_init_extra(&request, &scanner);
    YY_BUFFER_STATE buffer_state = yy_scan_bytes(buf, length, scanner);

    int ret = yylex(scanner);

    yy_delete_buffer(buffer_state, scanner);
    yylex_destroy(scanner);

    return make_error_code(static_cast<http_status_error>(ret));
}

}} // namespace network { namespace http {1
