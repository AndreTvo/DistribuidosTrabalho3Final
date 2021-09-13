#include "Aplicacao.h"
#include "Network.h"

void Aplicacao::_appFunctions(UDPSocket* appSocket)
{
	UDPPacket packetData;
	sockaddr_in clientAddr = appSocket->RecvFrom((char*)& packetData, sizeof(packetData));

	// Caso Superservidor
	if (packetData.sourceMessage == 0)
	{
		cout << "Mensagem de verificação do superservidor" << endl;
		appSocket->SendTo(clientAddr, (char*)&packetData, sizeof(packetData));
	}
	// Caso Cliente
	else if (packetData.sourceMessage == 1)
	{
		char ipAddr[INET_ADDRSTRLEN];
		unsigned short port = 0;

		inet_ntop(AF_INET, &clientAddr.sin_addr, ipAddr, INET_ADDRSTRLEN);
		port = htons(clientAddr.sin_port);

		cout << "Cliente " << ipAddr << ":" << port << endl;
		cout << "A = " << packetData.a << endl;
		cout << "B = " << packetData.b << endl;

		switch (_appFunctionType)
		{
			case 1:
			{
				// Soma
				int response = packetData.a + packetData.b;
				packetData.response = response;
				break;
			}

			case 2:
			{
				// Multiplicacao
				double response = packetData.a * packetData.b;
				packetData.response = response;
				break;

			}

			case 3:
			{
				// Divisao
				double response = packetData.a / packetData.b;
				packetData.response = response;
				break;
			}
		}

		cout << "Resultado = " << packetData.response << endl;
		cout << "Enviando resposta..." << endl;

		appSocket->SendTo(clientAddr, (char*)& packetData, sizeof(packetData));
	}
}

Aplicacao::Aplicacao()
{
	_appFunctionType = 0;
	_appServiceType = 0;

	// Socket to connect to super server
}

Aplicacao::~Aplicacao()
{

}

void Aplicacao::iniciar()
{
	int op = 1;

	// Funcionalidade do aplicativo
	cout << "Qual tipo de aplicacao: " << endl;
	cout << "1 - Soma" << endl;
	cout << "2 - Multiplicação" << endl;
	cout << "3 - Divisão" << endl;
	cin >> op;
	_appFunctionType = op;
	cout << "Aplicação " << _appFunctionType << endl;

	string ipAddr;
	unsigned short port;
	cout << "Digite o IP do super servidor: ";
	cin >> ipAddr;
	cout << "Digite o a porta do super servidor: ";
	cin >> port;

	UDPSocket applicationSocket;
	applicationSocket.Bind(0);

	// Envia o tipo da aplicacao
	//
	// A resposta do super servidor será respondendo se essa aplicação funcionará como um
	// serviço principal ou replica
	//
	int msg = op;
	cout << "Registrando aplicacao no super servidor:" << endl;
	cout << "Enviando pacote de registro..." << endl;
	applicationSocket.SendTo(ipAddr, port, (char*)&msg, sizeof(msg));
	applicationSocket.RecvFrom((char*)&msg, sizeof(msg));
	_appServiceType = msg;

	cout << "Registro confirmado" << endl;
	cout << "Aplicacao de tipo de serviço ";

	switch (_appServiceType)
	{
		case 0:
			cout << "PRINCIPAL" << endl;
			break;

		case 1:
			cout << "REPLICA" << endl;
			break;
	}
	
	cout << "Iniciando aplicação... " << endl;
	while (true)
	{
		_appFunctions(&applicationSocket);
	}	
}
