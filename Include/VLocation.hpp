#pragma once

#include <Computer.hpp>
#include <UplinkObject.hpp>

class VLocation : UplinkObject
{
	char _ip[0x18] = "0.0.0.0";
	char _computer[0x40];
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
	int GetX();
	int GetY();
	void SetIp(const char* ip);
	void SetPLocation(int x, int y);
	void SetComputer(const char* computer);
	void SetListed(bool listed);
	Computer* GetComputer();
};
