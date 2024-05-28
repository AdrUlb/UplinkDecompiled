#include <Person.hpp>

#include <Globals.hpp>

Person::Person()
{
	puts("TODO: implement Person::Person()");
}

Person::~Person()
{
	puts("TODO: implement Person::~Person()");
}

bool Person::Load(FILE* file)
{
	(void)file;
	puts("TODO: implement Person::Load()");
	return false;
}

void Person::Save(FILE* file)
{
	(void)file;
	puts("TODO: implement Person::Save()");
}

void Person::Print()
{
	puts("TODO: implement Person::Print()");
}

void Person::Update()
{
	puts("TODO: implement Person::Update()");
}

const char* Person::GetID()
{
	return "PERSON";
}

UplinkObjectId Person::GetOBJECTID()
{
	return UplinkObjectId::Person;
}

void Person::GiveMessage(Message* message)
{
	(void)message;
	puts("TODO: implement Person::CreateNewAccount()");
}

void Person::CreateNewAccount(const char* bankIp, const char* owner, const char* password, int amount, int loan)
{
	(void)bankIp;
	(void)owner;
	(void)password;
	(void)amount;
	(void)loan;
	puts("TODO: implement Person::CreateNewAccount()");
}

void Person::SetName(const char* value)
{
	UplinkStrncpy(name, value, 0x80);
	connection.SetOwner(name);
	rating.SetOwner(name);
}

void Person::SetLocalHost(const char* value)
{
	UplinkStrncpy(localHost, value, 0x18);
	UplinkAssert(game->GetWorld()->GetVLocation(localHost) != nullptr);
}

void Person::SetRemoteHost(const char* value)
{
	UplinkStrncpy(remoteHost, value, 0x18);
	UplinkAssert(game->GetWorld()->GetVLocation(remoteHost) != nullptr);
}

void Person::SetIsTargetable(bool value)
{
    isTargetable = value;
}

Agent::~Agent()
{
	DeleteLListData(&links);
	DeleteBTreeData(&accessCodes);
	DeleteLListData(reinterpret_cast<LList<UplinkObject*>*>(&missions));
}

bool Agent::Load(FILE* file)
{
	(void)file;
	puts("TODO: implement Agent::Load()");
	return false;
}

void Agent::Save(FILE* file)
{

	(void)file;
	puts("TODO: implement Agent::Save()");
}

void Agent::Print()
{
	puts("TODO: implement Agent::Print()");
}

void Agent::Update()
{
	puts("TODO: implement Agent::Update()");
}

const char* Agent::GetID()
{
	return "AGENT";
}

UplinkObjectId Agent::GetOBJECTID()
{
	return UplinkObjectId::Agent;
}

void Agent::GiveMessage(Message* message)
{
	(void)message;
	puts("TODO: implement Agent::GiveMessage()");
}

void Agent::CreateNewAccount(const char* bankIp, const char* owner, const char* password, int amount, int loan)
{
	(void)bankIp;
	(void)owner;
	(void)password;
	(void)amount;
	(void)loan;
	puts("TODO: implement Agent::CreateNewAccount()");
}

Player::~Player()
{
	DeleteBTreeData(&shares);
}

bool Player::Load(FILE* file)
{
	(void)file;
	puts("TODO: implement Person::Load()");
	return false;
}

void Player::Save(FILE* file)
{
	(void)file;
	puts("TODO: implement Person::Save()");
}

void Player::Print()
{
	puts("TODO: implement Person::Print()");
}

void Player::Update()
{
	puts("TODO: implement Person::Person()");
}

const char* Player::GetID()
{
	return "PLAYER";
}

UplinkObjectId Player::GetOBJECTID()
{
	return UplinkObjectId::Player;
}

void Player::GiveMessage(Message* message)
{
	(void)message;
	puts("TODO: implement Person::GiveMessage()");
}
