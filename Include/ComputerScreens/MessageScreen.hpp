#pragma once

#include <ComputerScreens/ComputerScreen.hpp>

class MessageScreen : public ComputerScreen
{
	int _nextPage = -1;
	char* _textMessage = nullptr;
	char* _buttonMessage = nullptr;
	bool _mailThisToMe = false;

public:
	~MessageScreen() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	int GetNextPage();
	const char* GetTextMessage();
	const char* GetButtonMessage();
	bool GetMailThisToMe();
	void SetTextMessage(const char* textMessage);
	void SetButtonMessage(const char* buttonMessage);
	void SetNextPage(int nextPage);
};
