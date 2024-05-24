#pragma once

#include <UplinkObject.hpp>

struct RatingData
{
	const char* name;
	int unknown;
};

class Rating : UplinkObject
{
	char owner[0x80];
	int uplinkRating;
	int uplinkScore;
	int neuromancerRating;
	int neuromancerScore;
	int creditRating;

	Rating();
	~Rating() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
};
