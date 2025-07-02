#pragma once

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <stdexcept>

#pragma comment (lib, "Ws2_32.lib")

using std::string;
constexpr DWORD MESSAGE_LEN_SIZE{ sizeof(DWORD) };


class Server {
public:
    Server(PCSTR port);
    ~Server();

    /* Initialize the server and move it to listening state. */
    void startListening();

    /* Accepts a client (stop listening, should only handle this connection). */
    void acceptClient();

    /* Handle stream of commands from the connected client */
    void handleClient();

    /* Handle stream of commands from the connected client */
    void disconnectClient();
private:
    PCSTR m_port;
    WSADATA wsaData;
    SOCKET ListenSocket;
    SOCKET ClientSocket;

    /* Receive a command from the client, first msg is length and second is command. */
    string recvCommand();

    /* Send a response to the client, first msg is length and second is command. */
    void sendResponse(string response);

    /* Receive one command from the client and send back the response. */
    void handleCommand();
};
