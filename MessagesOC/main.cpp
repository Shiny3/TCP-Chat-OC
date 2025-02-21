#include <thread>
#include "ServerMessage.h"
#include "ClientMessage.h"

void serverThread() {

    ServerMessage serverMessage;
    serverMessage.setContent("Hello from the server!");
    serverMessage.send();
}

void clientThread() {

    ClientMessage clientMessage;
    clientMessage.readSingleFromConsole();
    clientMessage.readMultiFromConsole('#');
    clientMessage.displayMessage();
    clientMessage.send();
}

int main() {

    std::thread server(serverThread);
    std::thread client(clientThread);

    server.join();
    client.join();

    return 0;
}
