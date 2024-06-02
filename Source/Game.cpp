#include <Game.hpp>
#include <Globals.hpp>
#include <NotificationEvent.hpp>
#include <NumberGenerator.hpp>
#include <Opengl.hpp>
#include <RedShirt.hpp>
#include <Sg.hpp>
#include <Util.hpp>
#include <WorldGenerator.hpp>

Game::Game()
{
	_interface = nullptr;
	_view = nullptr;
	_world = nullptr;
	_speed = 0;
	_obituary = nullptr;
	_loadedSaveFileVer = nullptr;
	_createdSaveFileVer = nullptr;
	_field_48 = 0;
	_winCodeDesc = nullptr;
	_field_58 = nullptr;
	_field_60 = 0;
	_worldMapType = 1;
}

Game::~Game()
{
	if (_interface != nullptr)
		delete _interface;

	if (_view != nullptr)
		delete _view;

	if (_world != nullptr)
		delete _world;

	if (_obituary != nullptr)
		delete _obituary;

	if (_loadedSaveFileVer != nullptr)
		delete[] _loadedSaveFileVer;

	if (_createdSaveFileVer != nullptr)
		delete[] _createdSaveFileVer;

	if (_winCodeDesc != nullptr)
		delete[] _winCodeDesc;

	if (_field_58 != nullptr)
		delete[] _field_58;
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
	if (_speed > 0)
	{
		GetWorld()->Update();
		GetView()->Update();
		GetInterface()->Update();
	}

	if (time(nullptr) > _lastAutosaveTime + 60)
	{
		const auto player = GetWorld()->GetPlayer();
		app->SaveGame(player->GetHandle());
		_lastAutosaveTime = time(nullptr);
	}
}

const char* Game::GetID()
{
	return "GAME";
}

int Game::GameSpeed()
{
	return _speed;
}

World* Game::GetWorld()
{
	UplinkAssert(_world != nullptr);
	return _world;
}

View* Game::GetView()
{
	UplinkAssert(_view != nullptr);
	return _view;
}

Interface* Game::GetInterface()
{
	UplinkAssert(_interface != nullptr);
	return _interface;
}

bool Game::IsRunning()
{
	return _speed > 0;
}

void Game::NewGame()
{
	if (_interface != nullptr)
		delete _interface;

	if (_view != nullptr)
		delete _view;

	if (_world != nullptr)
		delete _world;

	if (_obituary != nullptr)
		delete _obituary;

	if (_createdSaveFileVer != nullptr)
		delete[] _createdSaveFileVer;

	_createdSaveFileVer = new char[strlen(latestSaveVersion) + 1];
	strcpy(_createdSaveFileVer, latestSaveVersion);
	_field_60 = 0;

	for (auto i = 0; i < 32; i += 8)
		_field_60 |= (NumberGenerator::RandomNumber(254) + 1) << i;

	_obituary = nullptr;
	_worldMapType = 1;
	if ((app->GetOptions()->GetOption("graphics_defaultworldmap") != nullptr &&
		 app->GetOptions()->GetOption("graphics_defaultworldmap")->GetValue() != 0))
		_worldMapType = 0;

	_world = new World();
	WorldGenerator::LoadDynamicsGatewayDefs();
	NotificationEvent::ScheduleStartingEvents();
	WorldGenerator::GenerateAll();
	_world->GetPlotGenerator().Initialise();
	SgPlaySound(RsArchiveFileOpen("sounds/ringout.wav"), "sounds/ringout.wav");

	_view = new View();
	_view->Initialise();

	_interface = new Interface();
	GetInterface()->Create();
	_speed = 1;

	Date date;
	date.SetDate(0, 0, 0, 24, 3, 2010);

	for (bool keepGen = true; keepGen; keepGen = Game::GetWorld()->GetCurrentDate().Before(&date))
	{
		GetWorld()->Update();
		GetWorld()->GetCurrentDate().AdvanceDay(1);
		GetWorld()->GetCurrentDate().AdvanceHour(NumberGenerator::RandomNumber(12) - 6);
		GetWorld()->GetCurrentDate().AdvanceMinute(NumberGenerator::RandomNumber(60));

		if (NumberGenerator::RandomNumber(5) == 0)
			WorldGenerator::GenerateSimpleStartingMissionA();

		if (NumberGenerator::RandomNumber(5) == 0)
			WorldGenerator::GenerateSimpleStartingMissionB();
	}
}

const char* Game::GetLoadedSavefileVer()
{
	if (_loadedSaveFileVer == nullptr)
		return latestSaveVersion;

	return _loadedSaveFileVer;
}

int Game::GetWorldMapType()
{
	return _worldMapType;
}

void Game::ExitGame()
{
	opengl_close();
}
