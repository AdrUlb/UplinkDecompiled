#pragma once

#include <Computer.hpp>
#include <DArray.hpp>

struct LanComputerSystem : UplinkObject
{
	int Type;
	int X;
	int Y;
	int Visible;
	int Subnet;
	int Security;
	int ScreenIndex;
	int DataScreenIndex;
	int Data1;
	int Data2;
	int Data3;
	DArray<int> Spoofs;
};

typedef void LanComputerLink;

class LanComputer : public Computer
{
	DArray<LanComputerSystem*> _systems;
	DArray<LanComputerLink*> _links;

public:
	~LanComputer() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	DArray<LanComputerSystem*>& GetSystems();
};
