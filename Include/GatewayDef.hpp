#pragma once

#include <DArray.hpp>
#include <UplinkObject.hpp>
#include <fstream>

struct GatewayDefLocation
{
	int x = 0;
	int y = 0;
};

class GatewayDef : UplinkObject
{
	char name[0x80] = "UnNamed";
	char description[0x100] = "UnDescribed ;)";
	char filename[0x100] = "None";
	char thumbnail[0x100] = "None";
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
	int modemX = 0;
	int modemY = 0;
	int powerX = 0;
	int powerY = 0;

public:
	~GatewayDef() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	const char* GetID() override;
	void LoadGatewayDefinition(std::ifstream& stream);
	void SetFilename(const char* filename);
	void SetThumbnail(const char* thumbnail);
	bool VerifyCorrectness();
	static int GatewayDefComparator(GatewayDef** a, GatewayDef** b);
};
