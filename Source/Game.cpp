#include <Eclipse.hpp>
#include <Events/NotificationEvent.hpp>
#include <Game.hpp>
#include <Globals.hpp>
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
	if (_interface != nullptr)
		delete _interface;
	if (_view != nullptr)
		delete _view;
	if (_world != nullptr)
		delete _world;

	if (_obituary != nullptr)
		delete _obituary;

	_obituary = nullptr;

	_interface = new Interface();
	_view = new View();
	_world = new World();

	WorldGenerator::LoadDynamicsGatewayDefs();

	if (strcmp(game->GetLoadedSavefileVer(), "SAV62") >= 0)
	{
		if (!FileReadData(&_worldMapType, 4, 1, file))
			return false;
	}
	else
		_worldMapType = 0;

	if (!FileReadData(&_speed, 4, 1, file))
		return false;

	if (_speed == -1)
	{
		_obituary = new GameObituary();
		if (!_obituary->Load(file))
			return false;
	}
	else
	{
		if (!GetWorld().Load(file))
			return false;

		if (!GetInterface().Load(file))
			return false;

		if (!GetView().Load(file))
			return false;
	}

	if (strcmp(game->GetLoadedSavefileVer(), "SAV58") >= 0)
	{
		if (!LoadDynamicString(_createdSaveFileVer, file))
			return false;

		if (!FileReadData(&_field_48, 4, 1, file))
			return false;

		if (!LoadDynamicString(_winCodeDesc, file))
			return false;

		if (!LoadDynamicString(_field_58, file))
			return false;

		if (!FileReadData(&_field_60, 4, 1, file))
			return false;
	}

	return true;
}

void Game::Save(FILE* file)
{
	if (_speed == 0)
		return;

	UplinkAssert(_interface != 0);
	UplinkAssert(_view != 0);
	UplinkAssert(_world != 0);

	fwrite("SAV62", 6, 1, file);
	fwrite(&_worldMapType, 4, 1, file);
	fwrite(&_speed, 4, 1, file);

	if (_speed != -1)
	{
		game->GetWorld().Save(file);
		game->GetInterface().Save(file);
		game->GetView().Save(file);
	}
	else
	{
		UplinkAssert(_obituary != nullptr);
		_obituary->Save(file);
	}

	SaveDynamicString(_createdSaveFileVer, file);
	fwrite(&_field_48, 4, 1, file);
	SaveDynamicString(_winCodeDesc, file);
	SaveDynamicString(_field_58, file);
	fwrite(&_field_60, 4, 1, file);
}

void Game::Print()
{
	UplinkAbort("TODO: implement Game::Print()");
}

void Game::Update()
{
	if (_speed > 0)
	{
		GetWorld().Update();
		GetView().Update();
		GetInterface().Update();
	}

	if (time(nullptr) > _lastAutosaveTime + 60)
	{
		app->SaveGame(GetWorld().GetPlayer().GetHandle());
		_lastAutosaveTime = time(nullptr);
	}
}

const char* Game::GetID()
{
	return "GAME";
}

Interface& Game::GetInterface()
{
	UplinkAssert(_interface != nullptr);
	return *_interface;
}

View& Game::GetView()
{
	UplinkAssert(_view != nullptr);
	return *_view;
}

World& Game::GetWorld()
{
	UplinkAssert(_world != nullptr);
	return *_world;
}

int Game::GetGameSpeed()
{
	return _speed;
}

int Game::GetWorldMapType()
{
	return _worldMapType;
}

const char* Game::GetLoadedSavefileVer()
{
	if (_loadedSaveFileVer == nullptr)
		return latestSaveVersion;

	return _loadedSaveFileVer;
}

void Game::SetGameSpeed(int speed)
{
	_speed = speed;
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
	if ((app->GetOptions().GetOption("graphics_defaultworldmap") != nullptr &&
		 app->GetOptions().GetOption("graphics_defaultworldmap")->GetValue() != 0))
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
	GetInterface().Create();
	_speed = 1;

	Date date;
	date.SetDate(0, 0, 0, 24, 3, 2010);

	for (bool keepGen = true; keepGen; keepGen = GetWorld().GetCurrentDate().Before(&date))
	{
		GetWorld().Update();
		GetWorld().GetCurrentDate().AdvanceDay(1);
		GetWorld().GetCurrentDate().AdvanceHour(NumberGenerator::RandomNumber(12) - 6);
		GetWorld().GetCurrentDate().AdvanceMinute(NumberGenerator::RandomNumber(60));

		if (NumberGenerator::RandomNumber(5) == 0)
			WorldGenerator::GenerateSimpleStartingMissionA();

		if (NumberGenerator::RandomNumber(5) == 0)
			WorldGenerator::GenerateSimpleStartingMissionB();
	}
}

bool Game::LoadGame(FILE* file)
{
	srand(time(nullptr));

	if (_loadedSaveFileVer != nullptr)
		delete[] _loadedSaveFileVer;

	_loadedSaveFileVer = new char[7];
	if (FileReadData(_loadedSaveFileVer, 6, 1, file))
	{
		_loadedSaveFileVer[6] = 0;
	}
	else
	{
		_loadedSaveFileVer[0] = 0;
	}

	if (_loadedSaveFileVer[0] == 0 || strcmp(_loadedSaveFileVer, minSaveVersion) < 0 || strcmp(_loadedSaveFileVer, latestSaveVersion) > 0)
	{
		EclReset();
		app->GetMainMenu().RunScreen(MainMenuScreenCode::Login);
		char str[0x100];
		UplinkSnprintf(str, 0x100,
					   "Failed to load user profile\n"
					   "The save file is not compatible\n"
					   "\n"
					   "Save file is Version [%s]\n"
					   "Required Version is between [%s] and [%s]",
					   _loadedSaveFileVer, minSaveVersion, latestSaveVersion);
		create_msgbox("Error", str, nullptr);
		return false;
	}

	if (!Load(file))
	{
		EclReset();
		app->GetMainMenu().RunScreen(MainMenuScreenCode::Login);
		create_msgbox("Error",
					  "Failed to load user profile\n"
					  "The save file is either\n"
					  "not compatible or\n"
					  "corrupted",
					  nullptr);
		return false;
	}

	return true;
}

void Game::ExitGame()
{
	opengl_close();
}
