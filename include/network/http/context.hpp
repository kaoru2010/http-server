#pragma once

#include <boost/asio.hpp>
#include <boost/asio/coroutine.hpp>
#include <boost/shared_ptr.hpp>

namespace network { namespace http {

#include <boost/asio/yield.hpp>
template <typename ConnectionContextCreator>
class socket_context : boost::asio::coroutine
{
    using self_t = socket_context;
    using tcp = boost::asio::ip::tcp;

    boost::asio::ip::tcp::acceptor& acceptor_;
    boost::shared_ptr<tcp::socket> socket_;
    ConnectionContextCreator& creator_;

public:
    socket_context(tcp::acceptor& acceptor, ConnectionContextCreator& creator)
    :   acceptor_(acceptor)
    ,   socket_()
    ,   creator_(creator)
    {}

    boost::asio::ip::tcp::socket& socket() {
        return *socket_;
    }

    void operator()(boost::system::error_code ec = {}, std::size_t length = 0) {
        if (ec)
            return;

        reenter(this) {
            do {
                socket_.reset(new tcp::socket(acceptor_.get_io_service()));
                yield acceptor_.async_accept(*socket_, *this);
                fork self_t(*this)();
            } while (is_parent());

            creator_.create(*socket_, *this)();
            socket_->shutdown(tcp::socket::shutdown_both, ec);
        }
    }

};
#include <boost/asio/unyield.hpp>

}} // namespace network { namespace http {
