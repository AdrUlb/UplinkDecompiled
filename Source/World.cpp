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

void World::CreateCompany(const char* name)
{
	const auto company = new Company();
	companies.PutData(name, company);
	company->SetName(name);
}
