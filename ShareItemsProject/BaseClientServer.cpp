#include "BaseClientServer.h"

void BaseClientServer::send_message1(boost::asio::ip::tcp::socket& socket, const std::string& message) {

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

const std::string& BaseClientServer::receive_message1(boost::asio::ip::tcp::socket& socket) {
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

void BaseClientServer::send_message(boost::asio::ip::tcp::socket& socket, const MessageLengthPrefixed& message) {

    boost::system::error_code error;

    try {
        std::vector<uint8_t> data = message.to_bytes();
        size_t bytes_written = boost::asio::write(socket, boost::asio::buffer(data), error);
        if (error) throw;
    }
    catch (std::exception& e) {
        std::cerr << "Error writing to the socket: " << error.message() << std::endl;
        std::cerr << "Failed to send a message: " << e.what() << std::endl;
    }

    // Convert message to byte stream

};

MessageLengthPrefixed BaseClientServer::receive_message(boost::asio::ip::tcp::socket& socket) {
    boost::system::error_code error;

    try {
        // Receive data
        std::vector<uint8_t> buffer(1024);  // buffer to store incoming data
        size_t length = socket.read_some(boost::asio::buffer(buffer), error);

        if (error) {
            std::cerr << "Error receiving message: " << error.message() << std::endl;

        }
        if (length > 0) {
            // Decrypt message (for validation)
            //std::string decrypted_msg = Cipher::decrypt(std::string(data, length));
            //std::cout << "Received from server: " << decrypted_msg << std::endl;
           // std::cout << "Received from : " << std::string(data, length) << std::endl;
        }

        if (error) throw;
        // Extract the message from the buffer
        return MessageLengthPrefixed::from_bytes(std::vector<uint8_t>(buffer.begin(), buffer.begin() + length));
    }
    catch (std::exception& e) {

        std::cerr << "Error reading from the socket: " << error.message() << std::endl;
        std::cerr << "Failed to receive a message: " << e.what() << std::endl;
    }
}

