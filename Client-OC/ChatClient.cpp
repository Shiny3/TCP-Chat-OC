#include "ChatClient.h"
#pragma once

#include <iostream>
#include <thread>
#include <boost/asio.hpp>
 
#include "ClientMessage.h"

void ChatClient::receive_messages(std::shared_ptr<tcp::socket> socket) {

    try {

        char data[1024];
        while (true) {

            boost::system::error_code error;
            size_t length = socket->read_some(boost::asio::buffer(data), error);
            if (error) {
                std::cerr << "Error receiving message: " << error.message() << std::endl;
                break;
            }

            std::string message(data, length);
            std::cout << message << std::endl;
        }
    }
    catch (const std::exception& e) {

        std::cerr << "Exception: " << e.what() << std::endl;
    }
};

void ChatClient::send_messages(std::shared_ptr<tcp::socket> socket) {

    try {
        ClientMessage* cl_message = new ClientMessage();

        std::string message;
        while (true) {

            cl_message->readSingleFromConsole();
           // std::getline(std::cin, message);
            boost::asio::write(*socket, boost::asio::buffer(message));
        }
    }
    catch (const std::exception& e) {

        std::cerr << "Exception: " << e.what() << std::endl;
    }

};



