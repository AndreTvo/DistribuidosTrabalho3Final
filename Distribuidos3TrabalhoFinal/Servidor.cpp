#include "Servidor.h"

void Servidor::_failCheckThread()
{
}

Servidor::Servidor()
{
	_aplicacaoPrincipal = nullptr;
}

Servidor::~Servidor()
{
}

void Servidor::iniciar(int idApplication)
{
	_aplicacaoPrincipal = new Aplicacao(idApplication);
}
