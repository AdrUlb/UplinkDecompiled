#pragma once

#include <ComputerScreens/ComputerScreen.hpp>

class DisconnectedScreen : public ComputerScreen
{
	int _nextPage = -1;
	char* _textMessage = nullptr;

public:
	static DArray<char*> loginslost;

	~DisconnectedScreen() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	[[nodiscard]] const char* GetTextMessage() const
	{
		return _textMessage;
	}
	void SetTextMessage(const char* textMessage);
	[[nodiscard]] int GetNextPage() const
	{
		return _nextPage;
	}
	void SetNextPage(int value);
	static void AddLoginLost(const char* ip);
	static void ClearLoginsLost();
};
