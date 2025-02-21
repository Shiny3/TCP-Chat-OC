#pragma once
#ifndef SERVERMESSAGE_H
#define SERVERMESSAGE_H

#include "Message.h"
#include <iostream>

class ServerMessage : public Message {
public:
    virtual ~ServerMessage() {}

    // Override the pure virtual function from the base class
    void send() const override {

        // Implementation for sending a message from the server
        std::cout << "Server sends: " << content << std::endl;
    }

    // Override the pure virtual function from the base class
    void send(const std::string& sender, const std::string& receiver) const override {

        // Implementation for sending a message from the server
        std::cout << "Server (" << sender << ") sends to " << receiver << ": " << content << std::endl;
    }
};

#endif // SERVERMESSAGE_H
