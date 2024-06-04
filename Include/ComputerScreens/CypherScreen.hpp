#pragma once

#include <ComputerScreens/ComputerScreen.hpp>

class CypherScreen : public ComputerScreen
{
	int _nextPage = -1;
	int _difficulty = 1;

public:
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	void SetNextPage(int nextPage);
	void SetDifficulty(int difficulty);
};
