#include <Date.hpp>

#include <Util.hpp>
#include "../UplinkDecompiledTempDefs.hpp"

Date::Date()
{
	second = 1;
	minute = 1;
	hour = 1;
	day = 1;
	month = 1;
	year = 1000;
	lastUpdateTime = 0;
	active = false;
}

Date::Date(int second, int minute, int hour, int day, int month, int year)
{
	SetDate(second, minute, hour, day, month, year);
	lastUpdateTime = 0;
}

Date::~Date()
{

}

void Date::Update()
{
	Date_Update(this);
}

const char* Date::GetID()
{
	return "DATE";
}

int Date::GetSecond()
{
	return second;
}

int Date::GetMinute()
{
	return minute;
}

int Date::GetHour()
{
	return hour;
}

int Date::GetDay()
{
	return day;
}

int Date::GetMonth()
{
	return month;
}

int Date::GetYear()
{
	return year;
}

void Date::SetDate(int second, int minute, int hour, int day, int month, int year)
{
	this->second = second;
	this->minute = minute;
	this->hour = hour;
	this->day = day;
	this->month = month;
	this->year = year;
	AdvanceSecond(0);
}

void Date::SetDate(Date* date)
{
	UplinkAssert(date);
	SetDate(date->GetSecond(), date->GetMinute(), date->GetHour(), date->GetDay(), date->GetMonth(), date->GetYear());
}

void Date::AdvanceSecond(int count)
{
	auto newSecond = second + count;

	if (newSecond >= 60)
	{
		const auto minutes = newSecond / 60;
		newSecond %= 60;
		AdvanceMinute(minutes);
	}
	else if (newSecond < 0)
	{
		const auto minutes = newSecond / 60 - 1;
		newSecond = 60 - (-newSecond % 60);
		AdvanceMinute(minutes);
	}

	second = newSecond;
}

void Date::AdvanceMinute(int count)
{
	auto newMinute = minute + count;

	if (newMinute >= 60)
	{
		const auto hours = newMinute / 60;
		newMinute %= 60;
		AdvanceHour(hours);
	}
	else if (newMinute < 0)
	{
		const auto hours = newMinute / 60 - 1;
		newMinute = 60 - (-newMinute % 60);
		AdvanceHour(hours);
	}

	minute = newMinute;
}

void Date::AdvanceHour(int count)
{
	auto newHour = hour + count;

	if (newHour >= 24)
	{
		const auto days = newHour / 24;
		newHour %= 24;
		AdvanceDay(days);
	}
	else if (newHour < 0)
	{
		const auto days = newHour / 24 - 1;
		newHour = 24 - (-newHour % 24);
		AdvanceDay(days);
	}

	hour = newHour;
}

void Date::AdvanceDay(int count)
{
	auto newDay = day - 1 + count;

	if (newDay >= 30)
	{
		const auto months = newDay / 30;
		newDay %= 30;
		AdvanceMonth(months);
	}
	else if (newDay < 0)
	{
		const auto months = newDay / 30 - 1;
		newDay = 30 - (-newDay % 30);
		AdvanceMonth(months);
	}

	day = newDay + 1;
}

void Date::AdvanceMonth(int count)
{
	auto newMonth = month - 1 + count;

	if (newMonth >= 12)
	{
		const auto years = newMonth / 12;
		newMonth %= 12;
		AdvanceYear(years);
	}
	else if (newMonth < 0)
	{
		const auto years = newMonth / 12 - 1;
		newMonth = 12 - (-newMonth % 12);
		AdvanceYear(years);
	}

	month = newMonth + 1;
}

void Date::AdvanceYear(int count)
{
	year += count;
	return;
}
