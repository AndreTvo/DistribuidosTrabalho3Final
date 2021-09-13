#pragma once

#include "Network.h"

class Cliente
{
private:
	UDPSocket _clientSocket;
	sockaddr_in _applicationAddr;

public:
	Cliente();
	~Cliente();

	void iniciar();

};