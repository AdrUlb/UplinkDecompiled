#include <Person.hpp>

#include <Globals.hpp>

Person::~Person()
{
	DeleteLListData(reinterpret_cast<LList<UplinkObject*>*>(&messages));
	DeleteLListData(&bankAccounts);
}

bool Person::Load(FILE* file)
{
	if (!LoadDynamicStringBuf(name, 0x80, file))
		return false;

	if (!LoadDynamicStringBuf(localHost, 0x18, file))
		return false;

	if (!LoadDynamicStringBuf(remoteHost, 0x18, file))
		return false;

	if (!LoadDynamicStringBuf(phoneNumber, 0x18, file))
		return false;

	// TODO: this does not seem right? it warns about an already existing thing when it *doesn't* find anything??
	if (game->GetWorld()->vlocations.LookupTree(localHost) == nullptr)
	{
		printf("Print Abort: %s ln %d : ", __FILE__, __LINE__);
		printf("WARNING: Person::Load, Localhost IP '%s' already existing\n", localHost);
		return false;
	}

	if (game->GetWorld()->vlocations.LookupTree(remoteHost) == nullptr)
	{
		printf("Print Abort: %s ln %d : ", __FILE__, __LINE__);
		printf("WARNING: Person::Load, Remotehost IP '%s' already existing\n", remoteHost);
		return false;
	}

	if (!FileReadData(&age, 4, 1, file))
		return false;

	if (!FileReadData(&photoIndex, 4, 1, file))
		return false;

	if (!FileReadData(&voiceIndex, 4, 1, file))
		return false;

	if (!FileReadData(&currentAccount, 4, 1, file))
		return false;

	if (!FileReadData(&isTargetable, 1, 1, file))
		return false;

	if (!FileReadData(&status, 4, 1, file))
		return false;

	if (!LoadLList(reinterpret_cast<LList<UplinkObject*>*>(&messages), file))
		return false;

	if (!LoadLList(&bankAccounts, file))
		return false;

	if (!connection.Load(file))
		return false;

	if (!rating.Load(file))
		return false;

	return true;
}

void Person::Save(FILE* file)
{
	SaveDynamicString(name, 0x80, file);
	SaveDynamicString(localHost, 0x18, file);
	SaveDynamicString(remoteHost, 0x18, file);
	SaveDynamicString(phoneNumber, 0x18, file);
	fwrite(&age, 4, 1, file);
	fwrite(&photoIndex, 4, 1, file);
	fwrite(&voiceIndex, 4, 1, file);
	fwrite(&currentAccount, 4, 1, file);
	fwrite(&isTargetable, 1, 1, file);
	fwrite(&status, 4, 1, file);
	SaveLList(reinterpret_cast<LList<UplinkObject*>*>(&messages), file);
	SaveLList(&bankAccounts, file);
	connection.Save(file);
	rating.Save(file);
}

void Person::Print()
{
	printf("Person : ");
	printf("name = %s, Age = %d, photoindex = %d, voiceindex = %d, status = %d\n", name, age, photoIndex, voiceIndex, status);
	printf("LocalHost : %s, RemoteHost : %s, Phone : %s\n", localHost, remoteHost, phoneNumber);
	puts("Messages : ");
	PrintLList(reinterpret_cast<LList<UplinkObject*>*>(&messages));
	PrintLList(&bankAccounts);
	printf("currentaccount:%d\n", currentAccount);
	connection.Print();
	rating.Print();
	printf("Is Targetable? %d\n", isTargetable);
}

void Person::Update()
{

	if (strcmp(name, "PLAYER") == 0)
		return;

	if (messages.Size() <= 0)
		return;

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
	messages.PutData(message);
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

VLocation* Person::GetRemoteHost()
{
	const auto ret = game->GetWorld()->GetVLocation(remoteHost);
	UplinkAssert(ret != nullptr);
	return ret;
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

Connection* Person::GetConnection()
{
	return &connection;
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
	Person::Update();
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
	puts("TODO: implement Player::Load()");
	return false;
}

void Player::Save(FILE* file)
{
	(void)file;
	puts("TODO: implement Player::Save()");
}

void Player::Print()
{
	puts("TODO: implement Player::Print()");
}

void Player::Update()
{
	static auto called = false;
	if (!called)
	{
		puts("TODO: Player::Update()");
		called = true;
	}
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
