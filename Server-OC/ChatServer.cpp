#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <mutex>
#include <boost/asio.hpp>
#include "ChatServer.h"

using boost::asio::ip::tcp;

void CharServer::broadcast_message(const std::string& message, std::shared_ptr<tcp::socket> sender_socket) {

    std::lock_guard<std::mutex> lock(clients_mutex);

    for (auto& client : clients) {

        if (client != sender_socket) {
            boost::asio::write(*client, boost::asio::buffer(message));
        }
    }
}

void CharServer::handle_client(std::shared_ptr<tcp::socket> client_socket) {

    try {

        char data[1024];
        while (true) {

            boost::system::error_code error;
            size_t length = client_socket->read_some(boost::asio::buffer(data), error);
            if (error) {
                std::cerr << "Client disconnected" << std::endl;
                break;
            }

            std::string message(data, length);
            std::cout << "Received: " << message << std::endl;

            // Broadcast message to all clients except the sender
            broadcast_message(message, client_socket);
        }
    }
    catch (const std::exception& e) {

        std::cerr << "Exception: " << e.what() << std::endl;
    }

    // Remove client from the list when they disconnect
    {

        std::lock_guard<std::mutex> lock(clients_mutex);
        auto it = std::find(clients.begin(), clients.end(), client_socket);
        if (it != clients.end()) {
            clients.erase(it);
        }
    }
    std::cout << "A client has left the room." << std::endl;
}

void CharServer::start_server(const std::string& port) {

    try {

        boost::asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), std::stoi(port)));

        std::cout << "Server is listening on port " << port << std::endl;

        while (true) {

            std::shared_ptr<tcp::socket> client_socket = std::make_shared<tcp::socket>(io_context);
            acceptor.accept(*client_socket);

            // Add the new client to the list
            {
                std::lock_guard<std::mutex> lock(clients_mutex);
                clients.push_back(client_socket);
            }

            std::cout << "A new client connected." << std::endl;

            // Handle client communication in a new thread
           std::thread th(&CharServer::handle_client, this, client_socket); 
           th.detach();
        }
    }
    catch (const std::exception& e) {

        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

 