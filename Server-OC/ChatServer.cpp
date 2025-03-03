#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <mutex>
#include <boost/asio.hpp>
#include "ChatServer.h"
#include <Cypher.h>
#include "ChatServer.h"
using boost::asio::ip::tcp;

/**/
ChatServer::ChatServer(boost::asio::io_context& io_context, const std::string& port, size_t thread_pool_size)
    :
    port_(port),
    acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), std::stoi(port))),
    io_context_(io_context),
    thread_pool_(thread_pool_size) {
  // start_accept();
    start_server_pool();
   //  start_server();
}
/*
TODO:
*/

/*
Broadcast message to the rest clients.
*/

void ChatServer::broadcast_message(std::shared_ptr<MessageLengthPrefixed> messagelp, std::shared_ptr<tcp::socket> sender_socket) {

    std::lock_guard<std::mutex> lock(clients_mutex);

    for (auto& client : clients_) {

        if (client != sender_socket) {

            // Send the message
            BaseClientServer::send_message(*client, messagelp);
            if (!(*messagelp).get_message().empty())
            {
                std::cout << "Message for sending: " << (*messagelp).get_message() << std::endl;
            }     
        }
    }
}

bool  ChatServer::reading_messages(std::shared_ptr<boost::asio::ip::tcp::socket>  client_socket) {

    // Receive the message
    std::shared_ptr <MessageLengthPrefixed> received_message = receive_message(*client_socket);
    if ((received_message) != nullptr) {

        // if (!(*received_message).get_message().empty()) {
        const std::string message = (*received_message).get_message();
        //std::cout << "Received message from ";
        std::cout <<"   " << (*received_message).get_client_name() << ": "
            << (*received_message).get_message() << std::endl;

        if (message == "exit")
        {
            const std::string client = (*received_message).get_client_name();
            MessageLengthPrefixed messagelp = MessageLengthPrefixed(client, "has left the room.");
            ChatServer::broadcast_message(std::make_shared<MessageLengthPrefixed>(messagelp), client_socket);  
            //std::cout << client + " has Left the Room." << std::endl;
            return false;
        }

        // Broadcast message to all clients except the sender
        ChatServer::broadcast_message(received_message, client_socket);
        //}
        return true;
    }
};

// Use write for blocking writes where you want to ensure that all data is written before proceeding.
//size_t bytes_written1 = boost::asio::write(socket_, boost::asio::buffer(message + " #"));

//Use write_some for non - blocking or asynchronous writes where you want to write as much data as possible immediately.
// Send the message using write_some


void ChatServer::writing_messages(boost::asio::ip::tcp::socket& socket_receiver, const std::string& message, const std::string& from)
{

// Create a message
    MessageLengthPrefixed messagelp(port_, message);

    // Send the message
    BaseClientServer::send_message(socket_receiver, make_shared<MessageLengthPrefixed>(messagelp));

    if (!messagelp.get_message().empty())
    {  
        std::cout << "sending: " << messagelp.get_message() << std::endl;    
    }
}

/*
Handle client - reading the messages from a accepted client.
*/
void ChatServer::handle_client(std::shared_ptr<tcp::socket> client_socket) {

     try {
       ///*while exit is not pressed or closing the window*/
        while (reading_messages(client_socket));
     }
    catch (std::exception& e) {

        //std::cerr << "Client Disconnected with error: " << e.what() << std::endl;
    }

    // Remove client from the list when they disconnect
    {
        remove_client(client_socket);
    }
    std::cout << "Client has Left the Room." << std::endl;
}


/*
Remove client when is disconnected.
*/
void ChatServer::remove_client(std::shared_ptr<tcp::socket> client_socket) {
    /*
    std::remove does not actually remove elements from the container; instead, it reorders the elements in the range such that
    the elements to be removed are moved to the end of the range.
    */
    //std::cout << "Client Disconnected." << std::endl;

    std::lock_guard<std::mutex> lock(clients_mutex);
    auto it = std::find(clients_.begin(), clients_.end(), client_socket);

    if (it != clients_.end()) {
        clients_.erase(it);
    }
};

/*
Using independent threads without using threadpool submit 
*/
void ChatServer::start_server() {

    try {

        std::cout << "Server Is listening On Port: " << port_ << std::endl;

        while (true) {

            std::shared_ptr<tcp::socket> client_socket = std::make_shared<tcp::socket>(io_context_);

            boost::system::error_code ec;
            acceptor_.accept(*client_socket, ec);
         
            // Add the new client to the list
            {
                std::lock_guard<std::mutex> lock(clients_mutex);
                clients_.push_back(client_socket);

            }

            // Handle client communication in a new thread
            std::thread th(&ChatServer::handle_client, this, client_socket);
            th.detach();
        }
    }
    catch (const std::exception& e) {

        std::cerr << "Connected Failed." << std::endl << "Exception: " << e.what() << std::endl;
    }
};


void ChatServer::start_server_pool() {

    try {

        std::cout << "Server Is listening On Port: " << port_ << std::endl;
        while (true) {
            //start_accept_async();
          start_accept();
        }
    }
    catch (const std::exception& e) {

        std::cerr << "Connected Failed." << std::endl << "Exception: " << e.what() << std::endl;
    }
};

void ChatServer::start_accept() {

    auto new_client = std::make_shared<boost::asio::ip::tcp::socket>(io_context_);

    boost::system::error_code error;
    acceptor_.accept(*new_client, error);

    if (!error) {

        handle_accept(new_client, error);
    }
    else {

        std::cerr << "Error during accept: " << error.message() << std::endl;
    }
}

void ChatServer::start_accept_async() {
    auto new_client = std::make_shared<boost::asio::ip::tcp::socket>(io_context_);
    acceptor_.async_accept(*new_client,
        [this, new_client](const boost::system::error_code& error) {
            handle_accept(new_client, error);
        });
};


void ChatServer::handle_accept(std::shared_ptr<tcp::socket> client_socket, const boost::system::error_code& error) {

    if (!error) {

        // Add the new client to the list
        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            clients_.push_back(client_socket);
        }

        /*thread pool adding the new connected client*/
        thread_pool_.submit(
            [this, client_socket]() {
                try {
                    handle_client(client_socket);
                }
                catch (const std::exception& e) {
                    std::cerr << "Exception in handle_client: " << e.what() << std::endl;
                    // Additional error handling logic can be placed here
                }
                catch (...) {
                    std::cerr << "Unknown exception in handle_client" << std::endl;
                    // Additional error handling logic for unknown exceptions
                }
            }
        );

    }
    else {
        std::cerr << "Connection Failed: " << error.message() << std::endl;
    }

   // start_accept();
}

