#include <Date.hpp>

#include <Eclipse.hpp>
#include <Globals.hpp>
#include <Util.hpp>

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
	Date::SetDate(second, minute, hour, day, month, year);
	lastUpdateTime = 0;
}

bool Date::Load(FILE* file)
{
	if (!FileReadData(&second, 4, 1, file))
		return false;

	if (!FileReadData(&minute, 4, 1, file))
		return false;

	if (!FileReadData(&hour, 4, 1, file))
		return false;

	if (!FileReadData(&day, 4, 1, file))
		return false;

	if (!FileReadData(&month, 4, 1, file))
		return false;

	if (!FileReadData(&year, 4, 1, file))
		return false;

	if (!FileReadData(&active, 1, 1, file))
		return false;

	return true;
}

void Date::Save(FILE* file)
{
	fwrite(&second, 4, 1, file);
	fwrite(&minute, 4, 1, file);
	fwrite(&hour, 4, 1, file);
	fwrite(&day, 4, 1, file);
	fwrite(&month, 4, 1, file);
	fwrite(&year, 4, 1, file);
	fwrite(&active, 1, 1, file);
	SaveID_END(file);
}

void Date::Print()
{
	printf("Date: ");
	printf("%d:%d:%d, %d/%d/%d\n", hour, minute, second, day, month, year);

	if (active)
		puts("Syncronised with real world time");
	else
		puts("Not Syncronised with real world time");
}

void Date::Update()
{
	if (!active)
		return;
	const auto rbx_2 = EclGetAccurateTime() - lastUpdateTime;

	if (game->GameSpeed() == 1)
	{
		if (rbx_2 > 1000)
		{
			AdvanceSecond(1);
			this->lastUpdateTime = EclGetAccurateTime();
		}

		return;
	}

	if (rbx_2 <= 150)
		return;
	Date newDate;
	newDate.SetDate(this);

	switch (game->GameSpeed())
	{
		case 2:
			newDate.AdvanceSecond(3);
			newDate.AdvanceMinute(5);
			break;
		case 3:
			newDate.AdvanceSecond(3);
			newDate.AdvanceMinute(5);
			newDate.AdvanceHour(1);
			break;
		case 4:
			newDate.AdvanceSecond(3);
			newDate.AdvanceMinute(3);
			newDate.AdvanceHour(24);
			break;
		default:
			UplinkAbort("Unrecognised Game Speed");
	}

	const auto nextEventDate = game->GetWorld()->eventScheduler.GetDateOfNextEvent();

	if (nextEventDate != 0 && nextEventDate->Before(&newDate))
	{
		if (!nextEventDate->Before(this))
		{
			nextEventDate->AdvanceSecond(1);
			const auto second = nextEventDate->GetSecond() + 1;
			const auto minute = nextEventDate->GetMinute();
			const auto hour = nextEventDate->GetHour();
			const auto day = nextEventDate->GetDay();
			const auto month = nextEventDate->GetMonth();
			const auto year = nextEventDate->GetYear();
			SetDate(second, minute, hour, day, month, year);
		}
		else
			AdvanceSecond(1);

		game->GetWorld()->ForceNextUpdate();
	}
	else
		SetDate(&newDate);

	this->lastUpdateTime = EclGetAccurateTime();
}

const char* Date::GetID()
{
	return "DATE";
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

void Date::SetDate(Date* other)
{
	UplinkAssert(other != nullptr);

	const auto second = other->GetSecond();
	const auto minute = other->GetMinute();
	const auto hour = other->GetHour();
	const auto day = other->GetDay();
	const auto month = other->GetMonth();
	const auto year = other->GetYear();

	SetDate(second, minute, hour, day, month, year);
}

void Date::AdvanceSecond(int advance)
{
	second += advance;

	if (second >= 60)
	{
		const auto minutes = (second - 60) / 60 + 1;
		second %= 60;
		AdvanceMinute(minutes);
	}

	if (second < 0)
	{
		const auto minutes = (-second - 1) / 60 + 1;
		second += minutes * 60;
		AdvanceMinute(-minutes);
	}
}

void Date::AdvanceMinute(int advance)
{
	minute += advance;

	if (minute >= 60)
	{
		const auto hours = (minute - 60) / 60 + 1;
		minute %= 60;
		AdvanceHour(hours);
	}

	if (minute < 0)
	{
		const auto hours = (-minute - 1) / 60 + 1;
		minute += hours * 60;
		AdvanceHour(-hours);
	}
}

void Date::AdvanceHour(int advance)
{
	hour += advance;

	if (hour >= 24)
	{
		const auto days = (hour - 24) / 24 + 1;
		hour %= 24;
		AdvanceDay(days);
	}

	if (hour < 0)
	{
		const auto days = (-hour - 1) / 24 + 1;
		hour += days * 24;
		AdvanceDay(-days);
	}
}

void Date::AdvanceDay(int advance)
{
	day += advance;

	if (day > 30)
	{
		const auto months = (day - 30) / 30 + 1;
		day %= 30;
		AdvanceMonth(months);
	}

	if (day < 1)
	{
		const auto months = -day / 30 + 1;
		day += months * 30;
		AdvanceMonth(-months);
	}
}

void Date::AdvanceMonth(int advance)
{
	month += advance;

	if (month > 12)
	{
		const auto years = (month - 12) / 12 + 1;
		month %= 12;
		AdvanceYear(years);
	}

	if (month < 1)
	{
		const auto years = -month / 12 + 1;
		month += years * 12;
		AdvanceYear(-years);
	}
}

void Date::AdvanceYear(int advance)
{
	year += advance;
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

bool Date::Before(Date* other)
{
	// Previous year, before.
	if (year < other->year)
		return true;

	// Following year, after.
	if (year > other->year)
		return false;

	// Previous month, before.
	if (month < other->month)
		return true;

	// Following month, after.
	if (month > other->month)
		return false;

	// Previous day, before.
	if (day < other->day)
		return true;

	// Following day, after.
	if (day > other->day)
		return false;

	// Previous hour, before.
	if (hour < other->hour)
		return true;

	// Following hour, after.
	if (hour > other->hour)
		return false;

	// Previous minute, before.
	if (minute < other->minute)
		return true;

	// Following minute, after.
	if (minute > other->minute)
		return false;

	// Previous second, before.
	if (second < other->second)
		return true;

	// Following or same second, after or equal.
	return false;
}

bool Date::After(Date* other)
{
	// Previous year, before.
	if (year < other->year)
		return false;

	// Following year, after.
	if (year > other->year)
		return true;

	// Previous month, before.
	if (month < other->month)
		return false;

	// Following month, after.
	if (month > other->month)
		return true;

	// Previous day, before.
	if (day < other->day)
		return false;

	// Following day, after.
	if (day > other->day)
		return true;

	// Previous hour, before.
	if (hour < other->hour)
		return false;

	// Following hour, after.
	if (hour > other->hour)
		return true;

	// Previous minute, before.
	if (minute < other->minute)
		return false;

	// Following minute, after.
	if (minute > other->minute)
		return true;

	// Following second, after.
	if (second > other->second)
		return true;

	// Previous or same second, before or equal.
	return false;
}

void Date::Activate()
{
	active = true;
}
