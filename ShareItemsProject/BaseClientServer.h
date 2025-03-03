#pragma once
#include <memory>
#include <boost/bind/bind.hpp>
#include <iostream>
#include <thread>
 
#include <string>
#include <mutex>
#include <boost/asio.hpp>
#include <boost/asio/buffered_write_stream.hpp>
#include <boost/asio/detail/win_iocp_handle_service.hpp>
#include <cstdint>
#include <string>
#include <chrono>
#include "../MessagesOC/MessageLengthPrefixed.h"
#include <csignal>
#include <vector>
#include <cstdint>
#include <stdexcept>

class BaseClientServer
{

protected:

    //virtual ~BaseClientServer() {};

    /*void signalHandler(int signal) {
        std::cerr << "Console is closing. Signal: " << signal << std::endl;
        // Perform any cleanup or resource release here
        // ...
        std::exit(signal);
    };*/

    void send_message(boost::asio::ip::tcp::socket& socket, std::shared_ptr<MessageLengthPrefixed> message);

    std::shared_ptr<MessageLengthPrefixed> receive_message(boost::asio::ip::tcp::socket& socket);

    virtual void writing_messages(boost::asio::ip::tcp::socket& socket, const std::string& message, const std::string& from) = 0;

    virtual bool reading_messages(std::shared_ptr<boost::asio::ip::tcp::socket>  socket) = 0;

    void close_window() {


        HWND hwnd = GetConsoleWindow(); // Get a handle to the console window
        if (hwnd != nullptr)
        {
            PostMessage(hwnd, WM_CLOSE, 0, 0); // Post a WM_CLOSE message to the console window
        }
    };

   static BOOL WINAPI ConsoleHandler(DWORD signal) {
        if (signal == CTRL_CLOSE_EVENT) {
            std::cerr << "Console window is closing. Performing cleanup..." << std::endl;
            // Perform any cleanup or resource release here
            // ...
            return TRUE;
        }
        return FALSE;
    };

};
