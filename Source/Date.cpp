#include <Date.hpp>

#include <Eclipse.hpp>
#include <Globals.hpp>
#include <Util.hpp>

static char _tempdate[0x40];
static char _monthname[13][10] = {"",	  "January", "February",  "March",	 "April",	 "May",		"June",
								  "July", "August",	 "September", "October", "November", "December"};

Date::Date()
{
	_second = 1;
	_minute = 1;
	_hour = 1;
	_day = 1;
	_month = 1;
	_year = 1000;
	_lastUpdateTime = 0;
	_active = false;
}

Date::Date(int second, int minute, int hour, int day, int month, int year)
{
	Date::SetDate(second, minute, hour, day, month, year);
	_lastUpdateTime = 0;
}

bool Date::Load(FILE* file)
{
	if (!FileReadData(&_second, 4, 1, file))
		return false;

	if (!FileReadData(&_minute, 4, 1, file))
		return false;

	if (!FileReadData(&_hour, 4, 1, file))
		return false;

	if (!FileReadData(&_day, 4, 1, file))
		return false;

	if (!FileReadData(&_month, 4, 1, file))
		return false;

	if (!FileReadData(&_year, 4, 1, file))
		return false;

	if (!FileReadData(&_active, 1, 1, file))
		return false;

	return true;
}

void Date::Save(FILE* file)
{
	fwrite(&_second, 4, 1, file);
	fwrite(&_minute, 4, 1, file);
	fwrite(&_hour, 4, 1, file);
	fwrite(&_day, 4, 1, file);
	fwrite(&_month, 4, 1, file);
	fwrite(&_year, 4, 1, file);
	fwrite(&_active, 1, 1, file);
}

void Date::Print()
{
	printf("Date: ");
	printf("%d:%d:%d, %d/%d/%d\n", _hour, _minute, _second, _day, _month, _year);

	if (_active)
		puts("Syncronised with real world time");
	else
		puts("Not Syncronised with real world time");
}

void Date::Update()
{
	if (!_active)
		return;
	const auto rbx_2 = EclGetAccurateTime() - _lastUpdateTime;

	if (game->GetGameSpeed() == 1)
	{
		if (rbx_2 > 1000)
		{
			AdvanceSecond(1);
			this->_lastUpdateTime = EclGetAccurateTime();
		}

		return;
	}

	if (rbx_2 <= 150)
		return;
	Date newDate;
	newDate.SetDate(this);

	switch (game->GetGameSpeed())
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

	const auto nextEventDate = game->GetWorld().GetEventScheduler().GetDateOfNextEvent();

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

		game->GetWorld().ForceNextUpdate();
	}
	else
		SetDate(&newDate);

	this->_lastUpdateTime = EclGetAccurateTime();
}

const char* Date::GetID()
{
	return "DATE";
}

void Date::SetDate(int second, int minute, int hour, int day, int month, int year)
{
	this->_second = second;
	this->_minute = minute;
	this->_hour = hour;
	this->_day = day;
	this->_month = month;
	this->_year = year;
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
	_second += advance;

	if (_second >= 60)
	{
		const auto minutes = (_second - 60) / 60 + 1;
		_second %= 60;
		AdvanceMinute(minutes);
	}

	if (_second < 0)
	{
		const auto minutes = (-_second - 1) / 60 + 1;
		_second += minutes * 60;
		AdvanceMinute(-minutes);
	}
}

void Date::AdvanceMinute(int advance)
{
	_minute += advance;

	if (_minute >= 60)
	{
		const auto hours = (_minute - 60) / 60 + 1;
		_minute %= 60;
		AdvanceHour(hours);
	}

	if (_minute < 0)
	{
		const auto hours = (-_minute - 1) / 60 + 1;
		_minute += hours * 60;
		AdvanceHour(-hours);
	}
}

