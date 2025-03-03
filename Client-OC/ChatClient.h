#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#pragma once

#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include "ClientMessage.h"
#include "../ShareItemsProject/ThreadPool.h"
#include <BaseClientServer.h>
#include "../MessagesOC/MessageLengthPrefixed.h"

using boost::asio::ip::tcp;

class ChatClient : BaseClientServer
{
    boost::asio::io_context& io_context_;  

    boost::asio::ip::tcp::resolver resolver_; 

    const std::string ip_; // 

    const std::string port_; ///unsigned int 

   // std::string buffer;  

    std::shared_ptr<boost::asio::ip::tcp::socket > socket_;

    std::string name_;

    bool  is_running_= false;

    void ClosingConnection();

    void run();

    //***********************//


    /*loop function for receiving*/
    void receive_messages();

    void writing_messages(boost::asio::ip::tcp::socket& socket,const std::string& message, const std::string& from) override;

    bool reading_messages(std::shared_ptr<boost::asio::ip::tcp::socket>  socket) override;

public:

    ChatClient(const std::string& name, boost::asio::io_context& io_context, const std::string& host, const std::string& port);

    std::shared_ptr<std::thread> communication_thread;

    /*loop function for sending*/
    bool send_messages();

    void start();

    void join();
    
    void connect();

};

#endif