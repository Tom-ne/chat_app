#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <vector>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib") // Link with Ws2_32.lib

class Server
{
#define BUFFER_SIZE 1024
public:
    Server(int port, int iface);
    ~Server();

    void run();
private:
    void bindSocket();
    void listenForConnections();
    void handleUser(SOCKET clientSocket);
    void notifyUsers();

    const int port;
    const int iface;

    SOCKET serverSocket;
    std::vector<SOCKET> connectedClients;
};
