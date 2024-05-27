#pragma once

#include <cstdio>

enum class UplinkObjectId : int
{
	Unknown = 0,
	Person = 4,
	Agent,
	Player,
	Option = 8,
	DataBank = 22
};

class UplinkObject
{
public:
	virtual ~UplinkObject();
	virtual bool Load(FILE* file);
	virtual void Save(FILE* file);
	virtual void Print();
	virtual void Update();
	virtual const char* GetID();
	virtual UplinkObjectId GetOBJECTID();

	char* GetID_END();
	void LoadID(FILE* file);
	void LoadID_END(FILE* file);
	void SaveID(FILE* file);
	void SaveID_END(FILE* file);
};
