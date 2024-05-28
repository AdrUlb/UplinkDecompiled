#pragma once

#include <BTree.hpp>
#include <Connection.hpp>
#include <Gateway.hpp>
#include <LList.hpp>
#include <Message.hpp>
#include <Mission.hpp>
#include <Person.hpp>
#include <Rating.hpp>
#include <UplinkObject.hpp>

class Person : public UplinkObject
{
public:
	char name[0x80];
	int age;
	int photoIndex;
	int voiceIndex;
	char localHost[0x18];
	char remoteHost[0x18];
	char phoneNumber[0x18];
	LList<Message*> messages;
	LList<char*> bankAccounts;
	int currentAccount;
	Connection connection;
	Rating rating;
	bool isTargetable;
	int status;

	Person();
	~Person() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	virtual void GiveMessage(Message* message);
	virtual void CreateNewAccount(const char* bankIp, const char* owner, const char* password, int amount, int loan);
	void SetName(const char* value);
	void SetLocalHost(const char* value);
	void SetRemoteHost(const char* value);
	void SetIsTargetable(bool value);
};

class Agent : public Person
{
public:
	LList<char*> links;
	BTree<char*> accessCodes;
	LList<Mission*> missions;
	char handle[0x40];

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

class Player : public Agent
{
public:
	BTree<char*> shares;
	Gateway gateway;
	int32_t livesRuined = 0;
	int32_t systemsDestroyed = 0;
	int32_t highSecurityHacks = 0;

	~Player() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	void GiveMessage(Message* message) override;
};
