#pragma once

#include <UplinkObject.hpp>

class GameObituary : UplinkObject
{
	char* _gameOverReason = nullptr;
	char _name[0x80] = " ";
	int _money;
	int _uplinkRating;
	int _neuromancerRating;
	int _specialMissionsCompleted;
	int _livesRuined;
	int _systemsDestroyed;
	int _highSecurityHacks;
	int _score;
	bool _demoGameOver;
	bool _warezGameOver;

public:
	~GameObituary() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
};
