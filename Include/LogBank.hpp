#pragma once

#include <AccessLog.hpp>
#include <DArray.hpp>
#include <UplinkObject.hpp>

class LogBank : public UplinkObject
{
	DArray<AccessLog*> accessLogs;
	DArray<AccessLog*> accessLogsModified;

public:
	~LogBank() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	void AddLog(AccessLog* value, int index);
	bool LogModified(int index);
};
