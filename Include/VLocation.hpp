#pragma once

#include <Computer.hpp>
#include <UplinkObject.hpp>

static constexpr int VirtualWidth = 594;
static constexpr int VirtualHeight = 315;

class VLocation : public UplinkObject
{
	char _ip[0x18] = "0.0.0.0";
	char _computerName[0x40];
	int _x;
	int _y;
	bool _listed = true;
	bool _displayed = true;
	bool _colored = false;

public:
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	const char* GetIp();
	const char* GetComputerName();
	int GetX();
	int GetY();
	bool GetListed();
	bool GetDisplayed();
	bool GetColored();
	void SetIp(const char* ip);
	void SetPLocation(int x, int y);
	void SetComputerName(const char* computer);
	void SetListed(bool listed);
	void SetDisplayed(bool displayed);
	void SetColorDisplayed(bool displayed);
	Computer* GetComputer();
};

class VLocationSpecial : public VLocation
{
	int _screenIndex = 0;
	int _securitySystemIndex = -1;

public:
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	int GetScreenIndex();
	int GetSecuritySystemIndex();
};