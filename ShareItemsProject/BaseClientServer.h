#pragma once
#include <memory>
#include <boost/bind/bind.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <mutex>
#include <boost/asio.hpp>
 

class BaseClientServer
{



protected:

    struct MessageLP {
    public:
        uint32_t header;
        std::string body;
    };

    void send_message(boost::asio::ip::tcp::socket& socket, const std::string& message) {

        // Create a buffer for the message size (4 bytes for a 32-bit integer)
        uint32_t message_size = static_cast<uint32_t>(message.size());

        // Convert the message size to network byte order (big-endian)
        message_size = htonl(message_size);

        // Create a buffer for the message header (4 bytes)
        boost::asio::streambuf header_buffer;
        std::ostream header_stream(&header_buffer);
        header_stream.write(reinterpret_cast<char*>(&message_size), sizeof(message_size));

        // Send the header (size information)
        boost::asio::write(socket, header_buffer);

        // Send the message body (the actual message)
        boost::asio::write(socket, boost::asio::buffer(message));
    }

    const std::string& receive_message(boost::asio::ip::tcp::socket& socket) {
        // First, read the header (4 bytes for message length)
        uint32_t message_size;
        boost::asio::read(socket, boost::asio::buffer(&message_size, sizeof(message_size)));

        // Convert from network byte order to host byte order
        message_size = ntohl(message_size);

        // Read the message body
        std::vector<char> buffer(message_size);
        boost::asio::read(socket, boost::asio::buffer(buffer));

        std::string message(buffer.begin(), buffer.end());
        std::cout << "Received message: " << message << std::endl;

        return message;
    }

    void send_message_async(boost::asio::ip::tcp::socket& socket, const std::string& body) {
        MessageLP message;
        message.header = static_cast<uint32_t>(body.size());
        message.body = body;
        std::vector<boost::asio::const_buffer> buffers;
        buffers.push_back(boost::asio::buffer(&message.header, sizeof(message.header)));
        buffers.push_back(boost::asio::buffer(message.body));
         async_write(socket, buffers, [](const boost::system::error_code& error, size_t) {
            if (!error) {
                // Message sent
            }
            });
    }


};
