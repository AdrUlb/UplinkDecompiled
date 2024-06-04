#pragma once

#include <ComputerScreens/ComputerScreen.hpp>

class DisconnectedScreen : public ComputerScreen
{
	int _nextPage = -1;
	char* _textMessage = nullptr;

public:
	~DisconnectedScreen() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	void SetTextMessage(const char* textMessage);
	void SetNextPage(int value);
	static void AddLoginLost(const char* ip);
};
