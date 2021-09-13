#pragma once

#include <string>
#include "Network.h"
#include <unordered_map>
#include <list>
using namespace std;

// Gerenciador de servidores
// Servidores s�o gerenciadores de uma ou mais aplica��es
class Superservidor
{
private:
	// Key   = ID Aplica��o
	// Value = Lista IP Servidor (principal e replicas)
	unordered_map<int, list<sockaddr_in*>> _serverList;

	UDPSocket _superServerSocket;

	int _failCheckingApplication;
	bool _failCheckResponse;

	void _failCheckThread();
	void _receiveApplications();

public:
	Superservidor();
	~Superservidor();

	void iniciar();
};