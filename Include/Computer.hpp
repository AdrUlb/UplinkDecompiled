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
	int _recentHacks = 0;
	int _recentHacksThisMonth = 0;
	int _recentHacksLastMonth = 0;
	int _type = 0;
	char _name[0x40] = {0};
	char _companyName[0x40] = {0};
	char _ip[0x18] = {0};
	int _traceSpeed = 0;
	int _traceAction = 0;
	char _isTargetable = true;
	char _isExternallyOpen = true;
	char _running = true;
	float _revelationInfectedVersion = 0.0f;
	Date _revelationInfectedDate;
	DArray<ComputerScreen*> _screens;
	DataBank _dataBank;
	LogBank _logBank;
	RecordBank _recordBank;
	Security _security;

public:
	~Computer() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	int GetType();
	const char* GetName();
	const char* GetIp();
	int GetTraceAction();
	DataBank& GetDataBank();
	LogBank& GetLogBank();
	RecordBank& GetRecordBank();
	Security& GetSecurity();
	void SetTYPE(int type);
	void SetName(const char* name);
	void SetCompanyName(const char* companyName);
	void SetIP(const char* ip);
	void SetTraceSpeed(int traceSpeed);
	void SetTraceAction(int traceAction);
	void SetIsTargetable(bool isTargetable);
	void SetIsExternallyOpen(bool isExternallyOpen);
	void AddComputerScreen(ComputerScreen* screen, int index);
	ComputerScreen* GetComputerScreen(int index);
};
