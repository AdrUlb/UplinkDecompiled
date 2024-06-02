#pragma once

#include <AccessLog.hpp>
#include <DArray.hpp>
#include <UplinkObject.hpp>

class LogBank : public UplinkObject
{
	DArray<AccessLog*> _accessLogs;
	DArray<AccessLog*> _accessLogsModified;

public:
	~LogBank() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	DArray<AccessLog*>& GetAccessLogs();
	DArray<AccessLog*>& GetAccessLogsModified();
	void AddLog(AccessLog* log, int index);
	bool LogModified(int index);
};
