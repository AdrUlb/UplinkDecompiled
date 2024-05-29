#pragma once

#include <ComputerScreens/ComputerScreen.hpp>

class MessageScreen : public ComputerScreen
{
public:
	int nextPage = -1;
	char* textMessage = nullptr;
	char* buttonMessage = nullptr;
	bool mailThisToMe = false;

	~MessageScreen() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	void SetTextMessage(const char* value);
	void SetButtonMessage(const char* value);
	void SetNextPage(int value);
};
