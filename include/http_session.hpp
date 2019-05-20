/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef HTTPSESSION_H
#define HTTPSESSION_H

#include <cstdlib>
#include <memory>

#include "net.hpp"
#include "beast.hpp"
#include "shared_state.hpp"

namespace beast = boost::beast;

/// Represents an established HTTP connection (an instance per user)
class HttpSession: public std::enable_shared_from_this<HttpSession>
{
    tcp::socket socket_;
    beast::flat_buffer buffer_;
    std::shared_ptr<SharedState> state_;
    http::request<http::string_body> request_;

    void fail(error_code ec, char const* what);           ///< Report a failure
    void onRead(error_code ec, std::size_t);              ///< Handle a request
    void onWrite(error_code ec, std::size_t, bool close); ///< Handle a response

    void doRead(); ///< Reads a request
public:
    HttpSession(tcp::socket socket, std::shared_ptr<SharedState> const& state);

    void run();
};

#endif //HTTPSESSION
