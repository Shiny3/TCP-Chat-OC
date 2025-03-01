#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <mutex>
#include <boost/asio.hpp>
#include "ChatServer.h"

using boost::asio::ip::tcp;

/**/
ChatServer::ChatServer(boost::asio::io_context& io_context, const std::string& port, size_t thread_pool_size)
    :
    port_(port),
    acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), std::stoi(port))),
    io_context_(io_context),
    thread_pool_(thread_pool_size) {
  // start_accept();
    start_server_pool();
   //  start_server();
}


/*
Handle client - reading the messages from a accepted client.
*/
void ChatServer::handle_client(std::shared_ptr<tcp::socket> client_socket) {

    try {

        char data[1024];
        while (true) {

            boost::system::error_code error;
            size_t length = client_socket->read_some(boost::asio::buffer(data), error);
            if (error) {
                std::cerr << "Reading failed..." << std::endl;
                break;
            }

            std::string message(data, length);
            std::cout << "Received: " << message << std::endl;

            // Broadcast message to all clients except the sender
            broadcast_message(message, client_socket);
        }
    }
    catch (std::exception& e) {

        std::cerr << "Client Disconnected with error: " << e.what() << std::endl;
    }

    // Remove client from the list when they disconnect
    {
        remove_client(client_socket);
    }
}

/*
Broadcast message to the rest clients.
*/
void ChatServer::broadcast_message(const std::string& message, std::shared_ptr<tcp::socket> sender_socket) {

    std::lock_guard<std::mutex> lock(clients_mutex);

    for (auto& client : clients_) {

        if (client != sender_socket) {

            boost::system::error_code error;
            size_t bytes_written = boost::asio::write(*client, boost::asio::buffer(message), error);
            if (!error) {
                std::cout << "Bytes written: " << bytes_written << std::endl;
            }
            else {
                std::cout << "Error: " << error.message() << std::endl;
            }
            // send_message(*client, message);
        }
    }
}

/*
Remove client when is disconnected.
*/
void ChatServer::remove_client(std::shared_ptr<tcp::socket> client_socket) {
    /*
    std::remove does not actually remove elements from the container; instead, it reorders the elements in the range such that
    the elements to be removed are moved to the end of the range.
    */
    std::cout << "Client Disconnected." << std::endl;

    std::lock_guard<std::mutex> lock(clients_mutex);
    auto it = std::find(clients_.begin(), clients_.end(), client_socket);

    if (it != clients_.end()) {
        clients_.erase(it);
    }

    std::cout << "A Client has Left the Room." << std::endl;
};

/*
Using independent threads without using threadpool submit 
*/
void ChatServer::start_server() {

    try {

        std::cout << "Server Is listening On Port: " << port_ << std::endl;

        while (true) {

            std::shared_ptr<tcp::socket> client_socket = std::make_shared<tcp::socket>(io_context_);

            boost::system::error_code ec;
            acceptor_.accept(*client_socket, ec);

            // Add the new client to the list
            {
                std::lock_guard<std::mutex> lock(clients_mutex);
                clients_.push_back(client_socket);
            }

            std::cout << "Client Is Connected Successfully ." << std::endl;

            // Handle client communication in a new thread
            std::thread th(&ChatServer::handle_client, this, client_socket);
            th.detach();
        }
    }
    catch (const std::exception& e) {

        std::cerr << "Connected Failed." << std::endl << "Exception: " << e.what() << std::endl;
    }
};


void ChatServer::start_server_pool() {

    try {

        std::cout << "Server Is listening On Port: " << port_ << std::endl;
        while (true) {
            //start_accept_async();
          start_accept();
        }
    }
    catch (const std::exception& e) {

        std::cerr << "Connected Failed." << std::endl << "Exception: " << e.what() << std::endl;
    }
};

void ChatServer::start_accept() {

    auto new_client = std::make_shared<boost::asio::ip::tcp::socket>(io_context_);

    boost::system::error_code error;
    acceptor_.accept(*new_client, error);

    if (!error) {

        handle_accept(new_client, error);
    }
    else {

        std::cerr << "Error during accept: " << error.message() << std::endl;
    }
}

void ChatServer::start_accept_async() {
    auto new_client = std::make_shared<boost::asio::ip::tcp::socket>(io_context_);
    acceptor_.async_accept(*new_client,
        [this, new_client](const boost::system::error_code& error) {
            handle_accept(new_client, error);
        });
};


void ChatServer::handle_accept(std::shared_ptr<tcp::socket> client_socket, const boost::system::error_code& error) {

    if (!error) {

        // Add the new client to the list
        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            clients_.push_back(client_socket);
        }

        std::cout << "Client Is Connected Successfully ." << std::endl;

        /*thread pool adding the new connected client*/
        thread_pool_.submit(
            [this, client_socket]() {
                try {
                    handle_client(client_socket);
                }
                catch (const std::exception& e) {
                    std::cerr << "Exception in handle_client: " << e.what() << std::endl;
                    // Additional error handling logic can be placed here
                }
                catch (...) {
                    std::cerr << "Unknown exception in handle_client" << std::endl;
                    // Additional error handling logic for unknown exceptions
                }
            }
        );

    }
    else {
        std::cerr << "Connection Failed: " << error.message() << std::endl;
    }

   // start_accept();
}

