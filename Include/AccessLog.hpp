#pragma once

#include <Date.hpp>
#include <UplinkObject.hpp>

class AccessLog : UplinkObject
{
public:
	int type = 0;
	Date date;
	char fromIp[0x18] = " ";
	char fromName[0x80];
	int suspicious = 0;
	char* data1 = nullptr;
	char* data2 = nullptr;
	char* data3 = nullptr;

	~AccessLog() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	void SetTYPE(int value);
	void SetSuspicious(int value);
	void SetFromIP(const char* value);
	void SetData1(const char* value);
	void SetData2(const char* value);
	void SetData3(const char* value);
	void SetProperties(Date& date, const char* ip, const char* fromName, int suspicious, int type);
	void SetProperties(AccessLog* other);
};
