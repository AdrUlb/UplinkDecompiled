#pragma once

#include <ComputerScreens/ComputerScreen.hpp>

class LinksScreen : public ComputerScreen
{
	int nextPage = -1;
	int type = 0;

public:
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	void SetNextPage(int value);
	void SetScreenType(int value);
};
