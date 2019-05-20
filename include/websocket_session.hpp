/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef WEBSOCKETSESSION_H
#define WEBSOCKETSESSION_H

#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

#include "net.hpp"
#include "beast.hpp"
#include "shared_state.hpp"

// Forward declare ?
class SharedState;

class WebSocketSession: public std::enable_shared_from_this<WebSocketSession>
{
    beast::flat_buffer buffer_;
    websocket::stream<tcp::socket> webSocketStream_;
    std::shared_ptr<SharedState> state_;
    std::vector<std::shared_ptr<std::string const>> queue_;

    void fail(error_code ec, char const* what);
    void onAccept(error_code ec);
    void onRead(error_code ec, std::size_t bytesTransferred);
    void onWrite(error_code ec, std::size_t bytesTransferred);

    void doRead(); ///< Read
    void doWrite();
public:
    WebSocketSession(
        tcp::socket socket_,
        std::shared_ptr<SharedState> const& state);

    ~WebSocketSession();

    template<class Body, class Allocator>
    void run(http::request<Body, http::basic_fields<Allocator>> request);

    /// Send a message (to all)
    void send(std::shared_ptr<std::string const> const& sharedString);
};

template<class Body, class Allocator>
void WebSocketSession::run(
http::request<Body, boost::beast::http::basic_fields<Allocator>> request)
{
    // Accept the websocket handshake
    webSocketStream_.async_accept(
        request,
        [self = shared_from_this()](error_code const& ec)
        {
            self->onAccept(ec);
        });
}

#endif //WEBSOCKETSESSION
