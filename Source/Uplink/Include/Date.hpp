#pragma once

#include <UplinkObject.hpp>

class Date : UplinkObject
{
public:
	Date();
	Date(int second, int minute, int hour, int day, int month, int year);
	virtual ~Date();
private:
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;

	int GetSecond();
	int GetMinute();
	int GetHour();
	int GetDay();
	int GetMonth();
	int GetYear();

	void SetDate(int second, int minute, int hour, int day, int month, int year);
	void SetDate(Date* date);

	void AdvanceSecond(int count);
	void AdvanceMinute(int count);
	void AdvanceHour(int count);
	void AdvanceDay(int count);
	void AdvanceMonth(int count);
	void AdvanceYear(int count);

	int second;
	int minute;
	int hour;
	int day;
	int month;
	int year;
	int lastUpdateTime;
	bool active;
};
