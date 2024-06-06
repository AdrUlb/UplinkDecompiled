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
	if (!LoadDynamicStringBuf(_owner, 0x80, file))
		return false;

	if (!FileReadData(&_uplinkScore, 4, 1, file))
		return false;

	if (!FileReadData(&_neuromancerScore, 4, 1, file))
		return false;

	if (!FileReadData(&uplinkRatings, 4, 1, file))
		return false;

	if (!FileReadData(&_neuromancerRating, 4, 1, file))
		return false;

	if (!FileReadData(&_creditRating, 4, 1, file))
		return false;

	return true;
}

void Rating::Save(FILE* file)
{
	SaveDynamicString(_owner, 0x80, file);
	fwrite(&_uplinkScore, 4, 1, file);
	fwrite(&_neuromancerScore, 4, 1, file);
	fwrite(&_uplinkRating, 4, 1, file);
	fwrite(&_neuromancerRating, 4, 1, file);
	fwrite(&_creditRating, 4, 1, file);
}

void Rating::Print()
{
	printf("Rating : Owner:%s\n", _owner);
	printf("\tUplink Score      = %d (%s)\n", _uplinkScore, GetUplinkRating());
	printf("\tNeuromancer Score = %d (%s)\n", _neuromancerScore, GetNeuromancerRating());
	printf("\tCredit rating     = %d\n", _creditRating);
}

const char* Rating::GetID()
{
	return "RATING";
}

int Rating::GetUplinkRating()
{
	return _uplinkRating;
}

int Rating::GetNeuromancerRating()
{
	return _neuromancerRating;
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