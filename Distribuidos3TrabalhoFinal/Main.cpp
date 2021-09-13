#include <iostream>
#include "Network.h"

#include "SuperServidor.h"
#include "Aplicacao.h"
#include "Cliente.h"

using namespace std;

int main()
{
	WSASession wsaSession;
	
	char nome[20];
	cin >> nome;
	cout << nome << endl;
	cout << "teste acentuação" << endl;

	int op;
	cout << "Executar como: " << endl;
	cout << "1 - Super Servidor" << endl;
	cout << "2 - Aplicação" << endl;
	cout << "3 - Cliente" << endl;
	cout << "0 - Sair" << endl;
	cin >> op;

	switch (op)
	{
		case 1:
		{
			Superservidor ss;
			ss.iniciar();
			break;
		}

		case 2:
		{
			Aplicacao app;
			app.iniciar();
			break;
		}

		case 3:
		{
			Cliente cliente;
			cliente.iniciar();
			break;
		}
	}



	return 0;
}