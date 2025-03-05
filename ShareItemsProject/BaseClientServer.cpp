#include "BaseClientServer.h"

void BaseClientServer::send_message(boost::asio::ip::tcp::socket& socket, std::shared_ptr<MessageLengthPrefixed> message) {
   // try {

        std::vector<uint8_t> data = (*message).to_bytes();
        size_t bytes_written = boost::asio::write(socket, boost::asio::buffer(data));
    /* }
    catch (std::exception& e) {
        std::cerr << "Failed to send a message: " << e.what() << std::endl;
    }*/
    // Convert message to byte stream
};


std::shared_ptr<MessageLengthPrefixed> BaseClientServer::receive_message(boost::asio::ip::tcp::socket& socket) {

    std::vector<uint8_t> buffer(1024);  // buffer to store incoming data
    size_t length = socket.read_some(boost::asio::buffer(buffer));
   // size_t length = boost::asio::read(socket,boost::asio::buffer(buffer));
    if (length > 0) {
        return MessageLengthPrefixed::from_bytes(std::vector<uint8_t>(buffer.begin(), buffer.begin() + length));
    }
};

