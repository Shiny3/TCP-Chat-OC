#pragma once
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime> 
#include <iomanip> // For std::put_time
//#include <openssl/md5.h>  // For checksum

// Message class with the required fields
class MessageLengthPrefixed {

private:
    std::string client_name_;
    std::string message_;
    size_t size_;
    std::chrono::system_clock::time_point timestamp_;
    /*std::vector<unsigned char> datasum_;

   // Calculate checksum using MD5 (for simplicity, using OpenSSL here)
   static std::vector<unsigned char> calculate_checksum(const std::string& data) {
       unsigned char md5_result[MD5_DIGEST_LENGTH];
       MD5_CTX md5_ctx;
       MD5_Init(&md5_ctx);
       MD5_Update(&md5_ctx, data.data(), data.size());
       MD5_Final(md5_result, &md5_ctx);

       return std::vector<unsigned char>(md5_result, md5_result + MD5_DIGEST_LENGTH);
   }*/


public:

    ~MessageLengthPrefixed() {
 
        // Cleanup code (if any) goes here
        // Since std::string and std::chrono::system_clock::time_point handle their own cleanup,
        // there may not be a need for explicit cleanup code here.
    }

    MessageLengthPrefixed(const std::string& client_name, const std::string& message)
        : client_name_(client_name), message_(message) {
        timestamp_ = std::chrono::system_clock::now();
        size_ = message.size();
       // datasum_ = calculate_checksum(message);
    }

    // Convert Message object to a byte stream for sending
    std::vector<uint8_t> to_bytes() const {
        std::vector<uint8_t> data;
        // Convert client name
        data.insert(data.end(), client_name_.begin(), client_name_.end());
        data.push_back('\0');  // Null-terminate the client name

        // Convert message size
        data.push_back(static_cast<uint8_t>(size_ >> 24));
        data.push_back(static_cast<uint8_t>(size_ >> 16));
        data.push_back(static_cast<uint8_t>(size_ >> 8));
        data.push_back(static_cast<uint8_t>(size_));

        // Convert message
        data.insert(data.end(), message_.begin(), message_.end());

        // Convert datasum (checksum)
        /*for (unsigned char byte : datasum_) {
            data.push_back(byte);
        }*/

        return data;
    }

    
    // Deserialize message from bytes received
    static  std::shared_ptr<MessageLengthPrefixed> from_bytes(const std::vector<uint8_t>& bytes) {

        std::string client_name;
        size_t index = 0;

        // Read client name
        while (bytes[index] != '\0') {
            client_name += bytes[index++];
        }
        index++;  // Skip the null terminator

        // Read size (4 bytes)
        size_t size = (bytes[index] << 24) | (bytes[index + 1] << 16) | (bytes[index + 2] << 8) | bytes[index + 3];
        index += 4;

        // Read message
        std::string message(bytes.begin() + index, bytes.begin() + index + size);
        index += size;

        // Read checksum (16 bytes, MD5 hash length)
        std::vector<unsigned char> datasum(bytes.begin() + index, bytes.end());
        //MessageLengthPrefixed messagelp =  MessageLengthPrefixed(client_name, message);
        return std::make_shared<MessageLengthPrefixed>(MessageLengthPrefixed(client_name, message));

    }

    const std::string& get_client_name() const { return client_name_; }
    const std::string& get_message() const { return message_; }
    size_t get_size() const { return size_; }
    //const std::vector<unsigned char>& get_datasum() const { return datasum_; }
    const std::chrono::system_clock::time_point& get_timestamp() const { return timestamp_; }

    /*void print_timestamp() const {
        auto time_t_timestamp = std::chrono::system_clock::to_time_t(timestamp_);
        std::cout << "Timestamp: " << std::put_time(std::localtime(&time_t_timestamp), "%Y-%m-%d %H:%M:%S") << std::endl;
    }*/

    void print_timestamp() const {

        MessageLengthPrefixed::print_timestamp(timestamp_);
    }

    static std::tm print_time_now() {

      return  MessageLengthPrefixed::print_timestamp(std::chrono::system_clock::now());    
        //return std::tm;
    }

    static std::tm print_timestamp(std::chrono::system_clock::time_point timestamp) {

        auto time_t_timestamp = std::chrono::system_clock::to_time_t(timestamp);
        std::tm tm_timestamp;
        localtime_s(&tm_timestamp, &time_t_timestamp);
       // std::cout << "Timestamp: " << std::put_time(&tm_timestamp, "%Y-%m-%d %H:%M:%S %ms") << std::endl;

        // Calculate milliseconds
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(timestamp.time_since_epoch()) % 1000;

        //std::cout << "Timestamp: ";
        std::cout << std::put_time(&tm_timestamp, "%Y-%m-%d %H:%M:%S")
            << '.' << std::setw(3) << std::setfill('0') << milliseconds.count();
          //  << std::endl;

        return tm_timestamp;
    }

    void calculate_delay_now() {
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - timestamp_);
        std::cout << "  Delay: " << duration.count() << " milliseconds" << std::endl;
    };

    static void calculate_delay(const std::chrono::system_clock::time_point& start,
        const std::chrono::system_clock::time_point& end) {
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Delay: " << duration.count() << " milliseconds" << std::endl;
    };
};
