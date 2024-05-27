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
	WorldGenerator::GenerateSpecifics();
	WorldGenerator::GeneratePlayer("NEWAGENT");
	WorldGenerator::GenerateRandomWorld();
	WorldGenerator::LoadDynamics();
}

void WorldGenerator::GenerateSpecifics()
{
	game->GetWorld()->CreateCompany("Player");
	WorldGenerator::GenerateLocalMachine();
	WorldGenerator::GenerateCompanyGovernment();
	WorldGenerator::GenerateGlobalCriminalDatabase();
	WorldGenerator::GenerateInternationalSocialSecurityDatabase();
	WorldGenerator::GenerateInternationalAcademicDatabase();
	WorldGenerator::GenerateInterNIC();
	WorldGenerator::GenerateStockMarket();
	WorldGenerator::GenerateProtoVision();
	WorldGenerator::GenerateOCP();
	WorldGenerator::GenerateSJGames();
	WorldGenerator::GenerateIntroversion();
	WorldGenerator::GenerateCompanyUplink();
}

void WorldGenerator::GeneratePlayer(const char* handle)
{
	puts("TODO: implement WorldGenerator::GeneratePlayer");
	/*const auto player = new Player();
	player->SetName(name, "PLAYER");
	player->SetAge(21);
	player->SetHandle(handle);
	player->SetLocalHost("127.0.0.1");
	player->SetIsTargetable(false);
	player->SetUplinkRating(0);
	player->SetNeuromancerRating(5);
	player->SetCreditRating(10);
	player->SetCurrentAccount(0);
	player->gateway.GiveStartingHardware();
	player->gateway.GiveStartingSoftware();
	game->GetWorld()->CreatePerson(player);
	player->GetConnection()->Reset();
	player->GetConnection()->AddVLocation("234.773.0.666");
	player->GetConnection()->Connect();
	player->GiveLink("234.773.0.666");
	player->GiveLink("458.615.48.651");*/
}

void WorldGenerator::GenerateRandomWorld()
{
	puts("TODO: implement WorldGenerator::GenerateRandomWorld");
}

void WorldGenerator::LoadDynamics()
{
	puts("TODO: implement WorldGenerator::LoadDynamics");
}

void WorldGenerator::GenerateSimpleStartingMissionA()
{
	puts("TODO: implement WorldGenerator::GenerateSimpleStartingMissionA()");
}

void WorldGenerator::GenerateSimpleStartingMissionB()
{
	puts("TODO: implement WorldGenerator::GenerateSimpleStartingMissionB()");
}
void WorldGenerator::GenerateLocalMachine()
{
	puts("TODO: implement WorldGenerator::GenerateLocalMachine()");
}

void WorldGenerator::GenerateCompanyGovernment()
{
	puts("TODO: implement WorldGenerator::GenerateCompanyGovernment()");
}

void WorldGenerator::GenerateGlobalCriminalDatabase()
{
	puts("TODO: implement WorldGenerator::GenerateGlobalCriminalDatabase()");
}

void WorldGenerator::GenerateInternationalSocialSecurityDatabase()
{
	puts("TODO: implement WorldGenerator::GenerateInternationalSocialSecurityDatabase()");
}

void WorldGenerator::GenerateInternationalAcademicDatabase()
{
	puts("TODO: implement WorldGenerator::GenerateInternationalAcademicDatabase()");
}

void WorldGenerator::GenerateInterNIC()
{
	puts("TODO: implement WorldGenerator::GenerateInterNIC()");
}

void WorldGenerator::GenerateStockMarket()
{
	puts("TODO: implement WorldGenerator::GenerateStockMarket()");
}

void WorldGenerator::GenerateProtoVision()
{
	puts("TODO: implement WorldGenerator::GenerateProtoVision()");
}

void WorldGenerator::GenerateOCP()
{
	puts("TODO: implement WorldGenerator::GenerateOCP()");
}

void WorldGenerator::GenerateSJGames()
{
	puts("TODO: implement WorldGenerator::GenerateSJGames()");
}

void WorldGenerator::GenerateIntroversion()
{
	puts("TODO: implement WorldGenerator::GenerateIntroversion()");
}

void WorldGenerator::GenerateCompanyUplink()
{
	puts("TODO: implement WorldGenerator::GenerateCompanyUplink()");
}
