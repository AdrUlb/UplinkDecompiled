#pragma once

#include <ComputerScreens/ComputerScreen.hpp>
#include <ComputerScreens/MenuScreenOption.hpp>
#include <LList.hpp>

class HighSecurityScreen : ComputerScreen
{
	LList<MenuScreenOption*> options;
	int nextPage;

	~HighSecurityScreen() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
};
