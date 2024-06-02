#pragma once

#include <Button.hpp>
#include <ComputerScreens/ComputerScreen.hpp>

class LinksScreen : public ComputerScreen
{
	int _nextPage = -1;
	int _type = 0;

public:
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	int GetNextPage();
	int GetScreenType();
	void SetNextPage(int nextPage);
	void SetScreenType(int type);
};
