#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <string>
#include <network/protocols/http/http_request.hpp>
#include <network/protocols/http/http_status.hpp>
#include <routes.hpp>

using namespace std;
using namespace network::protocols::http;

BOOST_AUTO_TEST_SUITE(routes_test)

BOOST_AUTO_TEST_CASE(routes_index)
{
    using boost::system::make_error_code;

    http_request_t r;
    auto ret = dispatch_controller("/index", r);

    BOOST_CHECK_EQUAL(make_error_code(HTTP_OK), ret);
}

BOOST_AUTO_TEST_SUITE_END()
