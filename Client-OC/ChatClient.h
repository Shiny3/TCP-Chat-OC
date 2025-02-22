#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#pragma once

#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "../ShareItemsProject/ThreadPool.h"

#include <iostream>

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include "ClientMessage.h"
#include "../ShareItemsProject/ThreadPool.h"

using boost::asio::ip::tcp;

class ChatClient
{
    boost::asio::io_context& io_context_;  

    boost::asio::ip::tcp::resolver resolver_; 

    const std::string ip_; // 

    const std::string port_; ///unsigned int 

   // std::string buffer;  

    std::shared_ptr<boost::asio::ip::tcp::socket > socket_;

    std::shared_ptr<ThreadPool777> thread_pool_;

    std::string name_;

    bool is_running_;

public:

  //  void send_message(const std::string& msg);

    void connect();

    void stop();

    void run();

    void join();

    //***********************//
    void start();

    void receive_messages();

    bool send_messages();

    ChatClient(const std::string& name, boost::asio::io_context& io_context, const std::string& host, const std::string& port, std::shared_ptr<ThreadPool777> thread_pool);

    std::shared_ptr<std::thread> communication_thread;

    void start_pool();
};

#endif