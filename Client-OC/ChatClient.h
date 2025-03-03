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
#include <ConsoleHandler.h>

using boost::asio::ip::tcp;

class ChatClient : BaseClientServer
{
    ConsoleHandler handler;

    boost::asio::io_context& io_context_;  

    boost::asio::ip::tcp::resolver resolver_; 

    const std::string ip_; // 

    const std::string port_; ///unsigned int 

   // std::string buffer;  

    std::shared_ptr<boost::asio::ip::tcp::socket > socket_;

    std::string name_;

    bool  is_running_= false;

  

    void run();

    //***********************//


    /*loop function for receiving*/
    void receive_messages();

    void writing_messages(boost::asio::ip::tcp::socket& socket,const std::string& message, const std::string& from) override;

    bool reading_messages(std::shared_ptr<boost::asio::ip::tcp::socket>  socket) override;

    /* Checking For a Signal of Closing the Connsole By Ctrl+C or X right top corner 
    static BOOL WINAPI ConsoleHandler(DWORD signal) {
        if (signal == CTRL_CLOSE_EVENT) {
            std::cout << "Console window closed!" << std::endl;
            // Perform cleanup before the application exits
            // Return TRUE to indicate that you handled the event
           // is_running_ = false;
            return TRUE;
        }
        else if (signal == CTRL_C_EVENT) {
            std::cout << "Ctrl+C pressed!" << std::endl;
           // is_running_ = false;
            return TRUE;
        }
        return FALSE; // Let the system handle other signals normally
    };*/

public:

    void ClosingConnection();

    ChatClient(const std::string& name, boost::asio::io_context& io_context, const std::string& host, const std::string& port);

    std::shared_ptr<std::thread> communication_thread;

    /*loop function for sending*/
    bool send_messages();

    void start();

    void join();
    
    void connect();

};

#endif