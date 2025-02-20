#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#pragma once

#include <iostream>
#include <thread>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class ChatClient
{

public:

    void receive_messages(std::shared_ptr<tcp::socket> socket);

    void send_messages(std::shared_ptr<tcp::socket> socket);

};

#endif