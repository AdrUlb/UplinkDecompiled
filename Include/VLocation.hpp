#pragma once

#include <UplinkObject.hpp>

class VLocation : UplinkObject
{
	char ip[0x18] = "0.0.0.0";
	char computer[0x40];
	int32_t x;
	int32_t y;
	bool listed = true;
	bool displayed = true;
	bool colored = false;

public:
	~VLocation() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
};
