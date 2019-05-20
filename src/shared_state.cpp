#include <iostream>

#include <boost/date_time.hpp>

#include "shared_state.hpp"
#include "websocket_session.hpp"

SharedState::SharedState(std::string documentRoot)
    : documentRoot_(std::move(documentRoot))
{ }

void SharedState::join(WebSocketSession& session)
{
    sessions_.insert(&session);
}

void SharedState::leave(WebSocketSession& session)
{
    sessions_.erase(&session);
}

void SharedState::send(std::string message)
{
    auto const sharedMessage =
        std::make_shared<std::string const>(std::move(message));

    // Send to each connected WebSocket session
    for (auto session : sessions_) {
        session->send(sharedMessage);
    }

    // Show the sent message on cout
    auto localTime = boost::posix_time::second_clock::universal_time();
    std::cout << '[' << localTime << "] " << *sharedMessage << '\n';
}