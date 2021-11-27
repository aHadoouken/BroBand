#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>

#include "Connection.h"
#include "Exceptions.h"
#include "Server.h"

Server::Server(const std::string& address, const std::string& port,
               std::size_t thread_pool_size)
        : acceptor_(net::make_strand(io_context_)),
          thread_pool_size_(thread_pool_size)
{
    // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
    boost::asio::ip::tcp::resolver resolver(io_context_);
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(address, port).begin();
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(net::socket_base::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();
}

void Server::Run() {
    StartAccept();

    // Create a pool of threads to run all of the io_contexts.
    std::vector<boost::shared_ptr<std::thread> > threads;
    for (std::size_t i = 0; i < thread_pool_size_; ++i)
    {
        boost::shared_ptr<std::thread> thread(new std::thread(
                boost::bind(&net::io_context::run, &io_context_)));
        threads.push_back(thread);
    }

    // Wait for all threads in the pool to exit.
    for (auto & thread : threads)
        thread->join();
}

void Server::StartAccept() {
    acceptor_.async_accept(
            net::make_strand(io_context_),
            beast::bind_front_handler(&Server::HandleAccept, shared_from_this()));
}

void Server::HandleAccept(beast::error_code e, tcp::socket socket) {
    if (!e)
    {
        std::cout << "accept!";

        auto c = std::make_shared<Connection>(std::move(socket), handlers);
        c->Start();
    }

    StartAccept();
}

void Server::HandleStop() {
    io_context_.stop();
}
