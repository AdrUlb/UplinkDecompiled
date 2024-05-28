#include <World.hpp>

#include <MissionGenerator.hpp>
#include <NameGenerator.hpp>
#include <WorldGenerator.hpp>

World::World()
{
	currentDate.SetDate(0, 0, 0, 24, 2, 2010);
	currentDate.Activate();
	WorldGenerator::Initialise();
	MissionGenerator::Initialise();
	passwords.SetStepSize(100);
}

World::~World()
{
	WorldGenerator::Shutdown();
	NameGenerator::Shutdown();
	MissionGenerator::Shutdown();

	DeleteBTreeData(&vlocations);
	DeleteBTreeData(&companies);
	DeleteBTreeData(&computers);
	DeleteBTreeData(&people);
	DeleteDArrayDataD(&passwords);

	for (auto i = 0; i < gatewayDefs.Size(); i++)
	{
		if (!gatewayDefs.ValidIndex(i))
			continue;

		const auto gatewayDef = gatewayDefs.GetData(i);

		if (gatewayDef != nullptr)
			delete gatewayDef;
	}
}

bool World::Load(FILE* file)
{
	if (!currentDate.Load(file))
		return false;

	if (!eventScheduler.Load(file))
		return false;

	if (!plotGenerator.Load(file))
		return false;

	if (!demoPlotGenerator.Load(file))
		return false;

	if (!LoadBTree(reinterpret_cast<BTree<UplinkObject*>*>(&vlocations), file))
		return false;

	if (!LoadBTree(reinterpret_cast<BTree<UplinkObject*>*>(&companies), file))
		return false;

	if (!LoadBTree(reinterpret_cast<BTree<UplinkObject*>*>(&computers), file))
		return false;

	if (!LoadBTree(reinterpret_cast<BTree<UplinkObject*>*>(&people), file))
		return false;

	WorldGenerator::ReplaceInvalidCompanyAdmins();
	WorldGenerator::UpdateSoftwareUpgrades();

	return true;
}

void World::Save(FILE* file)
{
	currentDate.Save(file);
	eventScheduler.Save(file);
	plotGenerator.Save(file);
	demoPlotGenerator.Save(file);
	SaveBTree(reinterpret_cast<BTree<UplinkObject*>*>(&vlocations), file);
	SaveBTree(reinterpret_cast<BTree<UplinkObject*>*>(&companies), file);
	SaveBTree(reinterpret_cast<BTree<UplinkObject*>*>(&computers), file);
	SaveBTree(reinterpret_cast<BTree<UplinkObject*>*>(&people), file);
	SaveID_END(file);
}

void World::Print()
{
	puts("============== W O R L D ===================================");
	currentDate.Print();
	eventScheduler.Print();
	plotGenerator.Print();
	demoPlotGenerator.Print();
	PrintBTree(reinterpret_cast<BTree<UplinkObject*>*>(&vlocations));
	PrintBTree(reinterpret_cast<BTree<UplinkObject*>*>(&companies));
	PrintBTree(reinterpret_cast<BTree<UplinkObject*>*>(&computers));
	PrintBTree(reinterpret_cast<BTree<UplinkObject*>*>(&people));
	puts("============== E N D  O F  W O R L D =======================");
}

void World::Update()
{
	currentDate.Update();
	if (currentDate.After(&nextUpdateDate) != 0)
	{
		UpdateBTree(reinterpret_cast<BTree<UplinkObject*>*>(&vlocations));
		UpdateBTree(reinterpret_cast<BTree<UplinkObject*>*>(&companies));
		UpdateBTree(reinterpret_cast<BTree<UplinkObject*>*>(&computers));
		UpdateBTree(reinterpret_cast<BTree<UplinkObject*>*>(&people));
		eventScheduler.Update();
		nextUpdateDate.SetDate(&currentDate);
		nextUpdateDate.AdvanceSecond(2);
	}
}

const char* World::GetID()
{
	return "WORLD";
}

void World::ForceNextUpdate()
{
	nextUpdateDate.SetDate(&currentDate);
	nextUpdateDate.AdvanceSecond(-1);
}

Player* World::GetPlayer()
{
	const auto ret = people.LookupTree("PLAYER");
	UplinkAssert(ret != nullptr);
	return dynamic_cast<Player*>(ret->Data);
}

VLocation* World::GetVLocation(const char* ip)
{
	const auto tree = vlocations.LookupTree(ip);

	if (tree == nullptr)
		return nullptr;

	return tree->Data;
}

Computer* World::GetComputer(const char* name)
{
	const auto tree = computers.LookupTree(name);

	if (tree == nullptr)
		return nullptr;

	return tree->Data;
}

Company* World::GetCompany(const char* name)
{
	struct BTree<Company*>* tree = companies.LookupTree(name);
	if (tree == nullptr)
		return nullptr;
	return tree->Data;
}

Person* World::CreatePerson(const char* name, const char* host)
{
	const auto person = new Person();
	people.PutData(name, person);
	person->SetName(name);
	person->SetLocalHost(host);
	person->SetRemoteHost(host);
	return person;
}

Company* World::CreateCompany(const char* name)
{
	const auto company = new Company();
	companies.PutData(name, company);
	company->SetName(name);
	return company;
}

void World::CreateCompany(Company* company)
{
	UplinkAssert(company != nullptr);
	companies.PutData(company->name, company);
}

VLocation* World::CreateVLocation(const char* ip, int x, int y)
{
	const auto vlocation = new VLocation();
	vlocations.PutData(ip, vlocation);
	vlocation->SetIP(ip);
	vlocation->SetPLocation(x, y);
	return vlocation;
}

void World::CreateVLocation(VLocation* vlocation)
{
	UplinkAssert(vlocation != nullptr);
	vlocations.PutData(vlocation->ip, vlocation);
}

Computer* World::CreateComputer(const char* computerName, const char* companyName, const char* ip)
{
	const auto computer = new Computer();
	computers.PutData(computerName, computer);
	computer->SetName(computerName);
	computer->SetCompanyName(companyName);
	computer->SetIP(ip);

	const auto vlocation = World::GetVLocation(ip);
	UplinkAssert(vlocation != nullptr);
	vlocation->SetComputer(computerName);

	return computer;
}

void World::CreateComputer(Computer* computer)
{
	UplinkAssert(computer != nullptr);
	computers.PutData(computer->name, computer);

	const auto vlocation = GetVLocation(computer->ip);
	UplinkAssert(vlocation != nullptr);
	vlocation->SetComputer(computer->name);
}
