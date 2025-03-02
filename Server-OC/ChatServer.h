#pragma once
#ifndef CHATSERVER_H
#define CHATSERVER_H


#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <mutex>
#include <boost/asio.hpp>
#include "../ShareItemsProject/ThreadPool.h"
#include <BaseClientServer.h>
#include "../MessagesOC/MessageLengthPrefixed.h"

using boost::asio::ip::tcp;

class ChatServer : BaseClientServer
{

private:

	void handle_client(std::shared_ptr<tcp::socket> client_socket);

	void handle_accept(std::shared_ptr<tcp::socket> client_socket, const boost::system::error_code& error);

	void remove_client(std::shared_ptr<tcp::socket>);

	void start_accept_async();

	void start_accept();

	void start_server();

	void start_server_pool();

	boost::asio::ip::tcp::acceptor acceptor_;

	boost::asio::io_context& io_context_;

	const std::string& port_;

	std::mutex clients_mutex;

	std::vector<std::shared_ptr<boost::asio::ip::tcp::socket>> clients_;

	bool init = false;

public:

	ChatServer(boost::asio::io_context& io_context, const std::string& port, size_t thread_pool_size);

	ThreadPool777 thread_pool_;

	void broadcast_message(MessageLengthPrefixed& messagelp, std::shared_ptr<tcp::socket> sender_socket);

	void writing_messages(boost::asio::ip::tcp::socket& socket, const std::string& message, const std::string& from) override;

	bool reading_messages(std::shared_ptr<boost::asio::ip::tcp::socket> socket) override;
};
#endif;