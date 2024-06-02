#pragma once

#include <Date.hpp>
#include <UplinkObject.hpp>

class AccessLog : UplinkObject
{
	int _type = 0;
	Date _date;
	char _fromIp[0x18] = " ";
	char _fromName[0x80];
	int _suspicious = 0;
	char* _data1 = nullptr;
	char* _data2 = nullptr;
	char* _data3 = nullptr;

public:
	~AccessLog() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	int GetTYPE();
	Date& GetDate();
	const char* GetFromIp();
	const char* GetFromName();
	int GetSuspicious();
	void SetTYPE(int value);
	void SetSuspicious(int value);
	void SetFromIP(const char* value);
	void SetData1(const char* value);
	void SetData2(const char* value);
	void SetData3(const char* value);
	void SetProperties(Date& date, const char* ip, const char* fromName, int suspicious, int type);
	void SetProperties(AccessLog* other);
};
