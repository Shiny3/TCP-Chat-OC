#pragma once
 
#ifndef MESSAGE_H
#define MESSAGE_H
/*
#ifdef MESSAGE_EXPORTS
#define Message_API __declspec(dllexport)
#else
#define MESSAGE_API __declspec(dllimport)
#endif

//extern "C"  MESSAGE_API void Message();
*/
#include <string>
#include <boost/asio.hpp>
#include <iostream>
#include "MessageLengthPrefixed.h"

class Message {

public:

    virtual ~Message() {}

    // Pure virtual function to be overridden by derived classes
    virtual void send() const = 0;

    virtual void send(const std::string& sender, const std::string& receiver) const = 0;

    // Common member function to get the content of the message
    std::string getContent() const {
        return content;
    }

    // Common member function to set the content of the message
    void setContent(const std::string& content) {
        this->content = content;
    }

protected:

    std::string content; // Content of the message

};

#endif // MESSAGE_H
 
