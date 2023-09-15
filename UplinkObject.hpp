#pragma once

class UplinkObject
{
	UplinkObject();
	virtual ~UplinkObject();
	virtual bool Load(FILE* file);
	virtual void Save(FILE* file);
	virtual void Print();
	virtual void Update();
	virtual const char* GetID();
	virtual int GetOBJECTID();
	const char* GetID_END();
	void LoadID(FILE* file);
	void LoadID_END(FILE* file);
	void SaveID(FILE* file);
	void SaveID_END(FILE* file);
};
