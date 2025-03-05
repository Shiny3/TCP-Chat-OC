#pragma once
#include <iostream>
#include <Windows.h>
#include <memory>
#include "ChatClient.h"
class ConsoleHandler {


private:

    // Static wrapper function that forwards the signal to the non-static method
    static BOOL WINAPI handlerWrapper(DWORD signal) {
        if (instance_) {
            return instance_->consoleHandler(signal);
        }
        return FALSE;
    }
public:

    static ConsoleHandler* instance_;

    std::function<void()> closing_connection_;  // Function to call when Ctrl+C is pressed

    // Constructor accepts a pointer to the ClosingConnection method of ChatClient
    ConsoleHandler(std::function<void()> closingConnectionFn) : closing_connection_(closingConnectionFn) {
   
        signal(SIGINT, &ConsoleHandler::HandleSignal);
    }

    // Static signal handler function
    static void HandleSignal(int signal) {

        if (signal == SIGINT) {
            std::cout << "\nCtrl+C detected. Closing connection..." << std::endl;
            // Call the provided closing connection function
            if (instance_) {
                instance_->closing_connection_();
            }
        }
    }
    BOOL consoleHandler(DWORD signal) {

        if (signal == CTRL_CLOSE_EVENT || signal == CTRL_C_EVENT) {
            std::cerr << "Console window is closing or Ctrl+C pressed. Performing cleanup..." << std::endl;
            if (instance_) {
                instance_->closing_connection_();
            }
            return TRUE;
        }
        return FALSE;
        //switch (signal) {
       // case CTRL_CLOSE_EVENT:
       //     std::cout << "Console window closed!" << std::endl;
       //     return TRUE;
       // case CTRL_C_EVENT:
        //    std::cout << "Ctrl+C pressed!" << std::endl;
        //    return TRUE;
        //default:
        //    return FALSE;
        //} 
    }

    // Static method to access instance
    static void setInstance(ConsoleHandler* handler) {
        instance_ = handler;
    }


};