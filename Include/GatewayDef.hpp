#pragma once

#include <DArray.hpp>
#include <UplinkObject.hpp>

struct GatewayDefLocation
{
	int x;
	int y;
};

class GatewayDef : UplinkObject
{
	char name[0x80];
	char description[0x100];
	char filename[0x100];
	char thumbnail[0x100];
	int cost;
	int maxCpus;
	int maxMemory;
	int maxUpgrades;
	int maxSecurity;
	int bandwidth;
	int width;
	int height;
	DArray<GatewayDefLocation*> cpuLocations;
	DArray<GatewayDefLocation*> memoryLocations;
	DArray<GatewayDefLocation*> securityLocations;
	int modemX;
	int modemY;
	int powerX;
	int powerY;

public:
	GatewayDef();
	~GatewayDef() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	const char* GetID() override;
};
