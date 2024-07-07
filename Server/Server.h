#ifndef SERVER_H
#define SERVER_H

#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <vector>
#include <mutex>
#include <thread>

#pragma comment(lib, "Ws2_32.lib") // Link with Ws2_32.lib

class Server
{
public:
    Server(int port, int iface);
    ~Server();

    void run();
    void closeServer();

private:
    void bindSocket();
    void listenForConnections();
    void handleUser(SOCKET clientSocket);
    void notifyUsers();

    const int port;
    const int iface;

    SOCKET serverSocket;
    std::vector<SOCKET> connectedClients;
    std::mutex loggerMutex;
    std::thread listenThread;
    bool isRunning;
};

#endif // SERVER_H
