#include <Person.hpp>

#include <Globals.hpp>
#include <Interface/RemoteScreens/LinksScreenInterface.hpp>

Person::~Person()
{
	DeleteLListData(reinterpret_cast<LList<UplinkObject*>*>(&_messages));
	DeleteLListData(&_bankAccounts);
}

bool Person::Load(FILE* file)
{
	if (!LoadDynamicStringBuf(_name, 0x80, file))
		return false;

	if (!LoadDynamicStringBuf(_localHostIp, 0x18, file))
		return false;

	if (!LoadDynamicStringBuf(_remoteHostIp, 0x18, file))
		return false;

	if (!LoadDynamicStringBuf(_phoneNumber, 0x18, file))
		return false;

	// TODO: this does not seem right? it warns about an already existing thing when it *doesn't* find anything??
	if (game->GetWorld().GetVLocations().LookupTree(_localHostIp) == nullptr)
	{
		printf("Print Abort: %s ln %d : ", __FILE__, __LINE__);
		printf("WARNING: Person::Load, Localhost IP '%s' already existing\n", _localHostIp);
		return false;
	}

	if (game->GetWorld().GetVLocations().LookupTree(_remoteHostIp) == nullptr)
	{
		printf("Print Abort: %s ln %d : ", __FILE__, __LINE__);
		printf("WARNING: Person::Load, Remotehost IP '%s' already existing\n", _remoteHostIp);
		return false;
	}

	if (!FileReadData(&_age, 4, 1, file))
		return false;

	if (!FileReadData(&_photoIndex, 4, 1, file))
		return false;

	if (!FileReadData(&_voiceIndex, 4, 1, file))
		return false;

	if (!FileReadData(&_currentAccount, 4, 1, file))
		return false;

	if (!FileReadData(&_isTargetable, 1, 1, file))
		return false;

	if (!FileReadData(&_status, 4, 1, file))
		return false;

	if (!LoadLList(reinterpret_cast<LList<UplinkObject*>*>(&_messages), file))
		return false;

	if (!LoadLList(&_bankAccounts, file))
		return false;

	if (!_connection.Load(file))
		return false;

	if (!_rating.Load(file))
		return false;

	return true;
}

void Person::Save(FILE* file)
{
	SaveDynamicString(_name, 0x80, file);
	SaveDynamicString(_localHostIp, 0x18, file);
	SaveDynamicString(_remoteHostIp, 0x18, file);
	SaveDynamicString(_phoneNumber, 0x18, file);
	fwrite(&_age, 4, 1, file);
	fwrite(&_photoIndex, 4, 1, file);
	fwrite(&_voiceIndex, 4, 1, file);
	fwrite(&_currentAccount, 4, 1, file);
	fwrite(&_isTargetable, 1, 1, file);
	fwrite(&_status, 4, 1, file);
	SaveLList(reinterpret_cast<LList<UplinkObject*>*>(&_messages), file);
	SaveLList(&_bankAccounts, file);
	_connection.Save(file);
	_rating.Save(file);
}

void Person::Print()
{
	printf("Person : ");
	printf("name = %s, Age = %d, photoindex = %d, voiceindex = %d, status = %d\n", _name, _age, _photoIndex, _voiceIndex, _status);
	printf("LocalHost : %s, RemoteHost : %s, Phone : %s\n", _localHostIp, _remoteHostIp, _phoneNumber);
	puts("Messages : ");
	PrintLList(reinterpret_cast<LList<UplinkObject*>*>(&_messages));
	PrintLList(&_bankAccounts);
	printf("currentaccount:%d\n", _currentAccount);
	_connection.Print();
	_rating.Print();
	printf("Is Targetable? %d\n", _isTargetable);
}

