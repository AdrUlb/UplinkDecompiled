#pragma once

#include <UplinkObject.hpp>

class Date : UplinkObject
{
	int second;
	int minute;
	int hour;
	int day;
	int month;
	int year;
	int lastUpdateTime;
	bool active;

public:
	Date();
	Date(int second, int minute, int hour, int day, int month, int year);
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;

	void SetDate(int second, int minute, int hour, int day, int month, int year);
	void SetDate(Date* other);
	void AdvanceSecond(int advance);
	void AdvanceMinute(int advance);
	void AdvanceHour(int advance);
	void AdvanceDay(int advance);
	void AdvanceMonth(int advance);
	void AdvanceYear(int advance);
	int GetSecond();
	int GetMinute();
	int GetHour();
	int GetDay();
	int GetMonth();
	int GetYear();
	bool Before(Date* other);
};
