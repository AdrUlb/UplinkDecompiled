#pragma once

#include <LList.hpp>
#include <UplinkObject.hpp>

typedef void Record;

class RecordBank : UplinkObject
{
	LList<Record*> records;

public:
	~RecordBank() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
};
