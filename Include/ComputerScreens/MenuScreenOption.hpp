#pragma once

#include <UplinkObject.hpp>

class MenuScreenOption : UplinkObject
{
	char _caption[0x40] = {0};
	char _tooltip[0x80] = {0};
	int _nextPage = -1;
	int _security = 10;

public:
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	const char* GetCaption();
	const char* GetTooltip();
	int GetNextPage();
	int GetSecurity();
	void SetCaption(const char* caption);
	void SetTooltip(const char* tooltip);
	void SetNextPage(int nextPage);
	void SetSecurity(int security);
};
