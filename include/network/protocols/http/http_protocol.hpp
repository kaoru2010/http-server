#pragma once
#include <network/servers/connection.hpp>
#include <network/servers/server.hpp>
#include <network/protocols/http/router.hpp>

namespace network { namespace protocols { namespace http {

class http_protocol
{
    std::vector<std::unique_ptr<router>> routers_;

public:
    void add_router(std::unique_ptr<router>&& r);

    template <typename Router>
    void add_router() {
        std::unique_ptr<router> ptr(new Router());
        add_router(std::move(ptr));
    }

private:
    class http_protocol_context_t;

protected:
    auto create_connection_context(boost::asio::io_service& io_service) -> std::unique_ptr<servers::connection_t>;
};

}}} // namespace network { namespace protocols { namespace http {
