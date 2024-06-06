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
#include <VLocation.hpp>

class Person : public UplinkObject
{
	char _name[0x80] = " ";
	int _age = -1;
	int _photoIndex = 0;
	int _voiceIndex = 0;
	char _localHostIp[0x18] = " ";
	char _remoteHostIp[0x18] = " ";
	char _phoneNumber[0x18] = " ";
	LList<Message*> _messages;
	LList<char*> _bankAccounts;
	int _currentAccount = 0;
	Connection _connection;
	Rating _rating;
	bool _isTargetable = true;
	int _status = 0;

public:
	~Person() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	const char* GetName();
	const char* GetLocalHostIp();
	VLocation* GetLocalHost();
	VLocation* GetRemoteHost();
	LList<Message*>& GetMessages();
	int GetCurrentAccount();
	Connection& GetConnection();
	Rating& GetRating();
	void SetName(const char* name);
	void SetAge(int age);
	void SetLocalHost(const char* localHost);
	const char* GetRemoteHostIp();
	void SetRemoteHost(const char* remoteHost);
	void SetCurrentAccount(int account);
	void SetIsTargetable(bool isTargetable);
	virtual void GiveMessage(Message* message);
	virtual void CreateNewAccount(const char* bankIp, const char* owner, const char* password, int amount, int loan);
};

class Agent : public Person
{
	LList<char*> _links;
	BTree<char*> _accessCodes;
	LList<Mission*> _missions;
	char _handle[0x40];

public:
	~Agent() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	void GiveMessage(Message* message) override;
	void CreateNewAccount(const char* bankIp, const char* owner, const char* password, int amount, int loan) override;
	LList<char*>& GetLinks();
	LList<Mission*>& GetMissions();
	const char* GetHandle();
	int HasAccount(const char* ip);
	bool HasLink(const char* ip);
	void GiveLink(const char* ip);
	void SetHandle(const char* handle);
};

class Player : public Agent
{
	BTree<char*> _shares;
	Gateway _gateway;
	int _livesRuined = 0;
	int _systemsDestroyed = 0;
	int _highSecurityHacks = 0;

public:
	~Player() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	void GiveMessage(Message* message) override;
	Gateway& GetGateway();
};
