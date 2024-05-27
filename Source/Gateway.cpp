#include <Gateway.hpp>

Gateway::~Gateway()
{
	DeleteLListData(&hardware);
	if (currentGatewayDef != 0)
		delete currentGatewayDef;

	if (exchangeGateway != 0)
		delete exchangeGateway;
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
	dataBank.Update();
}

const char* Gateway::GetID()
{
	return "GATEWAY";
}
