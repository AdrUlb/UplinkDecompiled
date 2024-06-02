#pragma once

#include <Data.hpp>
#include <GatewayDef.hpp>
#include <LList.hpp>
#include <UplinkObject.hpp>

class Gateway : UplinkObject
{
	char _upgrades = 0;
	DataBank _dataBank;
	GatewayDef* _currentGatewayDef = nullptr;
	int _id = 0;
	GatewayDef* _exchangeGateway = nullptr;
	bool _nuked = false;
	int _proximity = 0;
	char _modem[0x40] = " ";
	int _modemSpeed = 0;
	int _memorySize = 0;
	LList<char*> _hardware;

public:
	~Gateway() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
	bool GetNuked();
};
