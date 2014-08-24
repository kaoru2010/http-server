#include <network/http/server.hpp>

int main()
{
    boost::asio::io_service io_service;
    network::http::server server(io_service);

    return 0;
}
