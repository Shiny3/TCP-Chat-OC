#pragma once

#ifndef CLIENTMESSAGE_H
#define CLIENTMESSAGE_H

#include "Message.h"
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>
#include <string>

class ClientMessage : public Message {

public:

    virtual ~ClientMessage();

    // Override the pure virtual function from the base class
    void send() const override;

    // Override the pure virtual function from the base class
    void send(const std::string& sender, const std::string& receiver) const;
    void readMultiFromConsole(char endChar);

    // Special characteristic for reading from console input
    /**/
    void readSingleFromConsole();

    void displayMessage() const;


private:

    void processMessage(const std::string& input);
};

#endif // CLIENTMESSAGE_H


