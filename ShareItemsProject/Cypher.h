/*Cipher Class(For message encryption and decryption)*/
#pragma once
class Cipher {
public:
    static std::string encrypt(const std::string& msg, int shift = 3) {
        std::string encrypted = msg;
        for (char& c : encrypted) {
            if (std::isalpha(c)) {
                char base = std::islower(c) ? 'a' : 'A';
                c = (c - base + shift) % 26 + base;
            }
        }
        return encrypted;
    }

    static std::string decrypt(const std::string& msg, int shift = 3) {
        return encrypt(msg, 26 - shift);  // Decrypt by shifting backwards
    }
};