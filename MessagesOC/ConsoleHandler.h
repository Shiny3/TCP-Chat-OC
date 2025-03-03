#include <iostream>
#include <Windows.h>
#include <memory>

class ConsoleHandler {
/*public:

    std::shared_ptr<ChatClient> clientInstance;

    ConsoleHandler(ChatClient client) : clientInstance(std::make_shared<ChatClient>(client)){
       
        if (!SetConsoleCtrlHandler(ConsoleHandler::handlerWrapper, TRUE)) {
            std::cerr << "Failed to set console control handler!" << std::endl;
        }
        else {
            std::cout << "Console control handler set!" << std::endl;
        }
    }

    BOOL consoleHandler(DWORD signal) {

        if (signal == CTRL_CLOSE_EVENT || signal == CTRL_C_EVENT) {
            std::cerr << "Console window is closing or Ctrl+C pressed. Performing cleanup..." << std::endl;
            if (clientInstance) {
               // (*clientInstance)->ClosingConnection();
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
    static void setInstance(std::shared_ptr<ConsoleHandler> handler) {
        instance = handler;
    }

 
private:
    static std::shared_ptr<ConsoleHandler> instance;

    // Static wrapper function that forwards the signal to the non-static method
    static BOOL WINAPI handlerWrapper(DWORD signal) {
        if (instance) {
            return instance->consoleHandler(signal);
        }
        return FALSE;
    }*/
};
