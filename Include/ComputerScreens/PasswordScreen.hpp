#pragma once

#include <ComputerScreens/ComputerScreen.hpp>

class PasswordScreen : public ComputerScreen
{
	int _nextPage = -1;
	int _difficulty = 1;
	char _password[0x40];

public:
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	int GetNextPage();
	const char* GetPassword();
	void SetNextPage(int nextPage);
	void SetDifficulty(int difficulty);
	void SetPassword(const char* password);
};
