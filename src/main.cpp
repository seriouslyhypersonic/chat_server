#include <iostream>
#include <cstdlib>
#include <memory>

#include <boost/beast.hpp>
#include <boost/asio/signal_set.hpp>

#include "net.hpp"
#include "listener.hpp"
#include "shared_state.hpp"

#include <experimental/filesystem>
namespace filesystem = std::experimental::filesystem;

/// Creates and runs the server
int main(int argc, char** argv)
{
    // Check command line arguments
    if (argc != 4) {
        std::cerr <<
            "  Usage: chat_server <address> <port> <doc_root>\n"
            "Example:\n"
            "         chat_server 0.0.0.0 8080 .\n";
        return EXIT_FAILURE;
    }

    auto address = net::ip::make_address(argv[1]);
    auto port = static_cast<unsigned short>(std::atoi(argv[2]));
    auto documentRoot = argv[3];

    // The io_context is required for all I/O
    net::io_context ioc;

    // Create and launch a listening port
    std::make_shared<Listener>(
        ioc,
        tcp::endpoint{address, port},
        std::make_shared<SharedState>(documentRoot))->run();

    // Capture SIGINT and SIGTERM to perform a clean shutdown
    net::signal_set signals(ioc, SIGINT, SIGTERM);
    signals.async_wait(
        [&ioc](boost::system::error_code const&, int)
        {
            // Stop the io_context. This will cause run() to return
            // immediately, eventually destroying the io_context and any
            // remaining handlers int it.
            ioc.stop();
        });

    // Run the I/O service on the main thread (execution blocks here)
    ioc.run();

    // If we get here, it means we got a SIGINT or SIGTERM

    return EXIT_SUCCESS;
}