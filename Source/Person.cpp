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

int Person::GetCurrentAccount()
{
	return _currentAccount;
}

Connection& Person::GetConnection()
{
	return _connection;
}

const Connection& Person::GetConnection() const
{
	return _connection;
}

Rating& Person::GetRating()
{
	return _rating;
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

void Person::SetCurrentAccount(int account)
{
	_currentAccount = account;
}

void Person::SetIsTargetable(bool isTargetable)
{
	_isTargetable = isTargetable;
}

bool Person::IsConnected()
{
	return strcmp(_localHostIp, _remoteHostIp) != 0;
}

Agent::~Agent()
{
	DeleteLListData(&_links);
	DeleteBTreeData(&_accessCodes);
	DeleteLListData(reinterpret_cast<LList<UplinkObject*>*>(&_missions));
}

bool Agent::Load(FILE* file)
{
	if (!Person::Load(file))
		return false;

	if (!LoadDynamicStringBuf(_handle, 0x40, file))
		return false;

	if (!LoadLList(&_links, file))
		return false;

	if (!LoadBTree(&_accessCodes, file))
		return false;

	if (!LoadLList(reinterpret_cast<LList<UplinkObject*>*>(&_missions), file))
		return false;

	const auto ips = _accessCodes.ConvertIndexToDArray();
	const auto codes = _accessCodes.ConvertToDArray();

	if (ips == nullptr)
	{
		if (codes != nullptr)
			delete codes;

		return true;
	}

	if (codes == nullptr)
	{
		delete ips;
		return true;
	}

	for (auto i = 0; i < ips->Size(); i++)
	{
		if (!ips->ValidIndex(i))
			continue;

		if (!codes->ValidIndex(i))
			continue;

		if (ips->GetData(i) == nullptr)
			continue;

		auto ip = ips->GetData(i);
		auto code = codes->GetData(i);

		auto deleteCode = false;

		if (code == nullptr)
		{
			_accessCodes.RemoveData(ip, code);
			continue;
		}

		if (strlen(code) > 0xFF)
		{
			deleteCode = true;
		}

		if (!deleteCode)
		{
			char name[0x100];
			char password[0x100];
			if (!ParseAccessCode(code, name, 0x100, password, 0x100))
				deleteCode = true;
		}

		if (!deleteCode)
		{
			for (int j = strlen(code) - 1; j >= 0; j--)
			{
				if (code[j] <= 31)
				{
					deleteCode = true;
					break;
				}
			}
		}

		if (deleteCode)
		{
			_accessCodes.RemoveData(ip, code);
			delete[] code;
			continue;
		}
	}

	delete ips;
	delete codes;
	return true;
}

void Agent::Save(FILE* file)
{
	Person::Save(file);
	SaveDynamicString(_handle, 0x40, file);
	SaveLList(&_links, file);
	SaveBTree(&_accessCodes, file);
	SaveLList(reinterpret_cast<LList<UplinkObject*>*>(&_missions), file);
}

void Agent::Print()
{
	printf("Uplink Agent : Handle %s\n", _handle);
	Person::Print();
	PrintLList(&_links);
	PrintBTree(&_accessCodes);
	PrintLList(reinterpret_cast<LList<UplinkObject*>*>(&_missions));
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

LList<char*>& Agent::GetLinks()
{
	return _links;
}

BTree<char*>& Agent::GetAccessCodes()
{
	return _accessCodes;
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
		const auto str = new char[0x18];
		UplinkStrncpy(str, ip, 0x18);
		_links.PutDataAtStart(str);

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

bool Agent::ParseAccessCode(const char* code, char* name, size_t nameMax, char* password, size_t passwordMax)
{
	char buf[0x100];
	UplinkStrncpy(buf, code, 0x100);

	auto quoteCount = 0;

	auto quoteStr = buf;
	while (true)
	{
		const auto match = strchr(quoteStr, '\'');
		if (quoteStr == nullptr)
			break;

		quoteStr = match + 1;
		quoteCount++;
	}

	if (quoteCount == 2)
	{
		// Start of password string is immediately after the first quote
		const auto pwStr = strchr(buf, '\'') + 1;

		// End of password string is the second quote
		*strchr(pwStr, '\'') = 0;

		if (nameMax != 0)
			UplinkStrncpy(name, pwStr, nameMax);

		if (passwordMax != 0)
			UplinkStrncpy(password, pwStr, passwordMax);

		return true;
	}

	if (quoteCount == 4)
	{
		// Start of name string is immediately after the first quote
		const auto nameStr = strchr(buf, '\'') + 1;
		// End of name string is the second quote
		const auto nameStrEnd = strchr(nameStr, '\'');
		// Start of password string is immediately after the third (first after the second)
		const auto pwStr = strchr(nameStrEnd + 1, '\'') + 1;
		// Write null terminator
		*nameStrEnd = 0;
		*strchr(pwStr, '\'') = 0;
		if (nameMax != 0)
		{
			UplinkStrncpy(name, nameStr, nameMax);
		}
		if (passwordMax != 0)
			UplinkStrncpy(password, pwStr, passwordMax);

		return true;
	}

	if (nameMax != 0)
		name[0] = 0;

	if (passwordMax != 0)
		password[0] = 0;

	return false;
}

Player::~Player()
{
	DeleteBTreeData(&_shares);
}

bool Player::Load(FILE* file)
{
	if (!Agent::Load(file))
		return false;

	if (!_gateway.Load(file))
		return false;

	if (!FileReadData(&_livesRuined, 4, 1, file))
		return false;

	if (!FileReadData(&_systemsDestroyed, 4, 1, file))
		return false;

	if (!FileReadData(&_highSecurityHacks, 4, 1, file))
		return false;

	if (!LoadBTree(&_shares, file))
		return false;

	return true;
}

void Player::Save(FILE* file)
{
	Agent::Save(file);
	_gateway.Save(file);
	fwrite(&_livesRuined, 4, 1, file);
	fwrite(&_systemsDestroyed, 4, 1, file);
	fwrite(&_highSecurityHacks, 4, 1, file);
	SaveBTree(&_shares, file);
}

void Player::Print()
{
	puts("Player : ");
	Agent::Print();
	_gateway.Print();
	printf("Score : People's lives ruined : %d\n", _livesRuined);
	printf("Score : Systems destroyed : %d\n", _systemsDestroyed);
	printf("Score : High Security Hacks : %d\n", _highSecurityHacks);
	PrintBTree(&_shares);
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
