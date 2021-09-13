#include "Superservidor.h"
#include <thread>
#include <vector>
#include <chrono>
using namespace std::chrono;
using namespace std;

void Superservidor::_failCheckThread()
{
	UDPSocket failCheckSocket;
	failCheckSocket.Bind(0);
	failCheckSocket.SetRecvTimeout(5000);

	UDPPacket packet;
	packet.a = 0;
	packet.b = 0;
	packet.response = 0;
	packet.sourceMessage = 0;

	while (true)
	{
		unordered_map<int, list<sockaddr_in*>> toDeleteServer;

		this_thread::sleep_for(seconds(5));

		cout << "Verificando estado dos servidores" << endl;

		// ID e Lista de Replicas
		for (auto appReg : _serverList)
		{
			for (auto appAddrSock : appReg.second)
			{
				packet.sourceMessage = 0;

				failCheckSocket.SendTo(*appAddrSock, (char*)& packet, sizeof(packet));
				failCheckSocket.RecvFrom((char*)& packet, sizeof(packet));

				if (packet.sourceMessage != 0)
				{
					toDeleteServer[appReg.first].push_back(appAddrSock);
					cout << "Um servidor da aplicação " << appReg.first << " não respondeu a tentativas de troca de mensagens" << endl;
				}
			}
		}

		if (toDeleteServer.size() > 0)
		{
			for (auto appReg : toDeleteServer)
			{
				for (auto appAddrSock : appReg.second)
				{
					_serverList[appReg.first].remove(appAddrSock);

					char ipAddr[INET_ADDRSTRLEN];
					unsigned short port = 0;

					inet_ntop(AF_INET, &appAddrSock->sin_addr, ipAddr, INET_ADDRSTRLEN);
					port = htons(appAddrSock->sin_port);

					cout << "Removido servidor " << ipAddr << ":" << port << endl;
				}
			}
		}
		
		packet.a = 0;
		packet.b = 0;
		packet.response = 0;
		packet.sourceMessage = 0;
	}
}

void Superservidor::_receiveApplications()
{
	while (true)
	{
		int appType = 0;

		sockaddr_in* applicationAddress = new sockaddr_in(_superServerSocket.RecvFrom((char*)& appType, 4));

		if (appType > 0)
			_serverList[appType].push_back(applicationAddress);
		else
			break;
		
		char ipAddr[INET_ADDRSTRLEN];
		unsigned short port = 0;

		inet_ntop(AF_INET, &applicationAddress->sin_addr, ipAddr, INET_ADDRSTRLEN);
		port = htons(applicationAddress->sin_port);

		cout << "Servidor " << ipAddr << ":" << port << " registrado" << endl;
		
		// 0 = Principal
		// 1 = Replica
		int response = _serverList[appType].size() > 1 ? 1 : 0;
		_superServerSocket.SendTo(*applicationAddress, (char*)& response, sizeof(response));
	}
}

Superservidor::Superservidor()
{
}

Superservidor::~Superservidor()
{
}

void Superservidor::iniciar()
{
	// Superservidor

	string ipAddr;
	unsigned short port;

	cout << "Digite o ip: ";
	cin >> ipAddr;
	cout << "Digite a porta: ";
	cin >> port;

	_superServerSocket.Bind(ipAddr, port);

	cout << "Configurando superservidor..." << endl;
	cout << "Recebendo aplicações" << endl;
	cout << "Digite 1 para iniciar o superservidor" << endl;

	int op = 0;

	thread receiveApps(&Superservidor::_receiveApplications, this);

	while (op != 1)
	{
		cin >> op;
	}

	// Finaliza thread de receber aplicações para registro
	op = -1;
	UDPSocket tempSocket;
	tempSocket.Bind(0);
	tempSocket.SendTo(ipAddr, port, (char*)&op, 4);

	op = 0;

	// Thread para verificar aplicações
	thread failCheckThread(&Superservidor::_failCheckThread, this);

	UDPSSResponse packet;

	while (op != -1)
	{
		// Thread para receber clientes
		int appId = 0;

		sockaddr_in* clientSocket = new sockaddr_in(_superServerSocket.RecvFrom((char*)& appId, sizeof(appId)));
		
		// Pega o primeiro servidor registrado do tipo de aplicação requisitado
		if (_serverList[appId].size() > 0)
		{
			packet.check = 0;
			packet.applicationAddress = **_serverList[appId].begin();
		}
		else
		{
			packet.check = 1;
			packet.applicationAddress = sockaddr_in();
		}

		_superServerSocket.SendTo(*clientSocket, (char*)&packet, sizeof(packet));
	}
}
