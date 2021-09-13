#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <system_error>
#include <string>
#include <iostream>

using namespace std;

// Pacote para a classe Aplicacao
struct UDPPacket
{
	int check;

	// 0 = Superservidor
	// 1 = Cliente
	int sourceMessage;

	double a;
	double b;
	double response;
};

struct UDPSSResponse
{
	int check;
	sockaddr_in applicationAddress;
};

class WSASession
{
public:
	WSASession();
	~WSASession();
private:
	WSAData data;
};

class UDPSocket
{
public:
	UDPSocket();
	~UDPSocket();

	void SetRecvTimeout(int time);
	void JoinMulticast(string groupIp);

	void SendTo(const std::string& address, unsigned short port, const char* buffer, int len, int flags = 0);
	void SendTo(sockaddr_in& address, const char* buffer, int len, int flags = 0);

	sockaddr_in RecvFrom(char* buffer, int len, int flags = 0);
	sockaddr_in RecvFrom(char* buffer, int len, int& bytes, int flags);

	void Bind(const std::string& address, unsigned short port);
	void Bind(unsigned short port);

	SOCKET& GetSOCKET();

private:
	bool timeoutSet;

	SOCKET sock;
};