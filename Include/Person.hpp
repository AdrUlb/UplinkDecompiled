#pragma once

#include <Connection.hpp>
#include <Message.hpp>
#include <Rating.hpp>
#include <UplinkObject.hpp>

class Person : UplinkObject
{
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

public:
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
};
