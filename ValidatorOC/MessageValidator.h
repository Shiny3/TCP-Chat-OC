#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

class MessageValidator {
public:
    // Minimum and Maximum length of a valid message
    static const size_t MIN_MESSAGE_LENGTH = 5;
    static const size_t MAX_MESSAGE_LENGTH = 256;

    // List of forbidden words (you can extend this list)
    static const std::vector<std::string> forbidden_words;

    // Validate the message
    static bool validate(const std::string& message) {
        // Check if the message is empty
        if (message.empty()) {
            std::cerr << "Error: Message cannot be empty!" << std::endl;
            return false;
        }

        // Check the message length
        if (message.length() < MIN_MESSAGE_LENGTH) {
            std::cerr << "Error: Message is too short. Minimum length is " << MIN_MESSAGE_LENGTH << " characters." << std::endl;
            return false;
        }

        if (message.length() > MAX_MESSAGE_LENGTH) {
            std::cerr << "Error: Message is too long. Maximum length is " << MAX_MESSAGE_LENGTH << " characters." << std::endl;
            return false;
        }

        // Check for forbidden words
        for (const std::string& word : forbidden_words) {
            if (message.find(word) != std::string::npos) {
                std::cerr << "Error: Message contains forbidden word: " << word << std::endl;
                return false;
            }
        }

        // Check for special characters (you can define specific rules here)
        if (contains_special_characters(message)) {
            std::cerr << "Error: Message contains invalid characters!" << std::endl;
            return false;
        }

        return true;
    }

private:
    // Helper function to check for special characters
    static bool contains_special_characters(const std::string& message) {
        // Check if the message contains any character that is not alphanumeric or a space
        return std::any_of(message.begin(), message.end(), [](unsigned char c) {
            return !std::isalnum(c) && c != ' ';
            });
    }
};

// List of forbidden words
const std::vector<std::string> MessageValidator::forbidden_words = { "badword1", "badword2", "offensiveword" };
