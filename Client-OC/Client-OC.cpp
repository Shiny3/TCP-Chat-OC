// ServerB-OC.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "ChatClient.h" 
#include <iostream>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/connect.hpp>

#include <thread>



#include <iostream>
#include <windows.h>

 
int main(int argc, char* argv[]) {

    std::string IPAddress;
    std::string port;
    std::string name;

    try {
        if (argc != 4) throw;
        name = std::string(argv[1]);
        IPAddress = std::string(argv[2]);
        port = std::string(argv[3]);

    }
    catch (...) {
        std::cout << "Incorrect Format" << std::endl;
        std::cout << "Usage: <Client> <Server-IPAddress> <Server-Port>" << std::endl;
        return 0;
    }

    try {
        // Set up Boost.Asio io_context and thread pool
        boost::asio::io_context io_context;

        auto client_handler = std::make_shared<ChatClient>(name, io_context, IPAddress, port);
        client_handler->start();

        // Accept incoming connections and handle each client with a ClientHandler
        while (client_handler->send_messages()) {
            // Start sending messages
           
            client_handler->join();
        }
        io_context.stop();
    }
    catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
    };

    return 0;
};
