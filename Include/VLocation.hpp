#pragma once

#include <UplinkObject.hpp>

class VLocation : UplinkObject
{
	char ip[0x18] = "0.0.0.0";
	char computer[0x40];
	int x;
	int y;
	bool listed = true;
	bool displayed = true;
	bool colored = false;

public:
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	void SetIP(const char* value);
	void SetPLocation(int x, int y);
	void SetComputer(const char* computerName);
	void SetListed(bool value);
};
