#pragma once

#include <ComputerScreens/ComputerScreen.hpp>

class LogScreen : public ComputerScreen
{
	int nextPage = -1;
	int target = -1;

public:
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	void SetNextPage(int value);
	void SetTARGET(int value);
};
