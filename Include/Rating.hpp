#pragma once

#include <UplinkObject.hpp>

struct RatingData
{
	const char* name;
	int unknown;
};

class Rating : UplinkObject
{
	char owner[0x80] = "";
	int uplinkRating = 0;
	int uplinkScore = 0;
	int neuromancerRating = 0;
	int neuromancerScore = 0;
	int creditRating = 0;

public:
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	void SetOwner(const char* value);
};
