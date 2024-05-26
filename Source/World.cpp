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

void World::ForceNextUpdate()
{
	nextUpdateDate.SetDate(&currentDate);
	nextUpdateDate.AdvanceSecond(-1);
}
