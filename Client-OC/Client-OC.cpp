// ServerB-OC.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "ChatClient.h" 
#include <iostream>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/connect.hpp>

#include <thread>

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

       // Create a thread pool with 4 threads
        auto thread_pool = std::make_shared<ThreadPool777>(4);

        // Start the thread pool to handle the io_context in multiple threads
        thread_pool->run();

       // ChatClient* client_handler = new ChatClient(name, io_context, IPAddress, port, thread_pool);
        auto client_handler = std::make_shared<ChatClient>(name,io_context, IPAddress, port, thread_pool);

        // //client_handler->connect();
       client_handler->start();

       // Accept incoming connections and handle each client with a ClientHandler
        while (true) {

          // Start sending messages
          
          /**/
          client_handler->send_messages();
          client_handler->join();
         /*
           client_handler->start_pool();
           client_handler->send_messages(); */
        }

        // Stop the thread pool and io_context when done (this will never be reached in this loop)
        thread_pool->stop();
        io_context.stop(); /*  */ 
    }
    catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
    }

    return 0;
};
