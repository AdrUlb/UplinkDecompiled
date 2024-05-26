#include <Game.hpp>
#include <Globals.hpp>
#include <NumberGenerator.hpp>
#include <Util.hpp>

Game::Game()
{
	interface = nullptr;
	view = nullptr;
	world = nullptr;
	speed = 0;
	obituary = nullptr;
	loadedSaveFileVer = nullptr;
	createdSaveFileVer = nullptr;
	field_48 = 0;
	winCodeDesc = nullptr;
	field_58 = nullptr;
	field_60 = 0;
	worldMapType = 1;
}

Game::~Game()
{
	UplinkAbort("TODO: implement Game::~Game()");
}

bool Game::Load(FILE* file)
{
	(void)file;
	UplinkAbort("TODO: implement Game::Load(FILE*)");
}

void Game::Save(FILE* file)
{
	(void)file;
	UplinkAbort("TODO: implement Game::Save(FILE*)");
}

void Game::Print()
{
	UplinkAbort("TODO: implement Game::Print()");
}

void Game::Update()
{
	UplinkAbort("TODO: implement Game::Update()");
}

const char* Game::GetID()
{
	return "GAME";
}

int Game::GameSpeed()
{
	return speed;
}

World* Game::GetWorld()
{
	UplinkAssert(world != nullptr);
	return world;
}

bool Game::IsRunning()
{
	return speed > 0;
}

void Game::NewGame()
{
	if (interface != nullptr)
		delete interface;

	if (view != nullptr)
		delete view;

	if (world != nullptr)
		delete world;

	if (obituary != nullptr)
		delete obituary;

	if (createdSaveFileVer != nullptr)
		delete[] createdSaveFileVer;

	createdSaveFileVer = new char[strlen(latestSaveVersion) + 1];
	strcpy(createdSaveFileVer, latestSaveVersion);
	field_60 = 0;

	for (auto i = 0; i < 32; i += 8)
		field_60 |= (NumberGenerator::RandomNumber(254) + 1) << i;

	obituary = nullptr;
	worldMapType = 1;
	if ((app->GetOptions()->GetOption("graphics_defaultworldmap") != nullptr &&
		 app->GetOptions()->GetOption("graphics_defaultworldmap")->GetValue() != 0))
		worldMapType = 0;

	world = new World();
	/*WorldGenerator::LoadDynamicsGatewayDefs();
	NotificationEvent::ScheduleStartingEvents();
	WorldGenerator::GenerateAll();
	world->plotGenerator.Initialise();
	SgPlaySound(RsArchiveFileOpen("sounds/ringout.wav"), "sounds/ringout.wav");

	view = new View();
	view.Initialise();

	interface = new Interface();
	GetInterface()->Create();
	speed = 1;

	Date date;
	date.SetDate(0, 0, 0, 24, 3, 2010);

	for (bool keepGen = true; keepGen; keepGen = Game::GetWorld()->currentDate.Before(&date))
	{
		GetWorld()->Update();
		GetWorld()->currentDate.AdvanceDay(1);
		GetWorld()->currentDate.AdvanceHour(NumberGenerator::RandomNumber(12) - 6);
		GetWorld()->currentDate.AdvanceMinute(NumberGenerator::RandomNumber(60));

		if (NumberGenerator::RandomNumber(5) == 0)
			WorldGenerator::GenerateSimpleStartingMissionA();

		if (NumberGenerator::RandomNumber(5) == 0)
			WorldGenerator::GenerateSimpleStartingMissionB();
	}*/

	UplinkAbort("TODO: implement Game::NewGame()");
}

const char* Game::GetLoadedSavefileVer()
{
	if (loadedSaveFileVer == nullptr)
		return latestSaveVersion;

	return loadedSaveFileVer;
}

int Game::GetWorldMapType()
{
	return worldMapType;
}
