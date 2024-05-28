#pragma once

#include <Date.hpp>
#include <LList.hpp>
#include <UplinkObject.hpp>

class Connection : public UplinkObject
{
	char owner[0x40] = {0};
	LList<char*> vlocations;
	bool traceInProgress = false;
	int traceProgress = 0;
	Date connectionTime;

public:
	~Connection();
	bool Load(FILE* file);
	void Save(FILE* file);
	void Print();
	const char* GetID();
	void SetOwner(const char* value);
};
