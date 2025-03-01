#include "ChatClient.h"
#include "../ShareItemsProject/Cypher.h"

void ChatClient::start_pool() {
    try {
        //std::shared_ptr<ThreadPool777> thread_pool_;
        if (thread_pool_) {
            // Safe to use thread_pool_ here
            thread_pool_->post([this]() { run(); });
        }
        else {
            throw;
            // Handle error, thread_pool_ is null
            receive_messages();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error ThreadPool777 is not created: " << e.what() << std::endl;
        return;  // Handle error appropriately
    }
};

ChatClient::ChatClient(const std::string& name, boost::asio::io_context& io_context, const std::string& host, const std::string& port, std::shared_ptr<ThreadPool777> thread_pool)
   :
    name_(name), 
    io_context_(io_context), 
    socket_(std::make_shared<tcp::socket>(io_context_)), 
    resolver_(io_context), 
    ip_(host), 
    port_(port),
    thread_pool_(thread_pool)
{
    connect();
};

void ChatClient::connect() {

    try {
        // Connect the socket
        boost::asio::connect(*socket_, resolver_.resolve(ip_, port_));
        std::cout<<name_ << " Connected Successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << name_ << " Connected Failed. " << e.what() << std::endl;
        return;  // Handle error appropriately
    }
};


// Send a message to the server
/**/bool ChatClient::send_messages() {

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


            boost::system::error_code ec;
           
            try {
                // Use write for blocking writes where you want to ensure that all data is written before proceeding.
                //size_t bytes_written1 = boost::asio::write(socket_, boost::asio::buffer(message + " #"));

                //Use write_some for non - blocking or asynchronous writes where you want to write as much data as possible immediately.
                // Send the message using write_some

                /* BaseClientServer::send_message(*socket_, message);*/ 
                /*//comment message_to_server(message);*/
              

               //std::string encrypted_msg = Cipher::encrypt(message);
               //size_t bytes_written = (*socket_).write_some(boost::asio::buffer(encrypted_msg), ec);

               size_t bytes_written = (*socket_).write_some(boost::asio::buffer(message), ec);
               if (ec) throw;
                
               /*TODO: remove connection*/
               if (message == "exit")
               {
                   close_window();
                   break;
               }

               // boost::asio::write(socket_, boost::asio::buffer(encrypted_msg + "#"));
                
                //std::cout << "Message sent. " << std::endl;
                //std::cout << "Bytes written: " << bytes_written << std::endl;
            }
            catch (std::exception& e) {
                std::cerr << "Error writing to socket: " << ec.message() << std::endl;
                std::cerr << "Failed to send message: " << e.what() << std::endl;
            }
        }
        return true;
    }
    catch (const std::exception& e) {

        std::cerr << "Exception: " << e.what() << std::endl;
    }

};

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

        char data[1024];
        while (true) {

           // std::string message;
           //   message = BaseClientServer::receive_message(*socket_);
         
            /* MessageLP lp_message = read_message(*socket_);

            std::cout << "Received from server: " << lp_message.to_string() << std::endl; */

           boost::system::error_code error;
            size_t length = (*socket_).read_some(boost::asio::buffer(data), error);

            if (error) {
                std::cerr << "Error receiving message: " << error.message() << std::endl;
                break;
            }

            if (length > 0) {
                // Decrypt message (for validation)
                //std::string decrypted_msg = Cipher::decrypt(std::string(data, length));
                //std::cout << "Received from server: " << decrypted_msg << std::endl;
                std::cout << "Received from server: " << data << std::endl;
            }
        }
    }
    catch (const std::exception& e) {

        std::cerr << "Error in communication: " << e.what() << std::endl;
    }

};


/**/
void ChatClient::run() {
    try {
        char data[1024];
        while (is_running_) {

            boost::asio::streambuf buf;
            boost::system::error_code error;

          //  std::make_shared<tcp::socket>(std::move(socket_));
            size_t length = (*socket_).read_some(boost::asio::buffer(data), error);

           //boost::asio::read_until(socket_, buf, "#", error);
          // std::istream input(&buf);
          // std::getline(input, message_); 
           if (error && error != boost::asio::error::eof) {
                std::cerr << "Error reading received message from socket: " << error.message() << std::endl;
                return;
            }
            else if (length>0) {
                // Validate message
                if (data == "exit") {
                    std::cout << "Client requested to exit\n";
                    stop();
                }
                else {
                    // Encrypt message
                    std::string encrypted_msg = Cipher::encrypt(data);
                    std::cout << "Encrypted message: " << encrypted_msg << std::endl;

                    // Decrypt message (for validation)
                    std::string decrypted_msg = Cipher::decrypt(encrypted_msg);
                    std::cout << "Decrypted message: " << decrypted_msg << std::endl;

                    // Send response to client
                    send_messages();
                }
            }
        }
    }
    catch (std::exception& e) {
        std::cerr << "Error in client communication: " << e.what() << std::endl;
    }
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
   /**/ if (communication_thread && communication_thread->joinable()) {
        communication_thread->join();
    }
}
/**/
void ChatClient::message_to_server(const std::string& message)
{
    // Create a sample message to send
     MessageLP *lp_message = new MessageLP(message, name_);   
    // Send the message to the server
   BaseClientServer::write_message(*socket_, *lp_message);

    std::cout << "Message sent to the server." << std::endl;
}