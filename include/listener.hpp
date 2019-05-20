/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef LISTENER_H
#define LISTENER_H

#include <memory>
#include <string>

#include "net.hpp"

// Forward declare
class SharedState;

/// Monitors the port, accepts incoming connections and launches the sessions
class Listener: public std::enable_shared_from_this<Listener>
{
    tcp::acceptor acceptor_;
    tcp::socket socket_;
    std::shared_ptr<SharedState> state_;

    void fail(error_code ec, char const* what); ///< Report a failure
    void onAccept(error_code ec);               ///< Handle a connection

    void doAccept();                            ///< Accept the connection

public:
    Listener(
        net::io_context& ioc,
        tcp::endpoint endpoint,
        std::shared_ptr<SharedState> const& state
    );

    /**
     * Start accepting incoming connections
     * @details A call to run is outstanding until it receives a connection.
     * Note that run extends the lifetime of the listener object
     */
    void run();
};


#endif //LISTENER