void Person::Update()
{

	if (strcmp(_name, "PLAYER") == 0)
		return;

	if (_messages.Size() <= 0)
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

const char* Person::GetName()
{
	return _name;
}

const char* Person::GetLocalHostIp()
{
	return _localHostIp;
}

VLocation* Person::GetLocalHost()
{
	const auto ret = game->GetWorld().GetVLocation(_localHostIp);
	UplinkAssert(ret != nullptr);
	return ret;
}

const char* Person::GetRemoteHostIp()
{
	return _remoteHostIp;
}

VLocation* Person::GetRemoteHost()
{
	const auto ret = game->GetWorld().GetVLocation(_remoteHostIp);
	UplinkAssert(ret != nullptr);
	return ret;
}

LList<Message*>& Person::GetMessages()
{
	return _messages;
}

Connection& Person::GetConnection()
{
	return _connection;
}

void Person::SetName(const char* name)
{
	UplinkStrncpy(_name, name, 0x80);
	_connection.SetOwner(_name);
	_rating.SetOwner(_name);
}

void Person::SetAge(int age)
{
	_age = age;
}

void Person::SetLocalHost(const char* value)
{
	UplinkStrncpy(_localHostIp, value, 0x18);
	UplinkAssert(game->GetWorld().GetVLocation(_localHostIp) != nullptr);
}

void Person::SetRemoteHost(const char* remoteHost)
{
	UplinkStrncpy(_remoteHostIp, remoteHost, 0x18);
	UplinkAssert(game->GetWorld().GetVLocation(_remoteHostIp) != nullptr);
}

void Person::SetIsTargetable(bool isTargetable)
{
	_isTargetable = isTargetable;
}

void Person::GiveMessage(Message* message)
{
	_messages.PutData(message);
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

Agent::~Agent()
{
	DeleteLListData(&_links);
	DeleteBTreeData(&_accessCodes);
	DeleteLListData(reinterpret_cast<LList<UplinkObject*>*>(&_missions));
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

LList<char*>& Agent::GetLinks()
{
	return _links;
}

LList<Mission*>& Agent::GetMissions()
{
	return _missions;
}

const char* Agent::GetHandle()
{
	return _handle;
}

int Agent::HasAccount(const char* ip)
{
	(void)ip;
	puts("TODO: implement Agent::HasAccount()");
	return -1;
}

bool Agent::HasLink(const char* ip)
{
	for (auto i = 0; i < _links.Size(); i++)
	{
		if (!_links.ValidIndex(i))
			continue;

		if (strcmp(ip, _links.GetData(i)) == 0)
			return true;
	}

	return false;
}

void Agent::GiveLink(const char* ip)
{
	if (!HasLink(ip))
	{
		const auto var_20 = new char[0x18];
		UplinkStrncpy(var_20, ip, 0x18);
		_links.PutDataAtStart(var_20);

		if (strcmp(GetName(), "NEWAGENT") == 0)
		{
			if (strcmp(game->GetWorld().GetPlayer().GetRemoteHostIp(), "127.0.0.1") == 0 &&
				game->GetInterface().GetRemoteInterface().GetScreenIndex() == 0)
			{
				dynamic_cast<LinksScreenInterface*>(game->GetInterface().GetRemoteInterface().GetInterfaceScreen())->SetFullList(&_links);
				dynamic_cast<LinksScreenInterface*>(game->GetInterface().GetRemoteInterface().GetInterfaceScreen())->ApplyFilter(nullptr);
			}
		}
	}

	const auto vlocation = game->GetWorld().GetVLocation(ip);
	UplinkAssert(vlocation != nullptr);
	vlocation->SetDisplayed(true);
}

void Agent::SetHandle(const char* handle)
{
	UplinkStrncpy(_handle, handle, 0x40);
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
	DeleteBTreeData(&_shares);
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
		puts("TODO: implement Player::Update()");
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

Gateway& Player::GetGateway()
{
	return _gateway;
}

void Player::GiveMessage(Message* message)
{
	(void)message;
	puts("TODO: implement Person::GiveMessage()");
}
