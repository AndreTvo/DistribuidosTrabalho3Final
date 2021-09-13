#pragma once

#include <string>
#include "Network.h"
#include <unordered_map>
#include <list>
using namespace std;

// Gerenciador de servidores
// Servidores são gerenciadores de uma ou mais aplicações
class Superservidor
{
private:
	// Key   = ID Aplicação
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