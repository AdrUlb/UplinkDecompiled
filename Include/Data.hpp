#pragma once

#include <DArray.hpp>
#include <UplinkObject.hpp>

class Data : UplinkObject
{
public:
	char title[0x40] = "Empty";
	int type = 0;
	int size = 0;
	int encrypted = 0;
	int compressed = 0;
	float softwareVersion = 1.0f;
	int softwareType = 0;

	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	void SetTitle(const char* value);
	void SetDetails(int type, int size, int encrypted, int compressed, float softwareVersion, int softwareType);
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
	void SetSize(int value);
	void PutData(Data* value, int index);
	bool PutData(Data* value);
	int FindValidPlacement(Data* value);
	int IsValidPlacement(Data* value, int placement);
};
