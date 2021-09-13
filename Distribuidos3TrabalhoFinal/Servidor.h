#pragma once

#include "Aplicacao.h"

#include <vector>
using namespace std;

// Cada maquina é um servidor
// Um servidor cuida de gerenciar replicas de uma aplicação
class Servidor
{
private:
	int _id;

	void _failCheckThread();
	
	Aplicacao* _aplicacaoPrincipal;	
	vector<Aplicacao*> _aplicacaoReplicas;

public:
	Servidor();
	~Servidor();

	void iniciar(int idApplication);

};