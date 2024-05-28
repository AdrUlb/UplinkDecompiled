#pragma once

#include <cstdio>

enum class UplinkObjectId
{
	Unknown = 0,
	VLocation = 1,
	Person = 4,
	Agent,
	Player,
	Option = 8,
	Computer = 20,
	DataBank = 22,
	RecordBank = 26,
	MessageScreen = 31,
	DialogScreen = 35,
	LinksScreen = 40,
	DisconnectedScreen = 44,
	Company = 50,
	NotificationEvent = 70
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
