#include "Server.h"
#include <exception>
#include <thread>
#include <iostream>
#include "Logger.h"

Server::Server(int port, int iface) : port(port), iface(iface), serverSocket(INVALID_SOCKET), isRunning(true)
{
    WSADATA wsa_data = {};
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
        throw std::exception("WSAStartup Failed");

    this->serverSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (this->serverSocket == INVALID_SOCKET) {
        throw std::exception(__FUNCTION__ " - socket");
    }
}

Server::~Server()
{
    closeServer();

    // clean up all connected clients
    for (auto& socket : connectedClients) {
        closesocket(socket); // close each client socket
    }

    // close the server socket
    if (this->serverSocket != INVALID_SOCKET) {
        closesocket(this->serverSocket);
    }

    WSACleanup();
}

void Server::run()
{
    this->bindSocket();
    std::cout << "Listening for connections on port " << this->port << std::endl;

    listenThread = std::thread(&Server::listenForConnections, this);
}

void Server::bindSocket()
{
    struct sockaddr_in sa = { 0 };
    sa.sin_port = htons(this->port);
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = this->iface;
    if (::bind(this->serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
        throw std::exception(__FUNCTION__ " - bind");

    if (::listen(this->serverSocket, SOMAXCONN) == SOCKET_ERROR)
        throw std::exception(__FUNCTION__ " - listen");
}

void Server::listenForConnections()
{
    while (isRunning) {
        SOCKET clientSocket = accept(this->serverSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            this->loggerMutex.lock();
            Logger::log("Error accepting client");
            this->loggerMutex.unlock();

            std::cerr << "Error accepting client: " << WSAGetLastError() << std::endl;
        }
        else {
            this->loggerMutex.lock();
            std::cout << "New client connected." << std::endl;
            Logger::log("New client connected.");
            this->loggerMutex.unlock();
            this->connectedClients.push_back(clientSocket);
            std::thread clientThread(&Server::handleUser, this, clientSocket);
            clientThread.detach();
        }
    }
}

void Server::handleUser(SOCKET clientSocket)
{
    while (clientSocket != INVALID_SOCKET) {
        char buffer[1024];
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            // handle disconnection or error
            if (bytesReceived == 0) {
                this->loggerMutex.lock();
                Logger::log("Client disconnected.");
                this->loggerMutex.unlock();
            }
            else {
                this->loggerMutex.lock();
                Logger::log("Error in recv: " + std::to_string(WSAGetLastError()));
                this->loggerMutex.unlock();
            }

            auto it = std::find(connectedClients.begin(), connectedClients.end(), clientSocket);
            if (it != connectedClients.end()) {
                connectedClients.erase(it);
            }

            closesocket(clientSocket);
            clientSocket = INVALID_SOCKET;

            return;
        }
        else {
            // process data
        }
    }
}


void Server::notifyUsers()
{
    // Placeholder for notifying users
}

void Server::closeServer()
{
    isRunning = false;

    if (listenThread.joinable()) {
        listenThread.join();
    }
}
