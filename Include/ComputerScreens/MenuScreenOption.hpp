#pragma once

#include <UplinkObject.hpp>

class MenuScreenOption : UplinkObject
{
public:
	char caption[0x40] = {0};
	char tooltip[0x80] = {0};
	int nextPage = -1;
	int security = 10;

	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	void SetCaption(const char* value);
	void SetTooltip(const char* value);
	void SetNextPage(int value);
	void SetSecurity(int value);
};
