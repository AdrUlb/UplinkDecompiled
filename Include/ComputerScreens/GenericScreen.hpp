#pragma once

#include <ComputerScreens/ComputerScreen.hpp>

class GenericScreen : public ComputerScreen
{
	int nextPage = -1;
	int type = 0;

public:
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	void SetScreenType(int value);
};
