#pragma once

#include <ComputerScreens/ComputerScreen.hpp>
#include <ComputerScreens/MenuScreenOption.hpp>
#include <LList.hpp>

class HighSecurityScreen : public ComputerScreen
{
	LList<MenuScreenOption*> options;
	int nextPage = -1;

public:
	~HighSecurityScreen() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	void SetNextPage(int nextPage);
	void AddSystem(const char* caption, int nextPage);
};
