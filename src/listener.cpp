#include <iostream>

#include "listener.hpp"
#include "http_session.hpp"

Listener::Listener(
    net::io_context& ioc,
    tcp::endpoint endpoint,
    std::shared_ptr<SharedState> const& state)
    : acceptor_(ioc)
    , socket_(ioc)
    , state_(state)
{
    error_code ec;

    // Open the acceptor
    acceptor_.open(endpoint.protocol(), ec);
    if (ec) {
        fail(ec, "open");
        return;
    }

    // Allow address reuse
    acceptor_.set_option(net::socket_base::reuse_address(true));
    if (ec) {
        fail(ec, "set_option");
        return;
    }

    // Bind to the server address
    acceptor_.bind(endpoint, ec);
    if (ec) {
        fail(ec, "bind");
        return;
    }

    // Start listening for connections
    acceptor_.listen(net::socket_base::max_listen_connections, ec);
    if (ec){
        fail(ec, "listen");
        return;
    }
}

void Listener::run()
{
    // Start accepting a connection
    doAccept();
}

void Listener::fail(error_code ec, char const* what)
{
    // Don't report on canceled operations
    if(ec == net::error::operation_aborted) {
        return;
    }
    std::cerr << what << ": " << ec.message() << '\n';
}

void Listener::onAccept(error_code ec)
{
    if (ec) {
        return fail(ec, "accept");
    } else {
        // Launch new session for this connection
        std::make_shared<HttpSession>(std::move(socket_), state_)->run();
    }

    // Accept another connection
    doAccept();
}

void Listener::doAccept()
{
    acceptor_.async_accept(
        socket_,
        [self = shared_from_this()](error_code ec)
        {
            // The completion handler extends the lifetime of the listener
            self->onAccept(ec);
        });
}