#include <Gateway.hpp>

Gateway::~Gateway()
{
	DeleteLListData(&_hardware);
	if (_currentGatewayDef != 0)
		delete _currentGatewayDef;

	if (_exchangeGateway != 0)
		delete _exchangeGateway;
}
bool Gateway::Load(FILE* file)
{
	(void)file;
	puts("TODO: implement Gateway::Load()");
	return false;
}

void Gateway::Save(FILE* file)
{
	(void)file;
	puts("TODO: implement Gateway::Save()");
}

void Gateway::Print()
{
	puts("TODO: implement Gateway::Print()");
}

void Gateway::Update()
{
	_dataBank.Update();
}

const char* Gateway::GetID()
{
	return "GATEWAY";
}

bool Gateway::GetNuked()
{
	return _nuked;
}