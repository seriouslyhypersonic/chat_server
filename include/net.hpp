/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef NET_H
#define NET_H

#include <boost/asio.hpp>

namespace net = boost::asio;       // From <boost/asio.hpp>
using tcp  = boost::asio::ip::tcp; // From <boost/asio/ip/tcp.hpp>
using error_code = boost::system::error_code;

#endif //NET_H
