#include "server.h"

using std::runtime_error;


Server::Server(PCSTR port) : m_port(port) {
    // initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        throw runtime_error("WSAStartup failed");
    }

    // initialize null sockets for later
    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;
}

Server::~Server() {
    WSACleanup();
}

void Server::startListening() {
    int iResult;
    struct addrinfo hints;
    struct addrinfo *result = NULL;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, m_port, &hints, &result);
    if (iResult != 0) {
        throw runtime_error("getaddrinfo failed");
    }

    // Create a SOCKET for the server to listen for client connections.
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        freeaddrinfo(result);
        throw runtime_error("socket failed");
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        freeaddrinfo(result);
        closesocket(ListenSocket);
        throw runtime_error("bind failed");
    }
    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        closesocket(ListenSocket);
        throw runtime_error("listen failed");
    }
}

void Server::acceptClient() {
    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        closesocket(ListenSocket);
        throw runtime_error("accept failed");
    }
    closesocket(ListenSocket);
}

void Server::handleClient() {
    while (TRUE) {
        // TODO catch exceptions to escape infinite loop
        handleCommand();
    }
}

void Server::disconnectClient() {
    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        closesocket(ClientSocket);
        throw runtime_error("shutdown failed");
    }
    closesocket(ClientSocket);
}

string Server::recvCommand() {
    int resultBytes;
    char commandLenBuffer[MESSAGE_LEN_SIZE];  // store the length of the message

    // receive the length of the command message
    resultBytes = recv(ClientSocket, commandLenBuffer, MESSAGE_LEN_SIZE, 0);
    if (resultBytes != MESSAGE_LEN_SIZE) {
        // TODO replace with custom message that can be caught later
        throw runtime_error("invalid message, expected length of type DWORD")
    }

    // allocate buffer for the new command, now that its length is known
    DWORD commandLen = static_cast<DWORD>(commandLenBuffer);
    char* commandBuffer = new char[commandLen];

    // receive the command to the allocated buffer
    resultBytes = recv(ClientSocket, commandBuffer, commandLen, 0);
    if (resultBytes != commandLen) {
        // TODO replace with custom message that can be caught later
        delete[] commandBuffer;
        throw runtime_error("invalid message, length mismatch")
    }

    // convert the message to dynamic cpp string and free allocated c-style buffer
    string command{commandBuffer, commandLen};
    delete[] commandBuffer;

    return command
}

void Server::sendResponse(string response) {
    // TODO send length
    // TODO send response
}

void Server::handleCommand() {
    std::string command = recvCommand();

    // generate the response message
    if (command.compare("PING") != 0) {
        sendResponse("PONG");
    } else {
        sendResponse("Unknown command");
    }
}