#pragma once

#include <iostream>
#include <string>
#include <regex>
//#include <openssl/evp.h> // .in OpenSSL library for encryption
//#include <openssl/ssl.h> // OpenSSL library for SSL/TLS

class ChatValidator {
public:
    static bool isValidMessage(const std::string& message) {
        // Check message length (e.g., max 256 characters)
        if (message.length() > 256) return false;

        // Validate characters (alphanumeric and few special characters)
        std::regex validPattern("^[a-zA-Z0-9 .,!?\"]+$");
        return std::regex_match(message, validPattern);
    }
};

class ChatEncryptor {
public:
    static std::string encryptMessage(const std::string& message, const std::string& key) {
        // Encryption logic using OpenSSL (for simplicity, omitted detailed implementation)
        // Use AES encryption to encrypt the message with the given key
        // Ensure to handle padding and initialization vector (IV) properly

        // Pseudo-code for encryption:
        // 1. Initialize encryption context
        // 2. Set encryption key and IV
        // 3. Encrypt the message
        // 4. Return the encrypted message as a base64 encoded string

        return "encrypted_message"; // Placeholder
    }
};

int chat_calidator_main() {
    // Example usage
    std::string message = "Hello, world!";
    std::string key = "supersecretkey"; // Example key

    if (ChatValidator::isValidMessage(message)) {
        std::string encryptedMessage = ChatEncryptor::encryptMessage(message, key);
        std::cout << "Valid and Encrypted Message: " << encryptedMessage << std::endl;
    }
    else {
        std::cout << "Invalid message!" << std::endl;
    }

    return 0;
}
