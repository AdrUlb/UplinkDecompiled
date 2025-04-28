#include <WorldGenerator.hpp>

#include <ComputerScreens/CypherScreen.hpp>
#include <ComputerScreens/DialogScreen.hpp>
#include <ComputerScreens/DisconnectedScreen.hpp>
#include <ComputerScreens/GenericScreen.hpp>
#include <ComputerScreens/HighSecurityScreen.hpp>
#include <ComputerScreens/LinksScreen.hpp>
#include <ComputerScreens/LogScreen.hpp>
#include <ComputerScreens/MenuScreen.hpp>
#include <ComputerScreens/MessageScreen.hpp>
#include <ComputerScreens/PasswordScreen.hpp>
#include <ComputerScreens/UserIDScreen.hpp>
#include <Globals.hpp>
#include <Image.hpp>
#include <LanGenerator.hpp>
#include <NameGenerator.hpp>
#include <NumberGenerator.hpp>
#include <RedShirt.hpp>
#include <fstream>

static Image* worldmapmask;

void WorldGenerator::Initialise()
{
	worldmapmask = new Image();
	const auto filePath = app->GetOptions().ThemeFilename(game->GetWorldMapType() == 1 ? "worldmaplarge_mask_defcon.tif" : "worldmaplarge_mask_new.tif");

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
	puts("TODO: implement WorldGenerator::ReplaceInvalidCompanyAdmins()");
}

void WorldGenerator::UpdateSoftwareUpgrades()
{
	puts("TODO: implement WorldGenerator::UpdateSoftwareUpgrades()");
}

void WorldGenerator::LoadDynamicsGatewayDefs()
{
	game->Game::GetWorld().GetGatewayDefs().Size();
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
	GenerateSpecifics();
	GeneratePlayer("NEWAGENT");
	GenerateRandomWorld();
	LoadDynamics();
}

#include <string>

void WorldGenerator::GenerateSpecifics()
{
	game->GetWorld().CreateCompany("Player");
	GenerateLocalMachine();
	GenerateCompanyGovernment();
	GenerateGlobalCriminalDatabase();
	GenerateInternationalSocialSecurityDatabase();
	GenerateInternationalAcademicDatabase();
	GenerateInterNIC();
	GenerateStockMarket();
	GenerateProtoVision();
	GenerateOCP();
	GenerateSJGames();
	GenerateIntroversion();
	GenerateCompanyUplink();
}

void WorldGenerator::GeneratePlayer(const char* handle)
{
	(void)handle;
	const auto player = new Player();
	player->SetName("PLAYER");
	player->SetAge(21);
	player->SetHandle(handle);
	player->SetLocalHost("127.0.0.1");
	player->SetIsTargetable(false);
	player->GetRating().SetUplinkRating(0);
	player->GetRating().SetNeuromancerRating(5);
	player->GetRating().SetCreditRating(10);
	player->SetCurrentAccount(0);
	/*player->gateway.GiveStartingHardware();
	player->gateway.GiveStartingSoftware();*/
	game->GetWorld().CreatePerson(player);
	player->GetConnection().Reset();
	player->GetConnection().AddVLocation("234.773.0.666");
	player->GetConnection().Connect();
	player->GiveLink("234.773.0.666");
	player->GiveLink("458.615.48.651");

	puts("TODO: implement WorldGenerator::GeneratePlayer()");
}

void WorldGenerator::GenerateRandomWorld()
{
	puts("TODO: implement WorldGenerator::GenerateRandomWorld()");
}

