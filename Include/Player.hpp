#pragma once

#include <Agent.hpp>
#include <BTree.hpp>
#include <Gateway.hpp>

class Player : Agent
{
	BTree<char*> shares;
	Gateway gateway;
	int32_t livesRuined;
	int32_t systemsDestroyed;
	int32_t highSecurityHacks;

	Player();
	~Player() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	void GiveMessage(Message* message) override;
};
