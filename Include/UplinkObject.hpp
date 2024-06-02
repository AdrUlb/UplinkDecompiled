#pragma once

#include <cstdio>

enum class UplinkObjectId
{
	Unknown = 0,
	VLocation,
	Person = 4,
	Agent,
	Player,
	Option = 8,
	VlocationSpecial,
	LanComputer = 15,
	Computer = 20,
	Data,
	DataBank,
	AccessLog,
	LogBank,
	Record,
	RecordBank,
	SecuritySystem = 29,
	GenericScreen,
	MessageScreen,
	MenuScreen = 33,
	MenuScreenOption,
	DialogScreen,
	DialogScreenWidget,
	UserIDScreen,
	LinksScreen = 40,
	HighSecurityScreen = 43,
	DisconnectedScreen,
	Company = 50,
	CompanyUplink = 52,
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
};
