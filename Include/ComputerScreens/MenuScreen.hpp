#pragma once

#include <ComputerScreens/ComputerScreen.hpp>
#include <ComputerScreens/MenuScreenOption.hpp>
#include <LList.hpp>

class MenuScreen : public ComputerScreen
{
	LList<MenuScreenOption*> options;

public:
	~MenuScreen() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	void AddOption(const char* caption, const char* tooltip, int nextPage, int security, int index);
	int NumOptions();
	int GetNextPage(int index);
	int GetSecurity(int index);
	const char* GetTooltip(int index);
	const char* GetCaption(int index);
};
