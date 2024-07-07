#include "Server.h"
#include <exception>
#include "thread"

Server::Server(int port, int iface) : port(port), iface(iface), serverSocket(INVALID_SOCKET)
{
	WSADATA wsa_data = { };
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
		throw std::exception("WSAStartup Failed");

	this->serverSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->serverSocket == INVALID_SOCKET) {
		throw std::exception(__FUNCTION__ " - socket");	
	}
}

Server::~Server()
{
	WSACleanup();
}

void Server::run()
{
	this->bindSocket();
	std::cout << "listening for connection on " << this->port;
	this->listenForConnections();
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
	SOCKET clientSocket = accept(this->serverSocket, NULL, NULL);
	if (clientSocket == INVALID_SOCKET) {
		std::cerr << "Error accepting client: " << WSAGetLastError() << std::endl;
		return;
	}

	std::cout << "New client connected." << std::endl;
	this->connectedClients.push_back(clientSocket);
	std::thread clientThread(&Server::handleUser, this, clientSocket);
	clientThread.detach();
}

void Server::handleUser(SOCKET clientSocket)
{
}

void Server::notifyUsers()
{
}
