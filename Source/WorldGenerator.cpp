#include <WorldGenerator.hpp>

#include <Globals.hpp>
#include <Image.hpp>
#include <RedShirt.hpp>

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
