#include "Cliente.h"

Cliente::Cliente()
{
	_clientSocket.Bind(0);
	_clientSocket.SetRecvTimeout(5000);
}

Cliente::~Cliente()
{
}

void Cliente::iniciar()
{
	// Conectar super-servidor
	// Perguntar por aplicativo a escolher
	// Recebe sockaddr
	//
	// Loop {
	// Envia mensagem para o aplicativo com as entradas
	// Recebe resposta e exibe
	// Se Timeout == Pede outro servidor do mesmo tipo ao super servidor
	// Continua loop
	// }

	string ipAddr;
	unsigned short port;

	cout << "Digite o IP do super servidor: ";
	cin >> ipAddr;
	cout << "Digite a porta do super servidor: ";
	cin >> port;

	int op = 0;

	while (true)
	{
		cout << "Qual tipo de aplicacao deseja se comunicar: " << endl;
		cout << "1 - Soma" << endl;
		cout << "2 - Multiplicacao" << endl;
		cout << "3 - Divisao" << endl;
		cout << "0 - Sair" << endl;
		cin >> op;

		if (op == 0)
			break;

		UDPSSResponse serverResponse;

		_clientSocket.SendTo(ipAddr, port, (char*)& op, sizeof(op));
		_clientSocket.RecvFrom((char*)& serverResponse, sizeof(serverResponse));

		UDPPacket packet;

		// Pacote valido
		if (serverResponse.check >= 0)
		{
			if (serverResponse.check == 1)
			{
				cout << "Não há servidores disponiveis para a aplicação escolhida" << endl;
			}
			else
			{
				_applicationAddr = serverResponse.applicationAddress;

				cout << "Bem vindo a aplicação de ";
				switch (op)
				{
					case 1:
						cout << "soma" << endl;
						break;

					case 2:
						cout << "multiplicação" << endl;
						break;

					case 3:
						cout << "divisão" << endl;
						break;
				}

				while (true)
				{
					cout << "Entre com o valor de A: " << endl;
					cin >> packet.a;
					cout << "Entre com o valor de B: " << endl;
					cin >> packet.b;

					// Cliente está enviando a mensagem
					packet.check = 0;
					packet.sourceMessage = 1;
					packet.response = 0;

					_clientSocket.SendTo(_applicationAddr, (char*)& packet, sizeof(packet));
					_clientSocket.RecvFrom((char*)& packet, sizeof(packet));

					// Pacote valido
					if (packet.check == 0)
					{
						cout << "\nA resposta enviada pelo servidor é: " << packet.response << "\n\n";						
					}
					else
					{
						cout << "O servidor não respondeu a mensagem" << endl;
						break;
					}

					int continueOp = 0;

					cout << "Continuar aplicacao ou fechar: " << endl;
					cout << "0 - Continuar" << endl;
					cout << "1 - Fechar" << endl;
					cin >> continueOp;

					if (continueOp > 0)
						break;
				}
			}
		}
		else
		{
			// Superservidor offline
			cout << "Superservidor não respondeu ou está offline" << endl;
		}
	}
}
