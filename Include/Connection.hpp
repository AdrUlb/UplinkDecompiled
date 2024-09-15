#pragma once

#include <Date.hpp>
#include <LList.hpp>
#include <UplinkObject.hpp>

class Person;

class Connection : public UplinkObject
{
	char _owner[0x40] = {0};
	LList<char*> _vlocations;
	bool _traceInProgress = false;
	int _traceProgress = 0;
	Date _connectionTime;

public:
	~Connection();
	bool Load(FILE* file);
	void Save(FILE* file);
	void Print();
	const char* GetID();
	const char* GetTarget();
	Person* GetOwner();
	LList<char*>& GetVLocations();
	int GetSize();
	void SetOwner(const char* owner);
	const char* GetGhost();
	void Connect();
	void Disconnect();
	void Reset();
	void AddVLocation(const char* ip);
	bool LocationIncluded(const char* ip);
	void BeginTrace();
	bool TraceInProgress();
	bool Traced();
	[[nodiscard]] int GetTraceProgress() const { return _traceProgress; };
};
