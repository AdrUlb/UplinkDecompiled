#include <Rating.hpp>

#include <Util.hpp>

struct RatingInfo
{
	const char* Name;
	int Score;
};

struct RatingInfo uplinkRatings[0x11] = {{"Unregistered", 0},	{"Registered", 1}, {"Beginner", 2},		{"Novice", 6},			{"Confident", 15},
										 {"Intermediate", 35},	{"Skilled", 60},   {"Experienced", 90}, {"Knowledgeable", 125}, {"Uber-Skilled", 160},
										 {"Professional", 220}, {"Elite", 300},	   {"Mage", 400},		{"Expert", 600},		{"Veteren", 1000},
										 {"Techno-mage", 1500}, {"TERMINAL", 2500}};

bool Rating::Load(FILE* file)
{
	(void)file;
	puts("TODO: implement Rating::Load()");
	return false;
}

void Rating::Save(FILE* file)
{
	(void)file;
	puts("TODO: implement Rating::Save()");
}

void Rating::Print()
{
	puts("TODO: implement Rating::Print()");
}

const char* Rating::GetID()
{
	return "RATING";
}

void Rating::SetOwner(const char* owner)
{
	UplinkStrncpy(_owner, owner, 0x80);
}

void Rating::SetUplinkRating(int uplinkRating)
{
	UplinkAssert(uplinkRating <= 16);
	_uplinkRating = uplinkRating;
	_uplinkScore = uplinkRatings[_uplinkRating].Score;
	if (_creditRating < _uplinkRating)
		_creditRating = _uplinkRating;
}

void Rating::SetNeuromancerRating(int neuromancerRating)
{
	UplinkAssert(neuromancerRating <= 10);
	_neuromancerRating = neuromancerRating;
	_neuromancerScore = uplinkRatings[_neuromancerRating].Score;
}

void Rating::SetCreditRating(int creditRating)
{
	_creditRating = creditRating;
}