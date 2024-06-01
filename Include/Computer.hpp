#pragma once

#include <DArray.hpp>
#include <Data.hpp>
#include <Date.hpp>
#include <LogBank.hpp>
#include <Record.hpp>
#include <Security.hpp>
#include <UplinkObject.hpp>

class ComputerScreen;

class Computer : public UplinkObject
{
public:
	int recentHacks = 0;
	int recentHacksThisMonth = 0;
	int recentHacksLastMonth = 0;
	int type = 0;
	char name[0x40] = {0};
	char companyName[0x40] = {0};
	char ip[0x18] = {0};
	int traceSpeed = 0;
	int traceAction = 0;
	char targetable = true;
	char externallyOpen = true;
	char running = true;
	float revelationInfectedVersion = 0.0f;
	Date revelationInfectedDate;
	DArray<ComputerScreen*> screens;
	DataBank dataBank;
	LogBank logBank;
	RecordBank recordBank;
	Security security;

	~Computer() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	void SetName(const char* value);
	void SetCompanyName(const char* value);
	void SetIP(const char* value);
	void SetIsTargetable(bool value);
	void AddComputerScreen(ComputerScreen* screen, int index);
	void SetTYPE(int value);
	void SetTraceSpeed(int value);
	void SetIsExternallyOpen(bool value);
	void SetTraceAction(int value);
	ComputerScreen* GetComputerScreen(int index);
};
