#include "ClientMessage.h"
#pragma once


    ClientMessage::~ClientMessage() {}

    // Override the pure virtual function from the base class
    void ClientMessage::send() const {

        // Implementation for sending a message from the client
        std::cout << "Client sends: " << content << std::endl;
    }

    // Override the pure virtual function from the base class
    void ClientMessage::send(const std::string& sender, const std::string& receiver) const {
        // Implementation for sending a message from the client
        std::cout << "Client (" << sender << ") sends to " << receiver << ": " << content << std::endl;
    }

    void ClientMessage::readMultiFromConsole(char endChar) {

        std::cout << "Enter your message (end with " << endChar << "): ";
        std::string input;
        while (true) {
            char ch = std::cin.get();
            if (ch == endChar) break;
            input += ch;
        }
        processMessage(input);
    }

    // Special characteristic for reading from console input
    /**/
    void ClientMessage::readSingleFromConsole() {

        //std::cout << "Enter message: ";
        std::getline(std::cin, content);
    }

    void ClientMessage::displayMessage() const {

        std::cout << "Message: " << content << std::endl;
    };


    void ClientMessage::processMessage(const std::string& input) {

        std::istringstream iss(input);
        std::istream_iterator<std::string> start(iss), end;
        std::vector<std::string> words(start, end);

        std::ostringstream oss;
        for (const auto& word : words) {
            oss << word << " ";
        }
        content = oss.str();
    }
