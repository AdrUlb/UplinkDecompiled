#include <WorldGenerator.hpp>

#include <ComputerScreens/DialogScreen.hpp>
#include <ComputerScreens/DisconnectedScreen.hpp>
#include <ComputerScreens/LinksScreen.hpp>
#include <ComputerScreens/MessageScreen.hpp>
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
	(void)handle;
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
	const auto vlocation = game->GetWorld()->CreateVLocation("127.0.0.1", 284, 73);
	const auto computer = game->GetWorld()->CreateComputer("Gateway", "Player", "127.0.0.1");
	vlocation->SetListed(false);
	computer->SetIsTargetable(false);

	const auto linksScreen = new LinksScreen();
	linksScreen->SetScreenType(2);
	linksScreen->SetMainTitle("Gateway");
	linksScreen->SetSubTitle("Click on a link to quick-connect to that site");
	computer->AddComputerScreen(linksScreen, 0);

	const auto disconnectedByGatewayScreen = new DisconnectedScreen();
	disconnectedByGatewayScreen->SetMainTitle("");
	disconnectedByGatewayScreen->SetSubTitle("");
	disconnectedByGatewayScreen->SetTextMessage("Connection terminated by Gateway");
	disconnectedByGatewayScreen->SetNextPage(0);
	computer->AddComputerScreen(disconnectedByGatewayScreen, 1);

	const auto disconnectedByRemoteHostScreen = new DisconnectedScreen();
	disconnectedByRemoteHostScreen->SetMainTitle("");
	disconnectedByRemoteHostScreen->SetSubTitle("");
	disconnectedByRemoteHostScreen->SetTextMessage("Connection terminated by remote host");
	disconnectedByRemoteHostScreen->SetNextPage(0);
	computer->AddComputerScreen(disconnectedByRemoteHostScreen, 2);

	const auto logonCompletedScreen = new DialogScreen();
	logonCompletedScreen->SetMainTitle("Gateway");
	logonCompletedScreen->SetSubTitle("Log on complete");
	logonCompletedScreen->AddWidget("text2", 2, 170, 210, 270, 15, "Welcome back to your Gateway", "");
	logonCompletedScreen->AddWidget("okbutton", 5, 300, 350, 50, 20, "OK", "Click to continue", 0, 0, nullptr, nullptr);
	computer->AddComputerScreen(logonCompletedScreen, 3);

	computer->AddComputerScreen(new DialogScreen(), 5);

	const auto osStartedScreen = new DialogScreen();
	osStartedScreen->AddWidget("text2", 2, 100, 210, 340, 15, "UPLINK Operating System successfully started on Gateway", "");
	osStartedScreen->AddWidget("okbutton", 5, 300, 350, 50, 20, "OK", "Click to continue", 7, 0, nullptr, nullptr);
	computer->AddComputerScreen(osStartedScreen, 6);

	const auto setupCompleteScreen = new DialogScreen();
	setupCompleteScreen->SetMainTitle("Gateway");
	setupCompleteScreen->SetSubTitle("Set up complete");
	setupCompleteScreen->AddWidget(
		"caption", 2, 50, 120, 400, 280,
		"Your Gateway computer has been successfully set up and will accept only your username and password in the future.  "
		"When you next start Uplink on your home computer, you will be connected automatically to this Gateway.\n"
		"\n"
		"In your email you will find a message from Uplink Corporation, with some useful IP addresses.\n"
		"You will also find the Uplink Test Mission, which will allow you to practice hacking.\n"
		"\n"
		"WOULD YOU LIKE TO RUN THE TUTORIAL?\n(Recommended for first time users)",
		"");
	setupCompleteScreen->AddWidget("runtutorial", 8, 300, 350, 50, 20, "Yes", "Run the tutorial", 43, 0, nullptr, nullptr);
	setupCompleteScreen->AddWidget("dontruntutorial", 5, 200, 350, 50, 20, "No", "Skip the tutorial and return to the main screen", 0, 0, nullptr,
								   nullptr);
	computer->AddComputerScreen(setupCompleteScreen, 7);

	const auto systemNotFoundScreen = new MessageScreen();
	systemNotFoundScreen->SetMainTitle("Error 404");
	systemNotFoundScreen->SetSubTitle("System not found");
	systemNotFoundScreen->SetTextMessage(
		"The IP address you specified did not \nhave a valid computer host.\n\nThe system may be temporarily out of action\nand may return shortly.");
	systemNotFoundScreen->SetButtonMessage("OK");
	systemNotFoundScreen->SetNextPage(0);
	computer->AddComputerScreen(systemNotFoundScreen, 8);

	const auto externalConnectionsDeniedScreen = new MessageScreen();
	externalConnectionsDeniedScreen->SetMainTitle("Failed to establish connection");
	externalConnectionsDeniedScreen->SetSubTitle("External connections not permitted");
	externalConnectionsDeniedScreen->SetTextMessage("The computer you tried to access will not accept external connections.\n"
													"Only connections from trusted internal systems will be allowed to connect.\n"
													"\n"
													"If you wish to establish a connection to this system,\n"
													"you must first route your connection through a trusted system owned by the same company.");
	externalConnectionsDeniedScreen->SetButtonMessage("OK");
	externalConnectionsDeniedScreen->SetNextPage(0);
	computer->AddComputerScreen(externalConnectionsDeniedScreen, 9);

	const auto gatewayUpgradeCompleteScreen = new MessageScreen();
	gatewayUpgradeCompleteScreen->SetMainTitle("Gateway");
	gatewayUpgradeCompleteScreen->SetSubTitle("Upgrade complete");
	gatewayUpgradeCompleteScreen->SetTextMessage(
		"Congratulations agent - your are now using your new gateway and your old one is already being dismantled.\n"
		"\n"
		"If you view your hardware profile you should see the new gateway in place.\n"
		"If you experience any problems with this new system, please get in contact with us at\n"
		"Problems@UplinkCorporation.net.");
	gatewayUpgradeCompleteScreen->SetButtonMessage("OK");
	gatewayUpgradeCompleteScreen->SetNextPage(0);
	computer->AddComputerScreen(gatewayUpgradeCompleteScreen, 10);
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
