#pragma once

#include "Network.h"

class Aplicacao
{
private:
	int _appFunctionType;

	// 0 = Aplicativo principal
	// 1 = Aplicativo replica
	int _appServiceType;

	void _appFunctions(UDPSocket* appSocket);

public:
	Aplicacao();
	~Aplicacao();

	void iniciar();
	
};