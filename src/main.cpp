#include <signal.h>

#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/coroutine.hpp>
#include <network/http/server.hpp>
#include <network/http/http_request.hpp>
#include <network/http/http_protocol.hpp>

using namespace network::http;

int main()
{
    boost::asio::io_service io_service;
    network::http::server<http_protocol> server(io_service);

    boost::asio::signal_set signals(io_service);
    signals.add(SIGINT);
    signals.add(SIGTERM);

#if defined(SIGQUIT)
    signals.add(SIGQUIT);
#endif // defined(SIGQUIT)

    signals.async_wait([&](const boost::system::error_code& error, int signal_number) {
        io_service.stop();
    });

    boost::system::error_code ec = server.listen("127.0.0.1", "10081");

    if (ec)
        return 1;

    io_service.run();

    return 0;
}
