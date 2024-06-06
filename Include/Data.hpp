#pragma once

#include <DArray.hpp>
#include <UplinkObject.hpp>

class Data : public UplinkObject
{
	char _title[0x40] = "Empty";
	int _type = 0;
	int _size = 0;
	int _encrypted = 0;
	int _compressed = 0;
	float _softwareVersion = 1.0f;
	int _softwareType = 0;

public:
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	int GetSize();
	void SetTitle(const char* title);
	void SetDetails(int type, int size, int encrypted, int compressed, float softwareVersion, int softwareType);
};

class DataBank : public UplinkObject
{
	DArray<Data*> _files;
	DArray<int> _placements;
	bool _formatted = false;

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
