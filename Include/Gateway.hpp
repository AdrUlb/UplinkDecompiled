#pragma once

#include <Data.hpp>
#include <GatewayDef.hpp>
#include <LList.hpp>
#include <UplinkObject.hpp>

class Gateway : UplinkObject
{
public:
	char upgrades = 0;
	DataBank dataBank;
	GatewayDef* currentGatewayDef = nullptr;
	int id = 0;
	GatewayDef* exchangeGateway = nullptr;
	bool nuked = false;
	int proximity = 0;
	char modem[0x40] = " ";
	int modemSpeed = 0;
	int memorySize = 0;
	LList<char*> hardware;

	~Gateway() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
};
