#include <App.hpp>
#include <Eclipse.hpp>
#include <Gci.hpp>
#include <Globals.hpp>
#include <IRCInterface.hpp>
#include <RedShirt.hpp>
#include <Sg.hpp>
#include <Svb.hpp>
#include <Util.hpp>
#include <WorldGenerator.hpp>
#include <cstdlib>
#include <dirent.h>

static void CopyGame(const char* name, const char* filePath)
{
	(void)name;
	char curAgentPath[0x100];
	UplinkSnprintf(curAgentPath, 0x100, "%scuragent.usr", app->UsersTempPath);
	EmptyDirectory(app->UsersTempPath);
	CopyFilePlain(filePath, curAgentPath);
}

App::App() : _startTime(0), _closed(false), _options(nullptr), _network(nullptr), _mainMenu(nullptr), _phoneDialler(nullptr), _nextLoadGame(nullptr)
{
	UplinkStrncpy(Path, "c:/", APP_PATH_MAX);
	UplinkStrncpy(UsersPath, Path, APP_PATH_MAX);
	UplinkStrncpy(Version, "1.31c", APP_VERSION_MAX);
	UplinkStrncpy(Type, "RELEASE", APP_TYPE_MAX);
	UplinkStrncpy(Date, "01/01/97", APP_DATE_MAX);
	UplinkStrncpy(Title, "NewApp", APP_TITLE_MAX);
	UplinkStrncpy(Build, "Version 1.0 (RELEASE), Compiled on 01/01/97", APP_BUILD_MAX);
}

App::~App()
{
	if (!Closed())
		Close();
}

const char* App::GetID()
{
	return "APP";
}

void App::Print()
{
	UplinkAbort("TODO: implement App::Print()");
}

void App::Update()
{
	UplinkAssert(game != nullptr);

	if (game->GetGameSpeed() == -1 || (game->IsRunning() && game->GetWorld().GetPlayer().GetGateway().GetNuked()))
	{
		if (_phoneDialler != nullptr)
			UnRegisterPhoneDialler(_phoneDialler);

		SaveGame(game->GetWorld().GetPlayer().GetHandle());
		game->SetGameSpeed(0);
		EclReset();
		UplinkAbort("TODO: implement App::Update()");
	}

	if (!Closed())
	{
		if (game->IsRunning())
			game->Update();

		if (_mainMenu->IsVisible())
			_mainMenu->Update();

		if (game->IsRunning() || _mainMenu->InScreen() == MainMenuScreenCode::FirstTimeLoading)
		{
			if (_phoneDialler != nullptr && _phoneDialler->UpdateSpecial())
				UnRegisterPhoneDialler(_phoneDialler);
		}

		if (_network->IsActive())
			UplinkAbort("TODO: implement App::Update()");

		IRCInterface::UpdateMessages();
	}
}

void App::Close()
{
	UplinkAssert(!_closed);

	_closed = true;

	CloseGame();

	EclReset();

	if (game != nullptr)
		game->ExitGame();

	_options->ApplyShutdownChanges();
	_options->Save(nullptr);

	SvbReset();
	GciDeleteAllTrueTypeFonts();
	RsCleanUp();
	// TODO: SgPlaylist_Shutdown();
	SgShutdown();

	if (_mainMenu != nullptr)
	{
		delete _mainMenu;
		_mainMenu = nullptr;
	}

	if (_options != nullptr)
	{
		delete _options;
		_options = nullptr;
	}

	if (_network != nullptr)
	{
		delete _network;
		_network = nullptr;
	}

	if (game != nullptr)
	{
		delete game;
		game = nullptr;
	}

	if (_phoneDialler != nullptr)
	{
		delete _phoneDialler;
		_phoneDialler = nullptr;
	}

	if (_nextLoadGame != nullptr)
	{
		delete[] _nextLoadGame;
		_nextLoadGame = nullptr;
	}
}

void App::CloseGame()
{
	IRCInterface::CloseConnection();
}

void App::CoreDump()
{
	puts("============== B E G I N  C O R E  D U M P =================");
	fflush(nullptr);
	PrintStackTrace();
	puts("============== E N D  C O R E  D U M P =====================");
	fflush(nullptr);
}

bool App::Closed()
{
	return _closed;
}

MainMenu& App::GetMainMenu()
{
	UplinkAssert(_mainMenu != nullptr);
	return *_mainMenu;
}

