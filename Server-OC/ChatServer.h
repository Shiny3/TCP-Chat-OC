#pragma once
#ifndef CHATSERVER_H
#define CHATSERVER_H


#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <mutex>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class CharServer
{

private:

	std::vector<std::shared_ptr<tcp::socket>> clients;

	std::mutex clients_mutex;

	void broadcast_message(const std::string& message, std::shared_ptr<tcp::socket> sender_socket);

	void handle_client(std::shared_ptr<tcp::socket> client_socket);

public:

	void start_server(const std::string& port);
 
};
#endif;