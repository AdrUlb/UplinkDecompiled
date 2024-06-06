#pragma once

#include <UplinkObject.hpp>

struct RatingData
{
	const char* Name;
	int Unknown;
};

class Rating : UplinkObject
{
	char _owner[0x80] = "";
	int _uplinkRating = 0;
	int _uplinkScore = 0;
	int _neuromancerRating = 0;
	int _neuromancerScore = 0;
	int _creditRating = 0;

public:
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	int GetUplinkRating();
	int GetNeuromancerRating();
	void SetOwner(const char* owner);
	void SetUplinkRating(int uplinkRating);
	void SetNeuromancerRating(int neuromancerRating);
	void SetCreditRating(int creditRating);
};
