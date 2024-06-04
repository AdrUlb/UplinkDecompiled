#include <Gateway.hpp>

Gateway::~Gateway()
{
	DeleteLListData(&_hardware);
	if (_currentGatewayDef != 0)
		delete _currentGatewayDef;

	if (_exchangeGatewayDef != 0)
		delete _exchangeGatewayDef;
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

GatewayDef* Gateway::GetExchangeGatewayDef()
{
	return _exchangeGatewayDef;
}

bool Gateway::GetNuked()
{
	return _nuked;
}

void Gateway::SetNuked(bool nuked)
{
	_nuked = nuked;
}

void Gateway::ExchangeGatewayComplete()
{
	puts("TODO implement Gateway::ExchangeGatewayComplete()");
}