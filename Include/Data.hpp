#pragma once

#include <DArray.hpp>
#include <UplinkObject.hpp>

class Data : UplinkObject
{
	char title[0x40];
	int type;
	int size;
	int encrypted;
	int compressed;
	float softwareVersion;
	int softwareType;

public:
	Data();
	~Data() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
};

class DataBank : public UplinkObject
{
	DArray<Data*> files;
	DArray<int> placements;
	bool formatted = false;

public:
	~DataBank() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
};
