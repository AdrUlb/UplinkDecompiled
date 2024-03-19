#include <App.hpp>
#include <Util.hpp>
#include <cstdlib>

App::App()
	: startTime(0), closed(false), options(nullptr), network(nullptr), mainMenu(nullptr), phoneDiallerScreen(nullptr), nextLoadGame(nullptr)
{
	UplinkStrncpy(path, "c:/", PATH_MAX);
	UplinkStrncpy(usersPath, path, PATH_MAX);
	UplinkStrncpy(version, "1.31c", VERSION_MAX);
	UplinkStrncpy(type, "RELEASE", TYPE_MAX);
	UplinkStrncpy(date, "01/01/97", DATE_MAX);
	UplinkStrncpy(title, "NewApp", TITLE_MAX);
	UplinkStrncpy(build, "Version 1.0 (RELEASE), Compiled on 01/01/97", BUILD_MAX);
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
	puts("TODO: implement App::Print()");
	abort();
}

void App::Update()
{
	puts("TODO: implement App::Update()");
	abort();
}

void App::Close()
{
	puts("TODO: implement App::Close()");
	abort();
}

void App::CloseGame()
{
	puts("TODO: implement App::CloseGame()");
	abort();
}

void App::CoreDump()
{
	puts("============== B E G I N  C O R E  D U M P =================");
	PrintStackTrace();
	puts("============== E N D  C O R E  D U M P =====================");
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
	puts("TODO: implement App::Initialise()");
	abort();
}

DArray<char*>* App::ListExistingGames()
{
	puts("TODO: implement App::ListExistingGames()");
	abort();
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
	puts("TODO: implement App::LoadGame(const char*)");
	abort();
}

void App::RegisterPhoneDialler(PhoneDiallerScreen* newPhoneDiallerScreen)
{
	UplinkAssert(newPhoneDiallerScreen != nullptr);
	UplinkAssert(phoneDiallerScreen != newPhoneDiallerScreen);

	if (phoneDiallerScreen != nullptr)
		UnRegisterPhoneDialler(phoneDiallerScreen);

	phoneDiallerScreen = newPhoneDiallerScreen;
}

void App::UnRegisterPhoneDialler(PhoneDiallerScreen* phoneDiallerScreen)
{
	(void)phoneDiallerScreen;
	puts("TODO: implement App::UnRegisterPhoneDialler(PhoneDiallerScreen*)");
	abort();
}

void App::RetireGame(const char* name)
{
	(void)name;
	puts("TODO: implement App::RetireGame(const char*)");
	abort();
}

void App::SaveGame(char const* name)
{
	(void)name;
	puts("TODO: implement App::SaveGame(const char*)");
	abort();
}

void App::Set(char* newPath, char* newVersion, char* newType, char* newDate, char* newTitle)
{
	// TODO: remove these unnecessary(?) asserts (UplinkStrncpy checks this??)
	UplinkAssert(strlen(newPath) < PATH_MAX);
	UplinkAssert(strlen(newVersion) < VERSION_MAX);
	UplinkAssert(strlen(newType) < TYPE_MAX);
	UplinkAssert(strlen(newDate) < DATE_MAX);
	UplinkAssert(strlen(newTitle) < TITLE_MAX);

	UplinkStrncpy(path, newPath, PATH_MAX);
	UplinkStrncpy(version, newVersion, VERSION_MAX);
	UplinkStrncpy(type, newType, TYPE_MAX);
	UplinkStrncpy(date, newDate, DATE_MAX);
	UplinkStrncpy(title, newTitle, TITLE_MAX);
	UplinkSnprintf(build, BUILD_MAX, "Version %s (%s)\nCompiled on %s\n", version, type, date);

	char* homeDirPath = getenv("HOME");

	if (homeDirPath != nullptr)
	{
		UplinkSnprintf(usersPath, PATH_MAX, "%s/.uplink/", homeDirPath);
		UplinkSnprintf(usersTempPath, PATH_MAX, "%s/.uplink/userstmp/", homeDirPath);
		UplinkSnprintf(usersOldPath, PATH_MAX, "%s/.uplink/usersold/", homeDirPath);
	}
	else
	{
		UplinkSnprintf(usersPath, PATH_MAX, "%susers/", path);
		UplinkSnprintf(usersTempPath, PATH_MAX, "%suserstmp/", path);
		UplinkSnprintf(usersOldPath, PATH_MAX, "%susersold/", path);
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
