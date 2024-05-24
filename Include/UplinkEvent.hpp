#pragma once

#include <UplinkObject.hpp>

struct UplinkEvent : UplinkObject
{
	Date runDate;

	UplinkEvent();
	~UplinkEvent() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	virtual void Run();
	virtual void RunWarning();
	virtual char* GetShortString();
	virtual char* GetLongString();
};
