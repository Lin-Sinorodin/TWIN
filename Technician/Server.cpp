#include "server.h"

using std::runtime_error;
using std::to_string;


ServerException::ServerException(string error) : m_error(std::move(error)) {}


ServerException::ServerException(string error, DWORD code) : m_error(std::move(error)) {
    m_error += " (code = " + to_string(code) + ")";
}


const char* ServerException::what() const noexcept{
    return m_error.c_str();
}


Server::Server(PCSTR port) : m_port(port) {
    // initialize Winsock
    int iCode = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iCode != 0) {
        throw ServerException("WSAStartup failed", iCode);
    }

    // initialize null sockets for later
    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;
}


Server::~Server() {
    // clean Winsock
    WSACleanup();

    // clean sockets
    if (ListenSocket != INVALID_SOCKET) {
        closesocket(ListenSocket);
    }
    if (ClientSocket != INVALID_SOCKET) {
        closesocket(ClientSocket);
    }
}


void Server::startListening() {
    int iCode;
    ADDRINFOA hints;
    PADDRINFOA result = NULL;

    // Resolve the server address and port
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = DEFAULT_FAMILY;
    hints.ai_socktype = DEFAULT_SOCKTYPE;
    hints.ai_protocol = DEFAULT_PROTOCOL;
    hints.ai_flags = DEFAULT_FLAGS;
    iCode = getaddrinfo(NULL, m_port, &hints, &result);
    if (iCode != 0) {
        throw ServerException("getaddrinfo failed", iCode);
    }

    // Create a SOCKET for the server to listen for client connections.
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        freeaddrinfo(result);
        throw ServerException("socket failed", WSAGetLastError());
    }

    // Setup the TCP listening socket
    iCode = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iCode == SOCKET_ERROR) {
        freeaddrinfo(result);
        throw ServerException("bind failed", WSAGetLastError());
    }
    freeaddrinfo(result);

    iCode = listen(ListenSocket, SOMAXCONN);
    if (iCode == SOCKET_ERROR) {
        throw ServerException("listen failed", WSAGetLastError());
    }
}


void Server::acceptClient() {
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        throw ServerException("accept failed", WSAGetLastError());
    }

    // once we accept a client, we stop listening and only communicating with it
    closesocket(ListenSocket);
    ListenSocket = INVALID_SOCKET;
}


void Server::handleClient() {
    while (TRUE) {
        // TODO add option for connection closing in the communication protocol
        try {
            handleCommand();
        } catch (const ServerException& e) {
            std::cerr << "[!] ServerException: " << e.what() << std::endl;
            break;
        } catch (const std::exception& e) {
            std::cerr << "[!] Exception: " << e.what() << std::endl;
            break;
        }
    }
}


void Server::disconnectClient() {
    int iCode = shutdown(ClientSocket, SD_SEND);
    if (iCode == SOCKET_ERROR) {
        closesocket(ClientSocket);
        throw ServerException("shutdown failed", WSAGetLastError());
    }

    // as a shutdown send to the client, there is no use for the socket
    closesocket(ClientSocket);
    ClientSocket = INVALID_SOCKET;
}


string Server::recvCommand() {
    int recvBytes;
    char commandLenBuffer[MESSAGE_LEN_SIZE];  // store the length of the message

    // receive the length of the command message
    recvBytes = recv(ClientSocket, commandLenBuffer, MESSAGE_LEN_SIZE, 0);
    if (recvBytes == 0) {
        throw ServerException("connection closed");
    } else if (recvBytes < 0) {
        throw ServerException("recv failed", WSAGetLastError());
    } else if (recvBytes != MESSAGE_LEN_SIZE) {
        throw ServerException("invalid message length, expected DWORD");
    }

    // allocate buffer for the new command, now that its length is known
    DWORD commandLen = *(PDWORD)commandLenBuffer;
    char* commandBuffer = new char[commandLen];

    // receive the command to the allocated buffer
    recvBytes = recv(ClientSocket, commandBuffer, commandLen, 0);
    if (recvBytes == 0) {
        delete[] commandBuffer;
        throw ServerException("connection closed");
    } else if (recvBytes < 0){
        delete[] commandBuffer;
        throw ServerException("recv failed", WSAGetLastError());
    }

    // convert the message to dynamic cpp string and free allocated c-style buffer
    string command{commandBuffer, commandLen};
    delete[] commandBuffer;

    std::cout << "[+] (recv) command:  " << command << std::endl;
    return command;
}


void Server::sendResponse(string response) {
    int sendBytes;

    // send response length
    int responseLength = static_cast<int>(response.length());
    sendBytes = send(ClientSocket, (LPCSTR)(&responseLength), MESSAGE_LEN_SIZE, 0);
    if (sendBytes == SOCKET_ERROR) {
        throw ServerException("send response length failed", WSAGetLastError());
    }

    // send response
    sendBytes = send(ClientSocket, response.c_str(), responseLength, 0);
    if (sendBytes == SOCKET_ERROR) {
        throw ServerException("send response failed", WSAGetLastError());
    }
    
    std::cout << "[+] (send) response: " << response << std::endl;
}


void Server::handleCommand() {
    std::string command = recvCommand();

    // generate the response message
    if (command.compare("PING") == 0) {
        sendResponse("PONG");
    } else {
        sendResponse("Unknown command");
    }
}
