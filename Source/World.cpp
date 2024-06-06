#include <World.hpp>

#include <MissionGenerator.hpp>
#include <NameGenerator.hpp>
#include <RedShirt.hpp>
#include <Util.hpp>
#include <WorldGenerator.hpp>
#include <fstream>

World::World()
{
	_currentDate.SetDate(0, 0, 0, 24, 2, 2010);
	_currentDate.Activate();
	WorldGenerator::Initialise();
	MissionGenerator::Initialise();
	_passwords.SetStepSize(100);

	const auto wordlist = RsArchiveFileOpen("data/wordlist.txt", "rt");
	UplinkAssert(wordlist != nullptr);
	while (!feof(wordlist))
	{
		char buf[0x40];
		fscanf(wordlist, "%s\n", buf);
		CreatePassword(buf);
	}
	RsArchiveFileClose("data/wordlist.txt", wordlist);

	const auto gatewayDefFile = RsArchiveFileOpen("data/gatewaydefs.txt");
	UplinkAssert(gatewayDefFile != nullptr);

	std::ifstream gatewayDefStream;
	gatewayDefStream.open(gatewayDefFile);

	char temp[0x100];
	gatewayDefStream.getline(temp, 0x100, '\r');
	if ((temp[0] = gatewayDefStream.get()) != '\n')
		gatewayDefStream.rdbuf()->sputbackc(temp[0]);

	gatewayDefStream >> temp >> std::ws;
	int gatewayDefCount;
	gatewayDefStream >> gatewayDefCount;
	gatewayDefStream.getline(temp, 0x100, '\r');
	if ((temp[0] = gatewayDefStream.get()) != '\n')
		gatewayDefStream.rdbuf()->sputbackc(temp[0]);

	for (auto i = 0; i < gatewayDefCount; i++)
	{
		const auto gatewayDef = new GatewayDef();
		gatewayDef->LoadGatewayDefinition(gatewayDefStream);

		char filename[0x100];
		UplinkSnprintf(filename, 0x100, "gateway/gateway%d.tif", i);
		char thumbnail[0x100];
		UplinkSnprintf(thumbnail, 0x100, "gateway/gateway_t%d.tif", i);

		gatewayDef->SetFilename(filename);
		gatewayDef->SetThumbnail(thumbnail);
		CreateGatewayDef(gatewayDef);
	}

	gatewayDefStream.close();
	RsArchiveFileClose("data/gatewaydefs.txt", nullptr);
}

World::~World()
{
	WorldGenerator::Shutdown();
	NameGenerator::Shutdown();
	MissionGenerator::Shutdown();

	DeleteBTreeData(&_vlocations);
	DeleteBTreeData(&_companies);
	DeleteBTreeData(&_computers);
	DeleteBTreeData(&_people);
	DeleteDArrayDataD(&_passwords);

	for (auto i = 0; i < _gatewayDefs.Size(); i++)
	{
		if (!_gatewayDefs.ValidIndex(i))
			continue;

		const auto gatewayDef = _gatewayDefs.GetData(i);

		if (gatewayDef != nullptr)
			delete gatewayDef;
	}
}

bool World::Load(FILE* file)
{
	if (!_currentDate.Load(file))
		return false;

	if (!_eventScheduler.Load(file))
		return false;

	if (!_plotGenerator.Load(file))
		return false;

	if (!_demoPlotGenerator.Load(file))
		return false;

	if (!LoadBTree(reinterpret_cast<BTree<UplinkObject*>*>(&_vlocations), file))
		return false;

	if (!LoadBTree(reinterpret_cast<BTree<UplinkObject*>*>(&_companies), file))
		return false;

	if (!LoadBTree(reinterpret_cast<BTree<UplinkObject*>*>(&_computers), file))
		return false;

	if (!LoadBTree(reinterpret_cast<BTree<UplinkObject*>*>(&_people), file))
		return false;

	WorldGenerator::ReplaceInvalidCompanyAdmins();
	WorldGenerator::UpdateSoftwareUpgrades();

	return true;
}

void World::Save(FILE* file)
{
	_currentDate.Save(file);
	_eventScheduler.Save(file);
	_plotGenerator.Save(file);
	_demoPlotGenerator.Save(file);
	SaveBTree(reinterpret_cast<BTree<UplinkObject*>*>(&_vlocations), file);
	SaveBTree(reinterpret_cast<BTree<UplinkObject*>*>(&_companies), file);
	SaveBTree(reinterpret_cast<BTree<UplinkObject*>*>(&_computers), file);
	SaveBTree(reinterpret_cast<BTree<UplinkObject*>*>(&_people), file);
}

void World::Print()
{
	puts("============== W O R L D ===================================");
	_currentDate.Print();
	_eventScheduler.Print();
	_plotGenerator.Print();
	_demoPlotGenerator.Print();
	PrintBTree(reinterpret_cast<BTree<UplinkObject*>*>(&_vlocations));
	PrintBTree(reinterpret_cast<BTree<UplinkObject*>*>(&_companies));
	PrintBTree(reinterpret_cast<BTree<UplinkObject*>*>(&_computers));
	PrintBTree(reinterpret_cast<BTree<UplinkObject*>*>(&_people));
	puts("============== E N D  O F  W O R L D =======================");
}

