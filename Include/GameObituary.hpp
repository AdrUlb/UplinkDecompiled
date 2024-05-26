#pragma once

#include <UplinkObject.hpp>

class GameObituary : UplinkObject
{
	const char* gameOverReason;
	char name[0x80];
	int money;
	int uplinkRating;
	int neuromancerRating;
	int specialMissionsCompleted;
	int livesRuined;
	int systemsDestroyed;
	int highSecurityHacks;
	int score;
	bool demoGameOver;
	bool warezGameOver;

public:
	GameObituary();
	~GameObituary() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
};
