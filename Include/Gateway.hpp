#pragma once

#include <Data.hpp>
#include <LList.hpp>
#include <UplinkObject.hpp>

class Gateway : UplinkObject
{
	char upgrades;
	DataBank dataBank;
	GatewayDef* currentGatewayDef;
	int id;
	GatewayDef* exchangeGateway;
	bool nuked;
	int proximity;
	char modem[0x40];
	int modemSpeed;
	int memorySize;
	LList<char*> hardware;

public:
	Gateway();
	~Gateway() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
};
