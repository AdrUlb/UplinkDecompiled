#pragma once

#include <Date.hpp>
#include <UplinkObject.hpp>

class AccessLog : UplinkObject
{
	int type;
	Date date;
	char fromIp[0x18];
	char fromName[0x80];
	int suspicious;
	char* data1;
	char* data2;
	char* data3;

public:
	AccessLog();
	~AccessLog() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
};
