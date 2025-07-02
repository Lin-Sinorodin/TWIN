#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

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

// default values
constexpr DWORD MESSAGE_LEN_SIZE{ sizeof(DWORD) };
constexpr int DEFAULT_FAMILY{ AF_INET };
constexpr int DEFAULT_SOCKTYPE{ SOCK_STREAM };
constexpr int DEFAULT_PROTOCOL{ IPPROTO_TCP };
constexpr int DEFAULT_FLAGS{ AI_PASSIVE };

enum Command {
    PING_COMMAND,
    RUN_COMMAND,
    UNKNOWN_COMMAND
};


/* The exception thrown by the Server class, appends the error code to the provide message. */
class ServerException : public std::exception {
public:
    explicit ServerException(string error);
    explicit ServerException(string error, DWORD code);
    [[nodiscard]] const char* what() const noexcept override;
private:
    string m_error;
};


class Server {
public:
    explicit Server(PCSTR port);
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

    /* Resolve the command from string to one of the types in the Command enum. */
    Command resolveCommand(string command);

    /* Receive a command from the client, first msg is length and second is command. */
    string recvCommand();

    /* Send a response to the client, first msg is length and second is command. */
    void sendResponse(string response);

    /* Receive one command from the client and send back the response. */
    void handleCommand();
};
