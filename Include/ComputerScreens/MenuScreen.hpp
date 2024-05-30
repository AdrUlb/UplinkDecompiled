#pragma once

#include <ComputerScreens/ComputerScreen.hpp>
#include <LList.hpp>

class MenuScreenOption : UplinkObject
{
	char caption[0x40] = {0};
	char tooltip[0x80] = {0};
	int nextPage = -1;
	int security = 10;

public:
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	void SetCaption(const char* value);
	void SetTooltip(const char* value);
	void SetNextPage(int value);
	void SetSecurity(int value);
};

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
};
