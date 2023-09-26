#pragma once

#include <cstdio>

class UplinkObject
{
public:
	UplinkObject();
	virtual ~UplinkObject();
	virtual bool Load(FILE* file);
	virtual void Save(FILE* file);
private:
	virtual void Print();
	virtual void Update();
	virtual const char* GetID();
public:
	virtual int GetOBJECTID();
protected:
	char* GetID_END();
	void LoadID(FILE* file);
	void LoadID_END(FILE* file);
	void SaveID(FILE* file);
	void SaveID_END(FILE* file);
};
