#pragma once

#include <ComputerScreens/ComputerScreen.hpp>

class DisconnectedScreen : public ComputerScreen
{
	int nextPage = -1;
	char* textMessage = nullptr;

public:
	~DisconnectedScreen() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	void SetTextMessage(const char* value);
	void SetNextPage(int value);
};