void WorldGenerator::LoadDynamics()
{
	puts("TODO: implement WorldGenerator::LoadDynamics()");
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
	const auto vlocation = game->GetWorld().CreateVLocation("127.0.0.1", 284, 73);
	const auto computer = game->GetWorld().CreateComputer("Gateway", "Player", "127.0.0.1");
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
	setupCompleteScreen->AddWidget("caption", 2, 50, 120, 400, 280,
		"Your Gateway computer has been successfully set up and will accept only your username and password in the future.  "
		"When you next start Uplink on your home computer, you will be connected automatically to this Gateway.\n"
		"\n"
		"In your email you will find a message from Uplink Corporation, with some useful IP addresses.\n"
		"You will also find the Uplink Test Mission, which will allow you to practice hacking.\n"
		"\n"
		"WOULD YOU LIKE TO RUN THE TUTORIAL?\n(Recommended for first time users)",
		"");
	setupCompleteScreen->AddWidget("runtutorial", 8, 300, 350, 50, 20, "Yes", "Run the tutorial", 43, 0, nullptr, nullptr);
	setupCompleteScreen->AddWidget("dontruntutorial", 5, 200, 350, 50, 20, "No", "Skip the tutorial and return to the main screen", 0, 0, nullptr, nullptr);
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
	gatewayUpgradeCompleteScreen->SetTextMessage("Congratulations agent - your are now using your new gateway and your old one is already being dismantled.\n"
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
	GenerateCompany("Government", 5, 1, 10, -20);
}

void WorldGenerator::GenerateGlobalCriminalDatabase()
{
	int x, y;
	WorldGenerator::GenerateValidMapPos(x, y);
	game->GetWorld().CreateVLocation("785.234.87.124", x, y);

	const auto computer = new Computer();
	computer->SetTYPE(0);
	computer->SetName("Global Criminal Database");
	computer->SetCompanyName("Government");
	computer->SetIP("785.234.87.124");
	computer->SetTraceSpeed(10);
	computer->SetTraceAction(9);
	computer->SetIsTargetable(0);
	computer->GetSecurity().AddSystem(1, 1, -1);
	computer->GetSecurity().AddSystem(4, 1, -1);
	game->GetWorld().CreateComputer(computer);

	const auto infoScreen = new MessageScreen();
	infoScreen->SetMainTitle("Global Criminal Database");
	infoScreen->SetSubTitle("Unauthorised Access will be punished");
	infoScreen->SetTextMessage("This is a Government owned computer system - all attempts at\n"
							   "illegal access will be severely punished to the full extent of the law.\n"
							   "\n"
							   "An Elliptic-Curve encryption cypher will be required for full system access.\n"
							   "\n"
							   "Valid User Accounts\n"
							   "====================\n"
							   "\n"
							   "admin     - Complete system wide access\n"
							   "readwrite  - Read/Write access to all records\n"
							   "readonly   - Read only access to all records\n");
	infoScreen->SetButtonMessage("Log in");
	infoScreen->SetNextPage(1);
	computer->AddComputerScreen(infoScreen, 0);

	const auto highSecurityScreen = new HighSecurityScreen();
	highSecurityScreen->SetMainTitle("Global Criminal Database");
	highSecurityScreen->SetSubTitle("Authorisation required");
	highSecurityScreen->AddSystem("UserID / password verification", 2);
	highSecurityScreen->AddSystem("Elliptic-Curve Encryption Cypher", 3);
	highSecurityScreen->SetNextPage(4);
	computer->AddComputerScreen(highSecurityScreen, 1);

	const auto loginScreen = new UserIDScreen();
	loginScreen->SetMainTitle("Global Criminal Database");
	loginScreen->SetSubTitle("Log in");
	loginScreen->SetDifficulty(180);
	loginScreen->SetNextPage(1);
	computer->AddComputerScreen(loginScreen, 2);

	const auto cypherScreen = new CypherScreen();
	cypherScreen->SetMainTitle("Global Criminal Database");
	cypherScreen->SetSubTitle("Enter elliptic-curve encryption cypher");
	cypherScreen->SetDifficulty(180);
	cypherScreen->SetNextPage(1);
	computer->AddComputerScreen(cypherScreen, 3);

	const auto menuScreen = new MenuScreen();
	menuScreen->SetMainTitle("Global Criminal Database");
	menuScreen->SetSubTitle("Main Menu");
	menuScreen->AddOption("Search Records", "", 7, 10, -1);
	menuScreen->AddOption("View Logs", "", 6, 1, -1);
	menuScreen->AddOption("Security", "", 9, 1, -1);
	computer->AddComputerScreen(menuScreen, 4);

	const auto screen_1 = new GenericScreen();
	screen_1->SetScreenType(11);
	screen_1->SetMainTitle("Global Criminal Database");
	screen_1->SetSubTitle("Criminal Records");
	screen_1->SetNextPage(4);
	computer->AddComputerScreen(screen_1, 5);

	const auto logsScreen = new LogScreen();
	logsScreen->SetTARGET(0);
	logsScreen->SetMainTitle("Global Criminal Database");
	logsScreen->SetSubTitle("Access logs");
	logsScreen->SetNextPage(4);
	computer->AddComputerScreen(logsScreen, 6);

	const auto searchScreen = new DialogScreen();
	searchScreen->SetMainTitle("Global Criminal Database");
	searchScreen->SetSubTitle("Search for Criminal Record");
	searchScreen->AddWidget("name", 3, 150, 200, 200, 15, "Enter name here", "Type the name of the person to search for here");
	searchScreen->AddWidget("OK", 8, 150, 240, 80, 15, "OK", "Click here when done", 15, -1, nullptr, nullptr);
	searchScreen->AddWidget("Cancel", 5, 270, 240, 80, 15, "Cancel", "Click here to return to the menu", 4, -1, nullptr, nullptr);
	searchScreen->AddWidget("border1", 1, 120, 170, 260, 4, "", "");
	searchScreen->AddWidget("border2", 1, 120, 266, 260, 4, "", "");
	searchScreen->AddWidget("border3", 1, 120, 170, 4, 100, "", "");
	searchScreen->AddWidget("border4", 1, 380, 170, 4, 100, "", "");
	searchScreen->SetReturnKeyButton("OK");
	searchScreen->SetEscapeKeyButton("Cancel");
	computer->AddComputerScreen(searchScreen, 7);

	const auto screen_4 = new GenericScreen();
	screen_4->SetScreenType(16);
	screen_4->SetMainTitle("Global Criminal Database");
	screen_4->SetSubTitle("View Criminal Records");
	screen_4->SetNextPage(4);
	computer->AddComputerScreen(screen_4, 8);

	const auto screen_5 = new GenericScreen();
	screen_5->SetScreenType(0x11);
	screen_5->SetMainTitle("Global Criminal Database");
	screen_5->SetSubTitle("Security settings");
	screen_5->SetNextPage(4);
	computer->AddComputerScreen(screen_5, 9);

	const auto adminUser = new Record();
	adminUser->AddField("Name", "admin");
	NameGenerator::GenerateComplexPassword();
	adminUser->AddField("Password", tempname);
	adminUser->AddField("Security", "1");
	computer->GetRecordBank().AddRecord(*adminUser);

	const auto readwriteUser = new Record();
	readwriteUser->AddField("Name", "readwrite");
	NameGenerator::GenerateComplexPassword();
	readwriteUser->AddField("Password", tempname);
	readwriteUser->AddField("Security", "2");
	computer->GetRecordBank().AddRecord(*readwriteUser);

	const auto readonlyUser = new Record();
	readonlyUser->AddField("Name", "readonly");
	readonlyUser->AddField("Password", NameGenerator::GeneratePassword());
	readonlyUser->AddField("Security", "4");
	computer->GetRecordBank().AddRecord(*readonlyUser);
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
	int x, y;
	WorldGenerator::GenerateValidMapPos(x, y);
	game->GetWorld().CreateVLocation("458.615.48.651", x, y);

	const auto computer = new Computer();
	computer->SetTYPE(0);
	computer->SetName("InterNIC");
	computer->SetCompanyName("Government");
	computer->SetIP("458.615.48.651");
	computer->SetTraceSpeed(0xf);
	computer->SetTraceAction(3);
	computer->SetIsTargetable(0);
	game->GetWorld().CreateComputer(computer);

	const auto welcomeScreen = new MessageScreen();
	welcomeScreen->SetMainTitle("InterNIC");
	welcomeScreen->SetSubTitle("InterNet Information Center");
	welcomeScreen->SetNextPage(1);
	welcomeScreen->SetTextMessage("Welcome to the Internet Information Center.\n\nUse of this service is free and open to all.");
	welcomeScreen->SetButtonMessage("OK");
	computer->AddComputerScreen(welcomeScreen, 0);

	const auto menuScreen = new MenuScreen();
	menuScreen->SetMainTitle("InterNIC");
	menuScreen->SetSubTitle("Main menu");
	menuScreen->AddOption("Browse/Search", "Click here to use a list of all known IP's", 2, 10, -1);
	menuScreen->AddOption("Admin", "Click here to modify the database", 3, 10, -1);
	computer->AddComputerScreen(menuScreen, 1);

	const auto listScreen = new LinksScreen();
	listScreen->SetMainTitle("InterNIC");
	listScreen->SetSubTitle("Search list");
	listScreen->SetScreenType(1);
	listScreen->SetNextPage(1);
	computer->AddComputerScreen(listScreen, 2);

	const auto passwordScreen = new PasswordScreen();
	passwordScreen->SetMainTitle("InterNIC");
	passwordScreen->SetSubTitle("Password authorisation required");
	passwordScreen->SetNextPage(4);
	passwordScreen->SetPassword(NameGenerator::GeneratePassword());
	passwordScreen->SetDifficulty(70);
	computer->AddComputerScreen(passwordScreen, 3);

	const auto adminMenuScreen = new MenuScreen();
	adminMenuScreen->SetMainTitle("InterNIC");
	adminMenuScreen->SetSubTitle("Admin menu");
	adminMenuScreen->AddOption("Access Logs", "Click here to view access logs", 5, 0xa, 0xffffffff);
	adminMenuScreen->AddOption("Exit", "Return to the main menu", 1, 0xa, 0xffffffff);
	computer->AddComputerScreen(adminMenuScreen, 4);

	const auto logScreen = new LogScreen();
	logScreen->SetMainTitle("InterNIC");
	logScreen->SetSubTitle("Log Screen");
	logScreen->SetTARGET(0);
	logScreen->SetNextPage(4);
	computer->AddComputerScreen(logScreen, 5);
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
	const auto company = new CompanyUplink();
	game->GetWorld().CreateCompany(company);
	GenerateUplinkPublicAccessServer();

	puts("TODO: implement WorldGenerator::GenerateCompanyUplink()");
}

Computer* WorldGenerator::GenerateComputer(const char* name)
{
	(void)name;
	puts("TODO: implement WorldGenerator::GenerateComputer()");
	return nullptr;
}

Company* WorldGenerator::GenerateCompany(const char* name, int size, int type, int growth, int alignment)
{
	const auto company = new Company();
	company->SetName(name);
	company->SetSize(size);
	company->SetTYPE(type);
	company->SetGrowth(growth);
	company->SetAlignment(alignment);
	game->GetWorld().CreateCompany(company);

	WorldGenerator::GeneratePublicAccessServer(name);
	WorldGenerator::GenerateInternalServicesMachine(name);
	WorldGenerator::GenerateCentralMainframe(name);
	WorldGenerator::GenerateLAN(name);

	const auto computerCount = size / 20;
	for (auto i = 0; i < computerCount; i++)
		WorldGenerator::GenerateComputer(name);

	NameGenerator::GeneratePublicAccessServerName(name);
	const auto computer = game->GetWorld().GetComputer(tempname);
	UplinkAssert(computer != nullptr);

	char s[0x80];
	UplinkSnprintf(s, 0x80, "internal@%s.net", name);
	game->GetWorld().CreatePerson(s, computer->GetIp())->SetIsTargetable(false);
	company->Grow(0);
	return company;
}

VLocation* WorldGenerator::GenerateLocation()
{
	int x, y;
	WorldGenerator::GenerateValidMapPos(x, y);

	int rax = NumberGenerator::RandomNumber(1000);
	int rax_1 = NumberGenerator::RandomNumber(1000);
	int rax_2 = NumberGenerator::RandomNumber(1000);

	char ip[0x18];
	UplinkSnprintf(ip, 0x18, "%d.%d.%d.%d", NumberGenerator::RandomNumber(0x3e8), rax_2, rax_1, rax);

	struct VLocation* vlocation = new VLocation();
	vlocation->SetPLocation(x, y);
	vlocation->SetIp(ip);
	game->GetWorld().CreateVLocation(vlocation);

	return vlocation;

	UplinkAbort("TODO: implement WorldGenerator::GenerateLocation()");
}

Computer* WorldGenerator::GeneratePublicAccessServer(const char* name)
{
	NameGenerator::GeneratePublicAccessServerName(name);
	char value[0x80];
	UplinkStrncpy(value, tempname, 0x80);

	const auto vlocation = WorldGenerator::GenerateLocation();

	const auto computer = new Computer();
	computer->SetTYPE(1);
	computer->SetName(value);
	computer->SetCompanyName(name);
	computer->SetTraceSpeed(NumberGenerator::RandomNormalNumber(-1.0f, -0.1f));
	computer->SetIP(vlocation->GetIp());

	const auto messageScreen = new MessageScreen();
	messageScreen->SetMainTitle(name);
	messageScreen->SetSubTitle("Public Access Server");
	messageScreen->SetTextMessage(value);
	messageScreen->SetButtonMessage("OK");
	messageScreen->SetNextPage(1);
	computer->AddComputerScreen(messageScreen, 0);

	const auto companyDataScreen = new GenericScreen();
	companyDataScreen->SetMainTitle(name);
	companyDataScreen->SetSubTitle("Company data");
	companyDataScreen->SetScreenType(28);
	computer->AddComputerScreen(companyDataScreen, 1);
	game->GetWorld().CreateComputer(computer);

	return computer;
}

Computer* WorldGenerator::GenerateInternalServicesMachine(const char* companyName)
{
	NameGenerator::GenerateInternalServicesServerName(companyName);

	char value[0x80];
	UplinkStrncpy(value, tempname, 0x80);

	const auto company = game->GetWorld().GetCompany(companyName);
	UplinkAssert(company != nullptr);

	const auto vlocation = WorldGenerator::GenerateLocation();
	const auto computer = new Computer();
	computer->SetTYPE(2);
	computer->SetName(value);
	computer->SetCompanyName(companyName);
	computer->SetTraceSpeed(NumberGenerator::RandomNormalNumber(15.0f, 1.5f));
	computer->SetIP(vlocation->GetIp());
	const auto companySize = company->GetSize();
	if (companySize > 1)
	{
		auto level = (companySize - 1) / 3;

		if (level == 0)
			level = 1;
		else if (level > 5)
			level = 5;

		computer->GetSecurity().AddSystem(4, level, -1);
	}

	if (companySize > 8)
	{
		auto level = ((companySize - 8) / 3);

		if (level == 0)
			level = 1;
		else if (level > 5)
			level = 5;

		computer->GetSecurity().AddSystem(1, level, -1);
	}

	if (companySize > 10)
	{
		auto level = (companySize - 10) / 3;

		if (level == 0)
			level = 1;
		else if (level > 5)
			level = 5;

		computer->GetSecurity().AddSystem(2, level, -1);
	}
	if (companySize <= 4)
		computer->SetTraceAction(3);
	else if (companySize <= 11)
		computer->SetTraceAction(5);
	else
		computer->SetTraceAction(9);

	if (strcmp(companyName, "Government") == 0)
		computer->SetIsTargetable(0);

	game->GetWorld().CreateComputer(computer);

	const auto loginScreen = new UserIDScreen();
	loginScreen->SetMainTitle(companyName);
	loginScreen->SetSubTitle("Log in");
	loginScreen->SetDifficulty(NumberGenerator::RandomNormalNumber(45.0f, 6.75f));
	loginScreen->SetNextPage(1);
	computer->AddComputerScreen(loginScreen, 0);

	const auto menuScreen = new MenuScreen();
	menuScreen->SetMainTitle(companyName);
	menuScreen->SetSubTitle("Internal Services Main Menu");
	computer->AddComputerScreen(menuScreen, 1);
	menuScreen->AddOption("File Server", "Access the file server", 2, 10, -1);
	menuScreen->AddOption("View records", "View the records stored on this system", 4, 10, -1);
	menuScreen->AddOption("View links", "View all links available on this system", 6, 10, -1);
	menuScreen->AddOption("Admin", "Enter administrative mode", 7, 1, -1);
	computer->AddComputerScreen(menuScreen, 1);

	const auto fileScreen = new GenericScreen();
	fileScreen->SetScreenType(6);
	fileScreen->SetMainTitle(companyName);
	fileScreen->SetSubTitle("File server");
	fileScreen->SetNextPage(1);
	computer->AddComputerScreen(fileScreen, 2);

	const auto logScreen = new LogScreen();
	logScreen->SetTARGET(0);
	logScreen->SetMainTitle(companyName);
	logScreen->SetSubTitle("Access Logs");
	logScreen->SetNextPage(7);
	computer->AddComputerScreen(logScreen, 3);

	const auto recordScreen = new GenericScreen();
	recordScreen->SetScreenType(11);
	recordScreen->SetMainTitle(companyName);
	recordScreen->SetSubTitle("Records");
	recordScreen->SetNextPage(1);
	computer->AddComputerScreen(recordScreen, 4);

	const auto securityScreen = new GenericScreen();
	securityScreen->SetScreenType(17);
	securityScreen->SetMainTitle(companyName);
	securityScreen->SetSubTitle("Security");
	securityScreen->SetNextPage(7);
	computer->AddComputerScreen(securityScreen, 5);

	const auto linksScreen = new LinksScreen();
	linksScreen->SetMainTitle(companyName);
	linksScreen->SetSubTitle("Links");
	linksScreen->SetNextPage(1);
	linksScreen->SetScreenType(3);
	computer->AddComputerScreen(linksScreen, 6);

	const auto adminScreen = new MenuScreen();
	adminScreen->SetMainTitle(companyName);
	adminScreen->SetSubTitle("Admin menu");
	adminScreen->AddOption("View logs", "View the access logs on this system", 3, 1, -1);
	adminScreen->AddOption("Security", "Enable and disable security systems", 5, 1, -1);
	adminScreen->AddOption("Console", "Run a console", 8, 1, -1);
	adminScreen->AddOption("Exit", "Return to the main menu", 1, 10, -1);
	computer->AddComputerScreen(adminScreen, 7);

	const auto consoleScreen = new GenericScreen();
	consoleScreen->SetScreenType(20);
	consoleScreen->SetMainTitle(companyName);
	consoleScreen->SetSubTitle("Console");
	consoleScreen->SetNextPage(7);
	computer->AddComputerScreen(consoleScreen, 8);
	computer->GetDataBank().SetSize(NumberGenerator::RandomNormalNumber(100.0f, 40.0f));

	const auto fileCount = NumberGenerator::RandomNormalNumber(10.0f, 5.0f);

	for (auto i = 0; i < fileCount; i++)
	{
		const auto type = NumberGenerator::RandomNumber(2) + 1;
		const auto size = NumberGenerator::RandomNormalNumber(6.0f, 6.0f);

		NameGenerator::GenerateDataName(companyName, type);
		const auto data = new Data();
		data->SetTitle(tempname);

		auto compressed = 0;
		auto encrypted = 0;
		if (NumberGenerator::RandomNumber(2) != 0)
			compressed = NumberGenerator::RandomNumber(5);

		if (NumberGenerator::RandomNumber(2))
			encrypted = NumberGenerator::RandomNumber(5);

		data->SetDetails(type, size, encrypted, compressed, 1.0f, 0);
		computer->GetDataBank().PutData(data);
	}

	const auto logCount = NumberGenerator::RandomNumber(10);

	for (auto i = 0; i < logCount; i++)
	{
		const auto log = new AccessLog();
		const auto loc = WorldGenerator::GetRandomLocation();
		log->SetProperties(game->GetWorld().GetCurrentDate(), loc->GetIp(), " ", 0, 1);
		log->SetData1("Accessed File");
		computer->GetLogBank().AddLog(log, -1);
	}

	const auto record = new Record();
	record->AddField("Name", "admin");
	record->AddField("Password", NameGenerator::GeneratePassword());
	record->AddField("Security", "1");
	computer->GetRecordBank().AddRecord(*record);

	return computer;
}

Computer* WorldGenerator::GenerateCentralMainframe(const char* companyName)
{
	NameGenerator::GenerateCentralMainframeName(companyName);

	char value[0x80];
	UplinkStrncpy(value, tempname, 0x80);

	const auto company = game->GetWorld().GetCompany(companyName);
	UplinkAssert(company != nullptr);

	const auto vlocation = WorldGenerator::GenerateLocation();
	vlocation->SetListed(false);

	const auto computer = new Computer();
	computer->SetTYPE(4);
	computer->SetName(value);
	computer->SetCompanyName(companyName);
	computer->SetIsExternallyOpen(false);
	computer->SetTraceSpeed(NumberGenerator::RandomNormalNumber(5.0f, 0.5f));
	computer->SetTraceAction(9);
	computer->SetIP(vlocation->GetIp());

	int level1 = (company->GetSize() - 1) / 3;
	if (level1 > 5)
	{
		level1 = 5;
	}
	else if (level1 < 1)
	{
		level1 = 1;
	}
	computer->GetSecurity().AddSystem(4, level1, -1);

	int level2 = (company->GetSize() - 8) / 3;
	if (level2 > 5)
	{
		level2 = 5;
	}
	else if (level2 < 1)
	{
		level2 = 1;
	}
	computer->GetSecurity().AddSystem(1, level2, -1);

	int level3 = (company->GetSize() - 10) / 3;
	if (level3 > 5)
	{
		level3 = 5;
	}
	else if (level3 < 1)
	{
		level3 = 1;
	}
	computer->GetSecurity().AddSystem(2, level3, -1);

	if (strcmp(companyName, "Government") == 0)
		computer->SetIsTargetable(false);

	game->GetWorld().CreateComputer(computer);

	const auto highSecurityScreen = new HighSecurityScreen();
	highSecurityScreen->SetMainTitle(companyName);
	highSecurityScreen->SetSubTitle("Authorisation required");
	highSecurityScreen->AddSystem("UserID / password verification", 1);
	highSecurityScreen->AddSystem("Voice Print Identification", 2);
	highSecurityScreen->SetNextPage(3);
	computer->AddComputerScreen(highSecurityScreen, 0);

	const auto loginScreen = new UserIDScreen();
	loginScreen->SetMainTitle(companyName);
	loginScreen->SetSubTitle("Log in");
	loginScreen->SetDifficulty(NumberGenerator::RandomNormalNumber(80.0f, 12.0f));
	loginScreen->SetNextPage(0);
	computer->AddComputerScreen(loginScreen, 1);

	const auto voiceRequiredScreen = new GenericScreen();
	voiceRequiredScreen->SetMainTitle(companyName);
	voiceRequiredScreen->SetSubTitle("Voice print analysis required");
	voiceRequiredScreen->SetScreenType(0x1b);
	voiceRequiredScreen->SetNextPage(0);
	computer->AddComputerScreen(voiceRequiredScreen, 2);

	const auto mainMenuScreen = new MenuScreen();
	mainMenuScreen->SetMainTitle(companyName);
	mainMenuScreen->SetSubTitle("Central Mainframe Main Menu");
	mainMenuScreen->AddOption(" File Server", "Access the file server", 4, 3, -1);
	mainMenuScreen->AddOption("View logs", "View the access logs on this system", 5, 1, -1);
	mainMenuScreen->AddOption("Console", "Use a console on this system", 6, 1, -1);
	computer->AddComputerScreen(mainMenuScreen, 3);

	const auto fileServerScreen = new GenericScreen();
	fileServerScreen->SetScreenType(6);
	fileServerScreen->SetMainTitle(companyName);
	fileServerScreen->SetSubTitle("File server");
	fileServerScreen->SetNextPage(3);
	computer->AddComputerScreen(fileServerScreen, 4);

	const auto logScreen = new LogScreen();
	logScreen->SetTARGET(0);
	logScreen->SetMainTitle(companyName);
	logScreen->SetSubTitle("Access Logs");
	logScreen->SetNextPage(3);
	computer->AddComputerScreen(logScreen, 5);

	const auto consoleScreen = new GenericScreen();
	consoleScreen->SetScreenType(20);
	consoleScreen->SetMainTitle(companyName);
	consoleScreen->SetSubTitle("Console");
	consoleScreen->SetNextPage(3);
	computer->AddComputerScreen(consoleScreen, 6);

	computer->GetDataBank().SetSize(NumberGenerator::RandomNormalNumber(100.0f, 40.0f));

	const auto fileCount = NumberGenerator::RandomNormalNumber(10.0f, 5.0f);

	for (auto i = 0; i < fileCount; i++)
	{
		const auto type = NumberGenerator::RandomNumber(2) + 1;
		const auto size = NumberGenerator::RandomNormalNumber(6.0f, 4.0f);

		NameGenerator::GenerateDataName(companyName, type);
		const auto data = new Data();
		data->SetTitle(tempname);

		auto compressed = 0;
		auto encrypted = 0;
		if (NumberGenerator::RandomNumber(2) != 0)
			compressed = NumberGenerator::RandomNumber(5);

		if (NumberGenerator::RandomNumber(2) != 0)
			encrypted = NumberGenerator::RandomNumber(5);

		data->SetDetails(type, size, encrypted, compressed, 1.0f, 0);
		computer->GetDataBank().PutData(data);
	}

	const auto logCount = NumberGenerator::RandomNumber(10);

	for (auto i = 0; i < logCount; i++)
	{
		const auto log = new AccessLog();
		const auto loc = WorldGenerator::GetRandomLocation();
		log->SetProperties(game->GetWorld().GetCurrentDate(), loc->GetIp(), " ", 0, 1);
		log->SetData1("Accessed File");
		computer->GetLogBank().AddLog(log, -1);
	}

	const auto record = new Record();
	record->AddField("Name", "admin");
	NameGenerator::GenerateComplexPassword();
	record->AddField("Password", tempname);
	record->AddField("Security", "1");
	computer->GetRecordBank().AddRecord(*record);

	return computer;
}

Computer* WorldGenerator::GenerateLAN(const char* companyName)
{
	const auto company = game->GetWorld().GetCompany(companyName);

	if (company == nullptr)
		return nullptr;

	if (company->GetSize() > 40)
		return LanGenerator::GenerateLAN(companyName, 4);

	return LanGenerator::GenerateLAN(companyName, (company->GetSize() / 10));
}

void WorldGenerator::GenerateValidMapPos(int& outX, int& outY)
{
	UplinkAssert(worldmapmask != nullptr);

	const auto vlocations = game->GetWorld().GetVLocations().ConvertToDArray();

	auto x = 0;
	auto y = 0;

	// 32 attempts
	for (auto attempts = 0; attempts < 32; attempts++)
	{
		x = NumberGenerator::RandomNumber(VirtualWidth - 1);
		y = NumberGenerator::RandomNumber(VirtualHeight - 1);

		UplinkAssert(x < VirtualWidth);
		UplinkAssert(y < VirtualHeight);

		// If we are outside of the world map, try again and do not count the attempt
		if (worldmapmask->GetPixelR(x, y) == 0)
		{
			attempts--;
			continue;
		}

		auto valid = false;
		for (auto i = vlocations->Size() - 1; i >= 0; i--)
		{
			if (!vlocations->ValidIndex(i))
				break;

			const auto vlocation = vlocations->GetData(i);
			if (vlocation->GetX() - x <= 1 && vlocation->GetY() - y <= 1)
			{
				valid = true;
				break;
			}
		}

		if (!valid)
			break;
	}

	outX = x;
	outY = y;

	delete vlocations;
}

void WorldGenerator::GenerateUplinkPublicAccessServer()
{
	int x, y;
	WorldGenerator::GenerateValidMapPos(x, y);
	NameGenerator::GeneratePublicAccessServerName("Uplink");

	char serverName[0x80];
	UplinkStrncpy(serverName, tempname, 0x80);

	game->GetWorld().CreateVLocation("234.773.0.666", x, y);
	game->GetWorld().CreateComputer(serverName, "Uplink", "234.773.0.666");

	const auto computer = game->GetWorld().GetComputer(serverName);
	UplinkAssert(computer != nullptr);
	computer->SetIsTargetable(0);
	computer->SetTraceSpeed(-1);

	const auto messageScreeen = new MessageScreen();
	messageScreeen->SetMainTitle("Uplink");
	messageScreeen->SetSubTitle("About us");
	messageScreeen->SetNextPage(1);
	messageScreeen->SetButtonMessage("OK");
	messageScreeen->SetTextMessage(
		"Welcome to the Uplink Public Access Server.\n"
		"\n"
		"Uplink Corporation maintains the largest list of freelance agents in the world, and we have operated for the last decade with a "
		"flawless record of satisfied customers and successful agents.  Our company acts as an anonymous job centre, bringing corporations "
		"together with agents who can work for them.  Our company also provides rental of essential gateway computers to all agents, which "
		"allow unparalleled security in a high risk environment.\n"
		"\n"
		"You are here because you wish to join this company.");
	computer->AddComputerScreen(messageScreeen, 0);

	const auto menuScreen = new MenuScreen();
	menuScreen->SetMainTitle("Uplink");
	menuScreen->SetSubTitle("Public server Main Menu");
	menuScreen->AddOption("About Us", "Find out who we are and what we do", 0, 10, -1);
	menuScreen->AddOption("Register as an Agent", "Click to register yourself as a new Uplink Agent", 2, 10, -1);
	computer->AddComputerScreen(menuScreen, 1);

	const auto dialogScreen = new DialogScreen();
	dialogScreen->SetMainTitle("Uplink");
	dialogScreen->SetSubTitle("Registration");
	dialogScreen->AddWidget("caption1", 2, 100, 110, 380, 270,
		"Your Uplink membership package includes : \n"
		"\n"
		"- A Gateway computer at a secure location.  "
		"You will connect to this machine from your home computer when you are working for Uplink.  "
		"You can have it upgraded at a later stage if necessary.\n"
		"\n"
		"- A low interest loan of 3000 credits with Uplink International Bank, to get you started.\n"
		"\n"
		"- Access to our Bulletin Board system - the usual place for Uplink Agents to find work.\n"
		"\n"
		"- You will be officially rated as an Uplink Agent, and we will monitor your progress.  "
		"As your rating increases you will find new avenues of work become available.",
		"");
	dialogScreen->AddWidget("cancel", 5, 160, 380, 100, 20, "Cancel", "Cancel registration", 1, 0, nullptr, nullptr);
	dialogScreen->AddWidget("continue", 5, 270, 380, 100, 20, "Continue", "Continue registration", 3, 0, nullptr, nullptr);
	dialogScreen->SetReturnKeyButton("continue");
	computer->AddComputerScreen(dialogScreen, 2);

	const auto registrationScreen = new DialogScreen();
	registrationScreen->SetMainTitle("Uplink Registration");
	registrationScreen->SetSubTitle("Create your agent profile");
	registrationScreen->AddWidget("name", 1, 80, 140, 170, 20, "Enter your name", "Enter the name you wish to use");
	registrationScreen->AddWidget("password", 1, 80, 170, 170, 20, "Enter your password", "You will need this to access the Uplink Services Machine");
	registrationScreen->AddWidget("password2", 1, 80, 200, 170, 20, "Re-type your password", "For verification purposes");
	registrationScreen->AddWidget("nametext", 3, 270, 140, 170, 20, "Fill this in", "Enter your name here");
	registrationScreen->AddWidget("passwordtext", 4, 270, 170, 170, 20, "", "Enter your password here");
	registrationScreen->AddWidget("passwordtext2", 4, 270, 200, 170, 20, "", "Re-type your password here");
	registrationScreen->AddWidget("moretext", 2, 80, 270, 360, 100,
		"Uplink Corporation will not ask for any more personal details.  In the event of you being charged with illegal "
		"operations, our corporation will be forced to disavow all knowledge of your actions, however you will be safe "
		"from arrest as your real world address will never be stored.",
		"");
	registrationScreen->AddWidget("continue", 8, 270, 400, 100, 20, "Done", "Click here when finished", 33, -1, nullptr, nullptr);
	registrationScreen->SetReturnKeyButton("continue");
	computer->AddComputerScreen(registrationScreen, 3);

	const auto gatewaySelectionScreen = new GenericScreen();
	gatewaySelectionScreen->SetMainTitle("Uplink");
	gatewaySelectionScreen->SetSubTitle("Local gateway selection");
	gatewaySelectionScreen->SetScreenType(31);
	gatewaySelectionScreen->SetNextPage(9);
	computer->AddComputerScreen(gatewaySelectionScreen, 4);

	const auto gatewayInfoScreen = new DialogScreen();
	gatewayInfoScreen->SetMainTitle("Uplink Registration");
	gatewayInfoScreen->SetSubTitle("Your Gateway computer");
	gatewayInfoScreen->AddWidget("caption1", 2, 100, 130, 380, 270,
		"Registration is now taking place.\n"
		"\n"
		"As part of your membership, we will assign you a Gateway computer system in your chosen server room.  "
		"This will act as your jumping off point to the rest of the Net.  "
		"When you next log in you will connect directly from your home computer to "
		"this gateway machine, and from there to the rest of the world.\n"
		"\n"
		"Should any of your actions be traced back to your Gateway, Uplink "
		"Corporation will disavow any knowledge of your actions and will destroy your account.  Your Gateway machine will also be "
		"destroyed.\n"
		"\n"
		"Rental of your Gateway computer will cost 300 credits a month.",
		"");
	gatewayInfoScreen->AddWidget("continue", 8, 270, 380, 100, 20, "Done", "Click here when finished", 34, 6, nullptr, nullptr);
	gatewayInfoScreen->SetReturnKeyButton("continue");
	computer->AddComputerScreen(gatewayInfoScreen, 5);

	const auto connectingScreen = new DialogScreen();
	connectingScreen->AddWidget("connecting", 2, app->GetOptions().GetOptionValue("graphics_screenwidth") - 370,
		app->GetOptions().GetOptionValue("graphics_screenheight") - 30, 370, 20, "", "");
	computer->AddComputerScreen(connectingScreen, 6);

	const auto codeCardScreen = new GenericScreen();
	codeCardScreen->SetMainTitle("Uplink");
	codeCardScreen->SetSubTitle("Code card verification");
	codeCardScreen->SetScreenType(33);
	codeCardScreen->SetNextPage(5);
	computer->AddComputerScreen(codeCardScreen, 9);

	const auto connectionFailedScreen = new DialogScreen();
	connectionFailedScreen->SetMainTitle("Uplink");
	connectionFailedScreen->SetSubTitle("Gateway connection failed");
	connectionFailedScreen->AddWidget("caption1", 2, 100, 130, 400, 270,
		"We have been unable to connect you to your Gateway computer.\n"
		"\n"
		"This may be due to a fault in out communications lines, and if this is the case then we apologise and ask you to try again later.\n"
		"\n"
		"We are aware that Uplink Agents occasionally suffer 'difficulties' with their Gateway systems and sometimes require a new one as a result.  "
		"If this is the case, click YES below and we will assign you a new gateway.  (There will be an administrative charge of 1000 "
		"credits if you take this action.)  Otherwise, click NO to log off and try again later.\n"
		"\n"
		"Rent a new Gateway computer?",
		"", 0, 0, nullptr, nullptr);
	connectionFailedScreen->AddWidget("yes", 8, 170, 380, 100, 20, "YES", "Click here to rent a new Gateway", 50, 4, nullptr, nullptr);
	connectionFailedScreen->AddWidget("no", 8, 300, 380, 100, 20, "NO", "Click here to log out", 51, -1, nullptr, nullptr);
	computer->AddComputerScreen(connectionFailedScreen, 10);
}

VLocation* WorldGenerator::GetRandomLocation()
{
	const auto indices = game->GetWorld().GetVLocations().ConvertIndexToDArray();

	UplinkAssert(indices->Size() > 0);

	const auto index = NumberGenerator::RandomNumber(indices->Size());

	UplinkAssert(indices->ValidIndex(index));

	const auto loc = game->GetWorld().GetVLocation(indices->GetData(index));

	delete indices;

	return loc;
}
