#pragma once

#include <ComputerScreens/ComputerScreen.hpp>

class LogScreen : public ComputerScreen
{
	int _nextPage = -1;
	int _target = -1;

public:
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	void SetNextPage(int nextPage);
	void SetTARGET(int type);
};
