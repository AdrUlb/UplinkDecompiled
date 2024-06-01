#pragma once

#include <LList.hpp>
#include <UplinkObject.hpp>

class Record : UplinkObject
{
	BTree<char*> fields;

public:
	~Record() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	void AddField(const char* name, const char* value);
};

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
	void AddRecord(Record& record);
	Record* GetRecord(char* search);
	char* MakeSafeField(const char* name);
	Record* GetRecordFromName(const char* name);
};
