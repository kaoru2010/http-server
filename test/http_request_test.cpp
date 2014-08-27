#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <string>
#include <network/http/http_request.hpp>

using namespace std;
using namespace network::http;

BOOST_AUTO_TEST_SUITE(http_request_test)

BOOST_AUTO_TEST_CASE(GET_simple)
{
    string request_text =
        "GET / HTTP/1.0\r\n"
        "Host: localhost\r\n"
        "\r\n"
        ;

    http_request_t r;
    if (auto ret = parse_request_header(request_text.data(), request_text.size(), r)) {
        cerr << ret.message() << "(" << ret << ")" << endl;
        BOOST_FAIL("auto ret = parse_request_header(request_text.data(), request_text.size(), r)");
        return;
    }

    BOOST_CHECK_EQUAL(GET, r.get_method());
    BOOST_CHECK_EQUAL("/", r.get_path());
    BOOST_CHECK_EQUAL(HTTP_1_0, r.get_protocol());
}

BOOST_AUTO_TEST_SUITE_END()
