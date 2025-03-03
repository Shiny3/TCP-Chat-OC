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

    virtual ~BaseClientServer() {};

    /*Sending a Message with Lengh Prefixed Information with Extra information About the Sender*/
    void send_message(boost::asio::ip::tcp::socket& socket, std::shared_ptr<MessageLengthPrefixed> message);

    /*Receiving a Lengh Prefixed Message with Extra information About the Sender*/
    std::shared_ptr<MessageLengthPrefixed> receive_message(boost::asio::ip::tcp::socket& socket);

    /* Writing Messages From a Sender: Can be a Client or Server. In the Body is Calling
    #send_message function.
    Depends on the receiver console message and logic can be different, so it is virtual function. */
    virtual void writing_messages(boost::asio::ip::tcp::socket& socket, const std::string& message, const std::string& from) = 0;

    /* Reading Messages From a Sender: Can be a Client or Server. In the Body is Calling 
    #recieve_message function.
    Depends on the receiver console message and logic can be different, so it is virtual function. */
    virtual bool reading_messages(std::shared_ptr<boost::asio::ip::tcp::socket>  socket) = 0;

    /*Close Console Of Client By Entering: Exit.*/
    void close_window() {

        HWND hwnd = GetConsoleWindow(); // Get a handle to the console window
        if (hwnd != nullptr)
        {
            PostMessage(hwnd, WM_CLOSE, 0, 0); // Post a WM_CLOSE message to the console window
        }
    };

    /* Checking For a Signal of Closing the Connsole By Ctrl+C or X right top corner */
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
