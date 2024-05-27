#pragma once

#include <UplinkObject.hpp>

class Company : UplinkObject
{
	int sharePrices[12] = {0};
	int sharePriceLastMonth = 0;
	char name[0x40] = " ";
	char boss[0x80] = "Unlisted";
	char admin[0x80] = "Unlisted";
	int size;
	int type;
	int growth;
	int alignment;

public:
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	void SetName(const char* name);
};
