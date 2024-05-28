#include <App.hpp>
#include <Eclipse.hpp>
#include <Globals.hpp>
#include <IRCInterface.hpp>
#include <Util.hpp>
#include <cstdlib>
#include <dirent.h>

App::App() : startTime(0), closed(false), options(nullptr), network(nullptr), mainMenu(nullptr), phoneDialler(nullptr), nextLoadGame(nullptr)
{
	UplinkStrncpy(path, "c:/", APP_PATH_MAX);
	UplinkStrncpy(usersPath, path, APP_PATH_MAX);
	UplinkStrncpy(version, "1.31c", APP_VERSION_MAX);
	UplinkStrncpy(type, "RELEASE", APP_TYPE_MAX);
	UplinkStrncpy(date, "01/01/97", APP_DATE_MAX);
	UplinkStrncpy(title, "NewApp", APP_TITLE_MAX);
	UplinkStrncpy(build, "Version 1.0 (RELEASE), Compiled on 01/01/97", APP_BUILD_MAX);
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

	if (game->GameSpeed() == -1 || (game->IsRunning() && game->GetWorld()->GetPlayer()->gateway.nuked))
	{
		if (phoneDialler != nullptr)
			UnRegisterPhoneDialler(phoneDialler);

		SaveGame(game->GetWorld()->GetPlayer()->handle);
		UplinkAbort("TODO: implement App::Update()");
	}

	if (!Closed())
	{
		if (game->IsRunning())
			game->Update();

		if (mainMenu->IsVisible())
			mainMenu->Update();

		if (game->IsRunning() || mainMenu->InScreen() == MainMenuScreenCode::FirstTimeLoading)
		{
			if (phoneDialler != nullptr && phoneDialler->UpdateSpecial())
				UnRegisterPhoneDialler(phoneDialler);
		}

		if (network->IsActive())
			UplinkAbort("TODO: implement App::Update()");

		IRCInterface::UpdateMessages();
	}
}

void App::Close()
{
	UplinkAbort("TODO: implement App::Close()");
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
	return closed;
}

MainMenu* App::GetMainMenu()
{
	UplinkAssert(mainMenu != nullptr);
	return mainMenu;
}

Network* App::GetNetwork()
{
	UplinkAssert(network != nullptr);
	return network;
}

Options* App::GetOptions()
{
	UplinkAssert(options != nullptr);
	return options;
}

void App::Initialise()
{
	options = new Options();
	options->Load(nullptr);
	options->CreateDefaultOptions();
	startTime = EclGetAccurateTime();
	network = new Network();
	mainMenu = new MainMenu();
}

DArray<char*>* App::ListExistingGames()
{
	const auto array = new DArray<char*>();

	char name[0x100];
	UplinkStrncpy(name, app->usersPath, sizeof(name));
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
	UplinkAssert(nextLoadGame != nullptr);
	LoadGame(nextLoadGame);
	delete[] nextLoadGame;
	nextLoadGame = nullptr;
}

void App::LoadGame(const char* name)
{
	(void)name;
	UplinkAbort("TODO: implement App::LoadGame(const char*)");
}

void App::RegisterPhoneDialler(PhoneDialler* newPhoneDiallerScreen)
{
	UplinkAssert(newPhoneDiallerScreen != nullptr);
	UplinkAssert(phoneDialler != newPhoneDiallerScreen);

	if (phoneDialler != nullptr)
		UnRegisterPhoneDialler(phoneDialler);

	phoneDialler = newPhoneDiallerScreen;
}

void App::UnRegisterPhoneDialler(PhoneDialler* dialler)
{
	if (phoneDialler != dialler)
		return;

	dialler->Remove();

	delete phoneDialler;
	phoneDialler = nullptr;
}

void App::RetireGame(const char* name)
{
	(void)name;
	UplinkAbort("TODO: implement App::RetireGame(const char*)");
}

void App::SaveGame(char const* name)
{
	(void)name;
	UplinkAbort("TODO: implement App::SaveGame(const char*)");
}

void App::Set(const char* newPath, const char* newVersion, const char* newType, const char* newDate, const char* newTitle)
{
	// TODO: remove these unnecessary(?) asserts (UplinkStrncpy checks this??)
	UplinkAssert(strlen(newPath) < APP_PATH_MAX);
	UplinkAssert(strlen(newVersion) < APP_VERSION_MAX);
	UplinkAssert(strlen(newType) < APP_TYPE_MAX);
	UplinkAssert(strlen(newDate) < APP_DATE_MAX);
	UplinkAssert(strlen(newTitle) < APP_TITLE_MAX);

	UplinkStrncpy(path, newPath, APP_PATH_MAX);
	UplinkStrncpy(version, newVersion, APP_VERSION_MAX);
	UplinkStrncpy(type, newType, APP_TYPE_MAX);
	UplinkStrncpy(date, newDate, APP_DATE_MAX);
	UplinkStrncpy(title, newTitle, APP_TITLE_MAX);
	UplinkSnprintf(build, APP_BUILD_MAX, "Version %s (%s)\nCompiled on %s\n", version, type, date);

#ifdef NDEBUG
	char* homeDirPath = getenv("HOME");
#else
	char* homeDirPath = nullptr;
#endif

	if (homeDirPath != nullptr)
	{
		UplinkSnprintf(usersPath, APP_PATH_MAX, "%s/.uplink/", homeDirPath);
		UplinkSnprintf(usersTempPath, APP_PATH_MAX, "%s/.uplink/userstmp/", homeDirPath);
		UplinkSnprintf(usersOldPath, APP_PATH_MAX, "%s/.uplink/usersold/", homeDirPath);
	}
	else
	{
		UplinkSnprintf(usersPath, APP_PATH_MAX, "%susers/", path);
		UplinkSnprintf(usersTempPath, APP_PATH_MAX, "%suserstmp/", path);
		UplinkSnprintf(usersOldPath, APP_PATH_MAX, "%susersold/", path);
	}
}

void App::SetNextLoadGame(char const* name)
{
	UplinkAssert(name != nullptr);

	if (nextLoadGame != 0)
		delete[] nextLoadGame;

	nextLoadGame = new char[strlen(name) + 1];

	strcpy(nextLoadGame, name);
}
