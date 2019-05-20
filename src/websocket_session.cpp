#include <iostream>

#include "websocket_session.hpp"

WebSocketSession::WebSocketSession(
    tcp::socket socket_,
    std::shared_ptr<SharedState> const& state)
    : webSocketStream_(std::move(socket_))
    , state_(state)
{ }

WebSocketSession::~WebSocketSession()
{
    // Remove this session from the list of active sessions
    state_->leave(*this);
}

void WebSocketSession::fail(error_code ec, char const* what)
{
    // Don't report these
    if (ec == net::error::operation_aborted ||
        ec == websocket::error::closed) {
        return;
    }

    std::cerr << what << ": " << ec.message() << '\n';
}

void WebSocketSession::onAccept(error_code ec)
{
    if (ec) {
        return fail(ec, "accept");
    }

    // Add this session to the list of active sessions
    state_->join(*this);

    // Read a message
    doRead();
}

void WebSocketSession::onRead(error_code ec, std::size_t bytesTransferred)
{
    if (ec) {
        return fail(ec, "read");
    }

    // Send to all connections
    state_->send(beast::buffers_to_string(buffer_.data()));

    // Clear the buffer
    buffer_.consume((buffer_.size()));

    // Read another message
    doRead();
}

void WebSocketSession::send(
    std::shared_ptr<std::string const> const& sharedString)
{
    // Always add to queue
    queue_.push_back(sharedString);

    // Are we already writing?
    if (queue_.size() > 1) {
        return;
    }

    // We are not currently writing, so send immediately
    doWrite();
}

void WebSocketSession::onWrite(error_code ec, std::size_t bytesTransferred)
{
    if (ec) {
        return fail(ec, "write");
    }

    // Remove the string from the queue
    queue_.erase(queue_.begin());

    // Send the next message if any
    if(!queue_.empty()) {
        doWrite();
    }
}

void WebSocketSession::doRead()
{
    webSocketStream_.async_read(
        buffer_,
        [self = shared_from_this()]( error_code ec, std::size_t bytes)
        {
            self->onRead(ec, bytes);
        });
}

void WebSocketSession::doWrite()
{
    webSocketStream_.async_write(
        net::buffer(*queue_.front()),
        [self = shared_from_this()](error_code ec, std::size_t bytes)
        {
            self->onWrite(ec, bytes);
        });
}
