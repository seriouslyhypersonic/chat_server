/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef SHAREDSTATE_H
#define SHAREDSTATE_H

#include <memory>
#include <string>
#include <unordered_set>

// Forward declare
class WebSocketSession;

/**
 * Represents the shared server state - information that every object in the
 * system needs to have access to
 */
class SharedState
{
    /// Also an http server that serves html files, etc
    std::string documentRoot_;

    /**
     * This method of tracking sessions only works with an implicit strand
     * (i.e. a single-threaded server)
     */
    std::unordered_set<WebSocketSession*> sessions_;

public:
    explicit SharedState(std::string documentRoot);

    std::string const& documentRoot() const noexcept { return documentRoot_; }

    void join(WebSocketSession& session);
    void leave(WebSocketSession& session);
    void send(std::string message);
};

#endif //SHAREDSTATE
