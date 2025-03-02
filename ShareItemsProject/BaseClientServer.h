#pragma once
#include <memory>
#include <boost/bind/bind.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <mutex>
#include <boost/asio.hpp>
#include <boost/asio/buffered_write_stream.hpp>
#include <boost/asio/detail/win_iocp_handle_service.hpp>
#include <cstdint>
#include <string>
#include <chrono>
#include "../MessagesOC/MessageLengthPrefixed.h"
 
class MessageLP
{
public:
    uint32_t header;           // Size of the message body
    std::string body;          // Message content
    std::string sender;        // Sender's identifier
    std::chrono::system_clock::time_point timestamp; // Time when the message was created
    uint32_t checksum;         // Checksum for data integrity

    MessageLP() : header(0), checksum(0), timestamp(std::chrono::system_clock::now()) {};
    MessageLP( std::string message, std::string client) : body(message), header(sizeof(message)), sender(client), checksum(0), timestamp(std::chrono::system_clock::now()) {};

    // Convert the MessageLP structure to a string representation
    std::string to_string() const
    {
        std::stringstream ss;
        ss << "Header (Body Length): " << header << "\n";
        ss << "Body: " << body << "\n";
        ss << "Sender: " << sender << "\n";
        ss << "Timestamp: " << std::chrono::duration_cast<std::chrono::milliseconds>(timestamp.time_since_epoch()).count() << " ms\n";
        ss << "Checksum: " << checksum << "\n";
        return ss.str();
    }
};

class BaseClientServer
{

protected:

    void send_message(boost::asio::ip::tcp::socket& socket, const MessageLengthPrefixed& message);

    MessageLengthPrefixed receive_message(boost::asio::ip::tcp::socket& socket);

    virtual void writing_messages(boost::asio::ip::tcp::socket& socket, const std::string& message, const std::string& from) = 0;

    virtual bool reading_messages(std::shared_ptr<boost::asio::ip::tcp::socket>  socket) = 0;

public:


    void send_message1(boost::asio::ip::tcp::socket& socket, const std::string& message);

    const std::string& receive_message1(boost::asio::ip::tcp::socket& socket);

    /*
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


    void send_message_sync(const std::string& sender, boost::asio::ip::tcp::socket& socket, const std::string& body) {

        MessageLP message;
        message.header = static_cast<uint32_t>(body.size());
        message.body = body;
        message.sender = sender;

        std::vector<boost::asio::const_buffer> buffers;
        buffers.push_back(boost::asio::buffer(message.body));
        buffers.push_back(boost::asio::buffer(&message.header, sizeof(message.header)));
        buffers.push_back(boost::asio::buffer(message.sender, sizeof(message.sender)));
        write(socket, buffers, [](const boost::system::error_code& error, size_t) {
            if (!error) {
                // Message sent
            }
            });
    }
    */
    void write_message(boost::asio::ip::tcp::socket& socket, const MessageLP& message)
    {
        boost::system::error_code ec;

        // Serialize the message header (length of the body)
        std::vector<boost::asio::const_buffer> buffers;
        buffers.push_back(boost::asio::buffer(&message.header, sizeof(message.header)));

        // Serialize the message body
        buffers.push_back(boost::asio::buffer(message.body));

        // Serialize the sender
        buffers.push_back(boost::asio::buffer(message.sender, 256));

        // Serialize the timestamp (as a 64-bit integer)
        auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(message.timestamp.time_since_epoch()).count();
        buffers.push_back(boost::asio::buffer(&timestamp, sizeof(timestamp)));

        // Serialize the checksum
        buffers.push_back(boost::asio::buffer(&message.checksum, sizeof(message.checksum)));

        // Write the serialized buffers to the socket
        boost::asio::write(socket, buffers, ec);
        if (ec)
        {
            throw boost::system::system_error(ec, "Error writing message");
        };
    }


    MessageLP read_message(boost::asio::ip::tcp::socket& socket)
    {
        boost::system::error_code ec;

        // Read the message header (length of the body)
        uint32_t body_length;
        boost::asio::read(socket, boost::asio::buffer(&body_length, sizeof(body_length)), ec);
        if (ec)
        {
            throw boost::system::system_error(ec, "Error reading body length");
        }

        // Read the message body
        std::vector<char> body_buffer(body_length);
        boost::asio::read(socket, boost::asio::buffer(body_buffer), ec);
        if (ec)
        {
            throw boost::system::system_error(ec, "Error reading body");
        }

        // Read the sender (assuming a fixed maximum size of 256 bytes)
        std::vector<char> sender_buffer(256);
        size_t sender_size = boost::asio::read(socket, boost::asio::buffer(sender_buffer), ec);
        if (ec)
        {
            throw boost::system::system_error(ec, "Error reading sender");
        }

        // Read the timestamp (as a 64-bit integer)
        int64_t timestamp;
        boost::asio::read(socket, boost::asio::buffer(&timestamp, sizeof(timestamp)), ec);
        if (ec)
        {
            throw boost::system::system_error(ec, "Error reading timestamp");
        }

        // Read the checksum
        uint32_t checksum;
        boost::asio::read(socket, boost::asio::buffer(&checksum, sizeof(checksum)), ec);
        if (ec)
        {
            throw boost::system::system_error(ec, "Error reading checksum");
        }

        // Construct and return the message
        MessageLP message;
        message.header = body_length;
        message.body.assign(body_buffer.begin(), body_buffer.end());
        message.sender.assign(sender_buffer.begin(), sender_buffer.begin() + sender_size);
        message.timestamp = std::chrono::system_clock::time_point(std::chrono::milliseconds(timestamp));
        message.checksum = checksum;

        return message;
    }

    MessageLP receive_message_sync(boost::asio::ip::tcp::socket& socket)
    {
        // Read the message body size (header)
        uint32_t body_size;
        boost::system::error_code ec;
        boost::asio::read(socket, boost::asio::buffer(&body_size, sizeof(body_size)), ec);
        if (ec)
        {
            throw boost::system::system_error(ec, "Error reading body size");
        }

        // Read the message body
        std::vector<char> body_buffer(body_size);
        boost::asio::read(socket, boost::asio::buffer(body_buffer), ec);
        if (ec)
        {
            throw boost::system::system_error(ec, "Error reading body");
        }

        // Read the sender size
        std::vector<char> sender_buffer(256); // Assuming sender size is known (e.g., 256 bytes)
        size_t sender_size = boost::asio::read(socket, boost::asio::buffer(sender_buffer), ec);
        if (ec)
        {
            throw boost::system::system_error(ec, "Error reading sender");
        }

        // Construct and return the message
        MessageLP message;
        message.header = body_size;
        message.body = std::string(body_buffer.begin(), body_buffer.end());
        message.sender = std::string(sender_buffer.begin(), sender_buffer.begin() + sender_size);

        return message;
    };


};
