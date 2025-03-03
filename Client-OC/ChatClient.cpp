#include "ChatClient.h"
#include "../ShareItemsProject/Cypher.h"
 
// Send a message to the server
bool ChatClient::send_messages() {

    try {

        // Ensure the socket is properly initialized
        if (!(*socket_).is_open()) {
            std::cerr << "Socket is not open!" << std::endl;
            return false;
        }

        ClientMessage* cl_message = new ClientMessage();

        std::string message;

        while (is_running_) {

            //  cl_message->readMultiFromConsole('#');
            cl_message->readSingleFromConsole();
            message = cl_message->getContent();

            writing_messages(*socket_, message, name_); 

            if (message == "exit")
            {
                std::cout << "Client Requested to Exit.\n";
               // ClosingConnection();
                std::cout << "The Client Window Will Close In 2 Seconds..." << std::endl;
                Sleep(2000);

                close_window();
                break;
            }
        }

        return is_running_;
    }
    catch (const std::exception& e) {

        std::cerr << "Exception: " << e.what() << std::endl;
    }
}


// Communication logic in a separate thread
void ChatClient::receive_messages() {

    try {

        /*while exit is not pressed or closing the window*/
        while (reading_messages(socket_ )); 
    }
    catch (const std::exception& e) {

        std::cerr << "Connection Failed.";
    }
};

bool  ChatClient::reading_messages(std::shared_ptr<boost::asio::ip::tcp::socket>  socket) {


    std::shared_ptr<MessageLengthPrefixed> received_message = receive_message(*socket);

    if (received_message != nullptr) {

        const std::string message = (*received_message).get_message();

        if (message.length() > 0) {

            //std::cout << "Sent At:  ";
            (*received_message).print_timestamp();// << std::endl;

            // std::cout << "Received message from " << (*received_message).get_client_name() << ": "
            //     << (*received_message).get_message() << std::endl;

            const std::string message = (*received_message).get_message();
            //std::cout << "Received message from ";
            std::cout << "   " << (*received_message).get_client_name() << ": "
                << (*received_message).get_message() << std::endl;

            //std::cout << " At:  ";
            //MessageLengthPrefixed::print_time_now();

            (*received_message).calculate_delay_now();
            return true;
        }
    }
    else {
        return false;
    }
    /* }
     catch (const std::exception& e) {

         std::cerr << "Error in communication: " << e.what() << std::endl;
     }*/
};

void ChatClient::writing_messages(boost::asio::ip::tcp::socket& socket, const std::string& message, const std::string& from)
{
    // Create a message
    MessageLengthPrefixed messagelp(from, message);

    // Send the message
    BaseClientServer::send_message(socket, make_shared<MessageLengthPrefixed>(messagelp));
    if (!messagelp.get_message().empty())
    {


        //std::cout << "Message for sending: " << messagelp.get_message() << std::endl;
    }

}

void ChatClient::ClosingConnection() {

    writing_messages(*socket_, "exit", name_);
    is_running_ = false;
   (*socket_).close();
}

/*
Start the communication thread and return a shared pointer to the thread
*/ 
void ChatClient::start() {
    // Create a shared_ptr to the communication thread to ensure it lives as long as needed
   communication_thread = std::make_shared<std::thread>(&ChatClient::receive_messages, this);
}

// Join the communication thread
void  ChatClient::join() {

    if (communication_thread && communication_thread->joinable()) {
        communication_thread->join();
    }
};

ChatClient::ChatClient(const std::string& name, boost::asio::io_context& io_context, const std::string& host, const std::string& port)
    :
    name_(name),
    io_context_(io_context),
    socket_(std::make_shared<tcp::socket>(io_context_)),
    resolver_(io_context),
    ip_(host),
    port_(port)
{
    connect();
};

void ChatClient::connect() {

    try {
        // Connect the socket
        is_running_ = true;
        boost::asio::connect(*socket_, resolver_.resolve(ip_, port_));
        writing_messages(*socket_, "Is Connected Successfully. ", name_);
        std::cout << name_ << " Connected Successfully!" << std::endl;
 
    }
    catch (const std::exception& e) {
        std::cerr << name_ << " Connected Failed. ";// << e.what() << std::endl;
        return;  // Handle error appropriately
    }
};
