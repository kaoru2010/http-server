#include <iostream>
#include <string>
#include <network/http/http_request.hpp>

using namespace std;
using namespace network::http;

int main()
{
    string request_text =
        "GET / HTTP/1.0\r\n"
        "Host: localhost\r\n"
        "\r\n"
        ;

    http_request_t r;
    if (auto ret = parse_request_header(request_text.data(), request_text.size(), r)) {
        cerr << ret.message() << "(" << ret << ")" << endl;
        return 1;
    }

    return 0;
}