Network& App::GetNetwork()
{
	UplinkAssert(_network != nullptr);
	return *_network;
}

Options& App::GetOptions()
{
	UplinkAssert(_options != nullptr);
	return *_options;
}

bool App::GetCheckCodeCard()
{
	return _checkCodecard;
}

void App::Initialise()
{
	_options = new Options();
	_options->Load(nullptr);
	_options->CreateDefaultOptions();
	_startTime = EclGetAccurateTime();
	_network = new Network();
	_mainMenu = new MainMenu();
}

DArray<char*>* App::ListExistingGames()
{
	const auto array = new DArray<char*>();

	char name[0x100];
	UplinkStrncpy(name, app->UsersPath, sizeof(name));
	name[0xff] = 0;

	DIR* dirp = opendir(name);
	if (dirp == nullptr)
		return array;

	for (auto i = readdir(dirp); i != 0; i = readdir(dirp))
	{
		char* usrExt = strstr(i->d_name, ".usr");
		if (usrExt != nullptr)
		{
			usrExt[0] = 0;
			auto data = new char[0x100];
			UplinkStrncpy(data, i->d_name, sizeof(name));
			array->PutData(data);
		}
	}

	closedir(dirp);
	return array;
}

void App::LoadGame()
{
	const auto game = _nextLoadGame;
	_nextLoadGame = nullptr;

	UplinkAssert(game != nullptr);
	LoadGame(game);
	delete[] game;
}

void App::LoadGame(const char* name)
{
	UplinkAssert(game != nullptr);

	char filePath[0x100];
	UplinkSnprintf(filePath, 0x100, "%s%s.usr", app->UsersPath, name);

	if (RsFileEncrypted(filePath) == 0)
	{
		char tempFileName[0x100];
		UplinkSnprintf(tempFileName, 0x100, "%s%s.tmp", app->UsersPath, name);
		if (RsFileEncrypted(tempFileName))
			strcpy(filePath, tempFileName);
	}

	CopyGame(name, filePath);

	printf("Loading profile from %s...", filePath);

	const auto file = RsFileOpen(filePath, "rb");
	if (file == nullptr)
	{
		EmptyDirectory(app->UsersTempPath);
		puts("failed");
		puts("App::LoadGame, Failed to load user profile");
		GetOptions().GetOptionValue("graphics_screenheight");
		GetOptions().GetOptionValue("graphics_screenwidth");
		EclReset();
		GetMainMenu().RunScreen(MainMenuScreenCode::Login);
		return;
	}

	GetMainMenu().Remove();
	const auto success = game->LoadGame(file);
	RsFileClose(filePath, file);
	if (!success)
	{
		EmptyDirectory(app->UsersTempPath);
		puts("App::LoadGame, Failed to load user profile");
		delete game;
		game = new Game();
		return;
	}

	puts("success");
	if (game->GetGameSpeed() == -1)
	{
		game->SetGameSpeed(0);
		app->GetOptions().GetOptionValue("graphics_screenheight");
		app->GetOptions().GetOptionValue("graphics_screenwidth");
		EclReset();
		_mainMenu->RunScreen(MainMenuScreenCode::Obituary);
		return;
	}

	WorldGenerator::LoadDynamics();

	game->GetWorld().GetPlayer().GetConnection().Disconnect();
	game->GetWorld().GetPlayer().GetConnection().Reset();

	auto& player = game->GetWorld().GetPlayer();
	if (player.GetGateway().GetExchangeGatewayDef() == nullptr)
	{
		if (player.GetGateway().GetNuked())
		{
			player.GetGateway().SetNuked(false);
			player.GetConnection().AddVLocation("234.773.0.666");
			player.GetConnection().Connect();
			game->GetInterface().GetLocalInterface().Remove();
			app->GetOptions().GetOptionValue("graphics_screenheight");
			app->GetOptions().GetOptionValue("graphics_screenwidth");
			EclReset();
		}
		else
		{
			game->GetInterface().GetRemoteInterface().RunNewLocation();
			game->GetInterface().GetRemoteInterface().RunScreen(3, nullptr);
		}
	}
	else
	{
		player.GetGateway().ExchangeGatewayComplete();
	}

	if (player.GetGateway().GetExchangeGatewayDef() != nullptr || player.GetGateway().GetNuked())
	{
		game->GetInterface().GetRemoteInterface().RunNewLocation();
		game->GetInterface().GetRemoteInterface().RunScreen(10, nullptr);
	}
}

