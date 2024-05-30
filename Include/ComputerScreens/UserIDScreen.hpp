#pragma once

#include <ComputerScreens/ComputerScreen.hpp>

class UserIDScreen : public ComputerScreen
{
	int nextPage = -1;
	int difficulty = 1;

public:
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	void SetDifficulty(int value);
	void SetNextPage(int value);
};
