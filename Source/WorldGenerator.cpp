#include <WorldGenerator.hpp>

#include <Globals.hpp>
#include <Image.hpp>
#include <RedShirt.hpp>
#include <fstream>

static Image* worldmapmask;

void WorldGenerator::Initialise()
{
	worldmapmask = new Image();
	const auto filePath =
		app->GetOptions()->ThemeFilename(game->GetWorldMapType() == 1 ? "worldmaplarge_mask_defcon.tif" : "worldmaplarge_mask_new.tif");

	const auto path = RsArchiveFileOpen(filePath);

	worldmapmask->LoadTIF(path);
	worldmapmask->Scale(594, 315);
	worldmapmask->FlipAroundH();

	delete[] filePath;
}

void WorldGenerator::Shutdown()
{
	if (worldmapmask != nullptr)
	{
		delete worldmapmask;
		worldmapmask = nullptr;
	}
}

void WorldGenerator::ReplaceInvalidCompanyAdmins()
{
	UplinkAbort("TODO: implement WorldGenerator::ReplaceInvalidCompanyAdmins()");
}

void WorldGenerator::UpdateSoftwareUpgrades()
{
	UplinkAbort("TODO: implement WorldGenerator::UpdateSoftwareUpgrades()");
}

void WorldGenerator::LoadDynamicsGatewayDefs()
{
	game->Game::GetWorld()->gatewayDefs.Size();
	const auto defFiles = ListDirectory("data/gateways/", ".txt");
	for (auto i = 0; i < defFiles->Size(); i++)
	{
		if (!defFiles->ValidIndex(i))
			continue;

		const auto filePath = defFiles->GetData(i);
		const auto rax_3 = RsArchiveFileOpen(filePath);
		UplinkAssert(rax_3 != nullptr);

		UplinkAbort("TODO: implement WorldGenerator::LoadDynamicsGatewayDefs()");
	}
	DeleteDArrayDataD(defFiles);
	delete defFiles;
}

void WorldGenerator::GenerateAll()
{
	puts("TODO: implement WorldGenerator::GenerateAll()");
}

void WorldGenerator::GenerateSimpleStartingMissionA()
{
	puts("TODO: implement WorldGenerator::GenerateSimpleStartingMissionA()");
}

void WorldGenerator::GenerateSimpleStartingMissionB()
{
	puts("TODO: implement WorldGenerator::GenerateSimpleStartingMissionB()");
}