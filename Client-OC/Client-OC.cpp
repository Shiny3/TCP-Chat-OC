// ServerB-OC.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "ChatClient.h" 
#include <iostream>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/connect.hpp>

#include <thread>

int main(int argc, char* argv[]) {

    if (argc != 3) {
        std::cerr << "Usage: chat_client <server> <port>" << std::endl;
        return 1;
    }

    ChatClient *chatClient = new ChatClient();

    std::string server = argv[1];
    std::string port = argv[2];

    try {
        boost::asio::io_context io_context;
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(server, port);
        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        std::cout << "Connected to the server." << std::endl;

        // Start receiving messages in a separate thread
        std::thread th(&ChatClient::receive_messages, chatClient, std::make_shared<tcp::socket>(std::move(socket)));

        th.detach();

        // Start sending messages
        chatClient->send_messages(std::make_shared<tcp::socket>(std::move(socket)));
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
