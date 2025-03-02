#include "BaseClientServer.h"


void BaseClientServer::send_message(boost::asio::ip::tcp::socket& socket, const MessageLengthPrefixed& message) {
    // Convert message to byte stream
    std::vector<uint8_t> data = message.to_bytes();

    // Send the message using write_some (as an example)
    boost::asio::write(socket, boost::asio::buffer(data));
}

MessageLengthPrefixed BaseClientServer::receive_message(boost::asio::ip::tcp::socket& socket) {
    // Receive data
    std::vector<uint8_t> buffer(1024);  // buffer to store incoming data
    size_t length = socket.read_some(boost::asio::buffer(buffer));

    // Extract the message from the buffer
    return MessageLengthPrefixed::from_bytes(std::vector<uint8_t>(buffer.begin(), buffer.begin() + length));
}