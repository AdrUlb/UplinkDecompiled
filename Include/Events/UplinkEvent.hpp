#pragma once

#include <Date.hpp>
#include <UplinkObject.hpp>

class UplinkEvent : public UplinkObject
{
	Date _runDate;

public:
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	Date& GetRunDate();
	virtual void Run();
	virtual void RunWarning();
	virtual char* GetShortString();
	virtual char* GetLongString();
	void SetRunDate(Date* date);
};
