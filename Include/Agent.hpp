#pragma once

#include <BTree.hpp>
#include <LList.hpp>
#include <Mission.hpp>
#include <Person.hpp>

class Agent : Person
{
	LList<char*> links;
	BTree<char*> accessCodes;
	LList<Mission*> missions;
	char handle[0x40];

	Agent();
	~Agent() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	void GiveMessage(Message* message) override;
	void CreateNewAccount(const char* bankIp, const char* owner, const char* password, int amount, int loan) override;
};
