#pragma once

#include <Computer.hpp>
#include <DArray.hpp>

typedef void LanComputerSystem;
typedef void LanComputerLink;

class LanComputer : public Computer
{
	DArray<LanComputerSystem*> systems;
	DArray<LanComputerLink*> links;

	~LanComputer() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
};