void World::Update()
{
	_currentDate.Update();
	if (_currentDate.After(&_nextUpdateDate) != 0)
	{
		UpdateBTree(reinterpret_cast<BTree<UplinkObject*>*>(&_vlocations));
		UpdateBTree(reinterpret_cast<BTree<UplinkObject*>*>(&_companies));
		UpdateBTree(reinterpret_cast<BTree<UplinkObject*>*>(&_computers));
		UpdateBTree(reinterpret_cast<BTree<UplinkObject*>*>(&_people));
		_eventScheduler.Update();
		_nextUpdateDate.SetDate(&_currentDate);
		_nextUpdateDate.AdvanceSecond(2);
	}
}

const char* World::GetID()
{
	return "WORLD";
}

void World::ForceNextUpdate()
{
	_nextUpdateDate.SetDate(&_currentDate);
	_nextUpdateDate.AdvanceSecond(-1);
}

Date& World::GetCurrentDate()
{
	return _currentDate;
}

EventScheduler& World::GetEventScheduler()
{
	return _eventScheduler;
}

PlotGenerator& World::GetPlotGenerator()
{
	return _plotGenerator;
}

BTree<VLocation*>& World::GetVLocations()
{
	return _vlocations;
}

BTree<Company*>& World::GetCompanies()
{
	return _companies;
}

BTree<Computer*>& World::GetComputers()
{
	return _computers;
}

DArray<char*>& World::GetPasswords()
{
	return _passwords;
}

DArray<GatewayDef*>& World::GetGatewayDefs()
{
	return _gatewayDefs;
}

Player& World::GetPlayer()
{
	const auto ret = _people.LookupTree("PLAYER");
	UplinkAssert(ret != nullptr);
	return *dynamic_cast<Player*>(ret->Data);
}

VLocation* World::GetVLocation(const char* ip)
{
	const auto tree = _vlocations.LookupTree(ip);

	if (tree == nullptr)
		return nullptr;

	return tree->Data;
}

Computer* World::GetComputer(const char* name)
{
	const auto tree = _computers.LookupTree(name);

	if (tree == nullptr)
		return nullptr;

	return tree->Data;
}

Company* World::GetCompany(const char* name)
{
	struct BTree<Company*>* tree = _companies.LookupTree(name);
	
	if (tree == nullptr)
		return nullptr;

	return tree->Data;
}

Person* World::GetPerson(const char* name)
{
	if (_people.LookupTree("PLAYER") != nullptr && strcmp(name, GetPlayer().GetHandle()) == 0)
		return &GetPlayer();

	const auto tree = _people.LookupTree(name);

	if (tree == nullptr)
		return nullptr;

	return tree->Data;
}

Person* World::CreatePerson(const char* name, const char* host)
{
	const auto person = new Person();
	_people.PutData(name, person);
	person->SetName(name);
	person->SetLocalHost(host);
	person->SetRemoteHost(host);
	return person;
}

void World::CreatePerson(Person* person)
{
	UplinkAssert(person != 0);
	_people.PutData(person->GetName(), person);
}

Company* World::CreateCompany(const char* name)
{
	const auto company = new Company();
	_companies.PutData(name, company);
	company->SetName(name);
	return company;
}

void World::CreateCompany(Company* company)
{
	UplinkAssert(company != nullptr);
	_companies.PutData(company->GetName(), company);
}

VLocation* World::CreateVLocation(const char* ip, int x, int y)
{
	const auto vlocation = new VLocation();
	_vlocations.PutData(ip, vlocation);
	vlocation->SetIp(ip);
	vlocation->SetPLocation(x, y);
	return vlocation;
}

void World::CreateVLocation(VLocation* vlocation)
{
	UplinkAssert(vlocation != nullptr);
	_vlocations.PutData(vlocation->GetIp(), vlocation);
}

Computer* World::CreateComputer(const char* computerName, const char* companyName, const char* ip)
{
	const auto computer = new Computer();
	_computers.PutData(computerName, computer);
	computer->SetName(computerName);
	computer->SetCompanyName(companyName);
	computer->SetIP(ip);

	const auto vlocation = World::GetVLocation(ip);
	UplinkAssert(vlocation != nullptr);
	vlocation->SetComputerName(computerName);

	return computer;
}

void World::CreateComputer(Computer* computer)
{
	UplinkAssert(computer != nullptr);
	_computers.PutData(computer->GetName(), computer);

	const auto vlocation = GetVLocation(computer->GetIp());
	UplinkAssert(vlocation != nullptr);
	vlocation->SetComputerName(computer->GetName());
}

void World::CreatePassword(const char* password)
{
	char* str = new char[strlen(password) + 1];
	strcpy(str, password);
	_passwords.PutData(str);
}

void World::CreateGatewayDef(GatewayDef* gatewayDef)
{
	if (!gatewayDef->VerifyCorrectness())
		return;

	_gatewayDefs.PutData(gatewayDef);
	_gatewayDefs.Sort(GatewayDef::GatewayDefComparator);
}
