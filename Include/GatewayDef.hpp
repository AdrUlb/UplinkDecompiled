#pragma once

#include <DArray.hpp>
#include <UplinkObject.hpp>
#include <fstream>

struct GatewayDefLocation
{
	int X = 0;
	int Y = 0;
};

class GatewayDef : UplinkObject
{
	char _name[0x80] = "UnNamed";
	char _description[0x100] = "UnDescribed ;)";
	char _filename[0x100] = "None";
	char _thumbnail[0x100] = "None";
	int _cost;
	int _maxCpus;
	int _maxMemory;
	int _maxUpgrades;
	int _maxSecurity;
	int _bandwidth;
	int _width;
	int _height;
	DArray<GatewayDefLocation*> _cpuLocations;
	DArray<GatewayDefLocation*> _memoryLocations;
	DArray<GatewayDefLocation*> _securityLocations;
	int _modemX = 0;
	int _modemY = 0;
	int _powerX = 0;
	int _powerY = 0;

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
