#include "ChatClient.h"
#include "../ShareItemsProject/Cypher.h"
 
// Send a message to the server
/**/
bool ChatClient::send_messages() {

    try {

        // Ensure the socket is properly initialized
        if (!(*socket_).is_open()) {
            std::cerr << "Socket is not open!" << std::endl;
            return false;
        }

        ClientMessage* cl_message = new ClientMessage();

        std::string message;

        while (true) {

            //  cl_message->readMultiFromConsole('#');
            cl_message->readSingleFromConsole();
            message = cl_message->getContent();

            writing_messages(*socket_, message, name_); 

            if (message == "exit")
            {
                close_window();
                break;
            }
        }
        return true;
    }
    catch (const std::exception& e) {

        std::cerr << "Exception: " << e.what() << std::endl;
    }

}

void ChatClient::close_window() {

    std::cout << "Client Requested to Exit.\n";
    stop();
    std::cout << "The Client Window Will Close In 5 Seconds..." << std::endl;
    Sleep(5000);

    HWND hwnd = GetConsoleWindow(); // Get a handle to the console window
    if (hwnd != nullptr)
    {
        PostMessage(hwnd, WM_CLOSE, 0, 0); // Post a WM_CLOSE message to the console window
    }
}

// Communication logic in a separate thread
/**/
void ChatClient::receive_messages() {

    try {

        while (true) {
            reading_messages(socket_);
        }
    }
    catch (const std::exception& e) {

        std::cerr << "Error in communication: " << e.what() << std::endl;
    }
};

 
/*
TODO:
*/

bool  ChatClient::reading_messages(std::shared_ptr<boost::asio::ip::tcp::socket>  socket) {

    //std::string message;
    //message = BaseClientServer::receive_message1(*socket);

    /* MessageLP lp_message = read_message(*socket_);

    std::cout << "Received from server: " << lp_message.to_string() << std::endl; */

    // Receive the message

    MessageLengthPrefixed received_message = receive_message(*socket);

    if (&received_message != nullptr) {

        const std::string message = received_message.get_message();

        if (message.length() > 0) {

            std::cout << "Received message from " << received_message.get_client_name() << ": "
                << received_message.get_message() << std::endl;
            return true;
        }
    }
    else return false;
}

// Use write for blocking writes where you want to ensure that all data is written before proceeding.
//size_t bytes_written1 = boost::asio::write(socket_, boost::asio::buffer(message + " #"));

//Use write_some for non - blocking or asynchronous writes where you want to write as much data as possible immediately.
// Send the message using write_some

void ChatClient::writing_messages(boost::asio::ip::tcp::socket& socket, const std::string& message, const std::string& from)
{
   // BaseClientServer::send_message1(socket, message);

    /*// Create a sample message to send
     MessageLP *lp_message = new MessageLP(message, name_);
    // Send the message to the server
   BaseClientServer::write_message(*socket_, *lp_message);

    std::cout << "Message sent to the server." << std::endl;
*/

// Create a message
    MessageLengthPrefixed messagelp(name_, message);

    // Send the message
    BaseClientServer::send_message(socket, messagelp);
    std::cout << "Message sent: " << messagelp.get_message() << std::endl;

}

void ChatClient::stop() {
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
        boost::asio::connect(*socket_, resolver_.resolve(ip_, port_));
        std::cout << name_ << " Connected Successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << name_ << " Connected Failed. " << e.what() << std::endl;
        return;  // Handle error appropriately
    }
};
