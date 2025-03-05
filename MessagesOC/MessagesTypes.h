#pragma once
#include <iostream>
#include <string>

class Messages {
public:
    // Enum representing different message types
    enum class MessageType {
        EXIT,
        CONNECT,
        DISCONNECT,
        CHAT,
        ERROR__,
        UNKNOWN
    };

    // Function to convert the enum to a string
    static std::string getMessageTypeString(MessageType messageType) {
        switch (messageType) {
        case MessageType::EXIT:
            return "EXIT";
        case MessageType::CONNECT:
            return "CONNECT";
        case MessageType::DISCONNECT:
            return "DISCONNECT";
        case MessageType::CHAT:
            return "CHAT";
        case MessageType::ERROR__:
            return "ERROR";
        default:
            return "UNKNOWN";
        }
    }

    // Function to compare enum with string
    static bool compareMessageType(MessageType messageType, const std::string str) {

        int result = getMessageTypeString(messageType).compare(str);

        if (result == 0)
            return true;

        return false;
    }
};