void Date::AdvanceHour(int advance)
{
	_hour += advance;

	if (_hour >= 24)
	{
		const auto days = (_hour - 24) / 24 + 1;
		_hour %= 24;
		AdvanceDay(days);
	}

	if (_hour < 0)
	{
		const auto days = (-_hour - 1) / 24 + 1;
		_hour += days * 24;
		AdvanceDay(-days);
	}
}

void Date::AdvanceDay(int advance)
{
	_day += advance;

	if (_day > 30)
	{
		const auto months = (_day - 30) / 30 + 1;
		_day %= 30;
		AdvanceMonth(months);
	}

	if (_day < 1)
	{
		const auto months = -_day / 30 + 1;
		_day += months * 30;
		AdvanceMonth(-months);
	}
}

void Date::AdvanceMonth(int advance)
{
	_month += advance;

	if (_month > 12)
	{
		const auto years = (_month - 12) / 12 + 1;
		_month %= 12;
		AdvanceYear(years);
	}

	if (_month < 1)
	{
		const auto years = -_month / 12 + 1;
		_month += years * 12;
		AdvanceYear(-years);
	}
}

void Date::AdvanceYear(int advance)
{
	_year += advance;
}

int Date::GetSecond()
{
	return _second;
}

int Date::GetMinute()
{
	return _minute;
}

int Date::GetHour()
{
	return _hour;
}

int Date::GetDay()
{
	return _day;
}

int Date::GetMonth()
{
	return _month;
}

int Date::GetYear()
{
	return _year;
}

bool Date::Before(Date* other)
{
	// Previous year, before.
	if (_year < other->_year)
		return true;

	// Following year, after.
	if (_year > other->_year)
		return false;

	// Previous month, before.
	if (_month < other->_month)
		return true;

	// Following month, after.
	if (_month > other->_month)
		return false;

	// Previous day, before.
	if (_day < other->_day)
		return true;

	// Following day, after.
	if (_day > other->_day)
		return false;

	// Previous hour, before.
	if (_hour < other->_hour)
		return true;

	// Following hour, after.
	if (_hour > other->_hour)
		return false;

	// Previous minute, before.
	if (_minute < other->_minute)
		return true;

	// Following minute, after.
	if (_minute > other->_minute)
		return false;

	// Previous second, before.
	if (_second < other->_second)
		return true;

	// Following or same second, after or equal.
	return false;
}

bool Date::After(Date* other)
{
	// Previous year, before.
	if (_year < other->_year)
		return false;

	// Following year, after.
	if (_year > other->_year)
		return true;

	// Previous month, before.
	if (_month < other->_month)
		return false;

	// Following month, after.
	if (_month > other->_month)
		return true;

	// Previous day, before.
	if (_day < other->_day)
		return false;

	// Following day, after.
	if (_day > other->_day)
		return true;

	// Previous hour, before.
	if (_hour < other->_hour)
		return false;

	// Following hour, after.
	if (_hour > other->_hour)
		return true;

	// Previous minute, before.
	if (_minute < other->_minute)
		return false;

	// Following minute, after.
	if (_minute > other->_minute)
		return true;

	// Following second, after.
	if (_second > other->_second)
		return true;

	// Previous or same second, before or equal.
	return false;
}

void Date::Activate()
{
	_active = true;
}

bool Date::Equal(Date& other)
{
	if (_year != other._year)
		return false;

	if (_month != other._month)
		return false;

	if (_day != other._day)
		return false;

	if (_hour != other._hour)
		return false;

	if (_minute != other._minute)
		return false;

	if (_second != other._second)
		return false;

	return true;
}

const char* Date::GetLongString()
{
	char s[0x40];
	UplinkSnprintf(s, 0x40, "%.2d:%.2d.%.2d, %d %s %d", _hour, _minute, _second, _day, GetMonthName(_month), _year);
	UplinkStrncpy(_tempdate, s, 0x40);
	return _tempdate;
}

const char* Date::GetMonthName(int month)
{
	UplinkAssert(month >= 1 && month <= 12);
	return _monthname[month];
}