void App::RegisterPhoneDialler(PhoneDialler* newPhoneDiallerScreen)
{
	UplinkAssert(newPhoneDiallerScreen != nullptr);
	UplinkAssert(_phoneDialler != newPhoneDiallerScreen);

	if (_phoneDialler != nullptr)
		UnRegisterPhoneDialler(_phoneDialler);

	_phoneDialler = newPhoneDiallerScreen;
}

void App::UnRegisterPhoneDialler(PhoneDialler* dialler)
{
	if (_phoneDialler != dialler)
		return;

	dialler->Remove();

	delete _phoneDialler;
	_phoneDialler = nullptr;
}

void App::RetireGame(const char* name)
{
	(void)name;
	UplinkAbort("TODO: implement App::RetireGame()");
}

void App::SaveGame(char const* name)
{
	if (strcmp(name, "NEWAGENT") == 0)
		return;

	UplinkAssert(game != nullptr);

	MakeDirectory(UsersPath);
	char tempfile[0x100];
	UplinkSnprintf(tempfile, 0x100, "%s%s.tmp", UsersPath, name);

	char destfile[0x100];
	UplinkSnprintf(destfile, 0x100, "%s%s.usr", UsersPath, name);

	printf("Saving profile to %s...", tempfile);

	const auto file = fopen(tempfile, "wb");
	if (file == nullptr)
	{
		puts("failed");
		puts("App::SaveGame, Failed to save user profile");
		return;
	}

	game->Save(file);
	fclose(file);
	RsEncryptFile(tempfile);
	printf("success. Moving profile to %s...", destfile);

	if (!CopyFilePlain(tempfile, destfile))
	{
		puts("failed");
		printf("App::SaveGame, Failed to copy user profile from %s to %s\n", tempfile, destfile);
		return;
	}

	puts("success");
	CopyGame(name, destfile);
}

void App::Set(const char* newPath, const char* newVersion, const char* newType, const char* newDate, const char* newTitle)
{
	// TODO: remove these unnecessary(?) asserts (UplinkStrncpy checks this??)
	UplinkAssert(strlen(newPath) < APP_PATH_MAX);
	UplinkAssert(strlen(newVersion) < APP_VERSION_MAX);
	UplinkAssert(strlen(newType) < APP_TYPE_MAX);
	UplinkAssert(strlen(newDate) < APP_DATE_MAX);
	UplinkAssert(strlen(newTitle) < APP_TITLE_MAX);

	UplinkStrncpy(Path, newPath, APP_PATH_MAX);
	UplinkStrncpy(Version, newVersion, APP_VERSION_MAX);
	UplinkStrncpy(Type, newType, APP_TYPE_MAX);
	UplinkStrncpy(Date, newDate, APP_DATE_MAX);
	UplinkStrncpy(Title, newTitle, APP_TITLE_MAX);
	UplinkSnprintf(Build, APP_BUILD_MAX, "Version %s (%s)\nCompiled on %s\n", Version, Type, Date);

#ifdef NDEBUG
	char* homeDirPath = getenv("HOME");
#else
	char* homeDirPath = nullptr;
#endif

	if (homeDirPath != nullptr)
	{
		UplinkSnprintf(UsersPath, APP_PATH_MAX, "%s/.uplink/", homeDirPath);
		UplinkSnprintf(UsersTempPath, APP_PATH_MAX, "%s/.uplink/userstmp/", homeDirPath);
		UplinkSnprintf(UsersOldPath, APP_PATH_MAX, "%s/.uplink/usersold/", homeDirPath);
	}
	else
	{
		UplinkSnprintf(UsersPath, APP_PATH_MAX, "%susers/", Path);
		UplinkSnprintf(UsersTempPath, APP_PATH_MAX, "%suserstmp/", Path);
		UplinkSnprintf(UsersOldPath, APP_PATH_MAX, "%susersold/", Path);
	}
}

void App::SetNextLoadGame(char const* name)
{
	UplinkAssert(name != nullptr);

	if (_nextLoadGame != 0)
		delete[] _nextLoadGame;

	_nextLoadGame = new char[strlen(name) + 1];

	strcpy(_nextLoadGame, name);
}
