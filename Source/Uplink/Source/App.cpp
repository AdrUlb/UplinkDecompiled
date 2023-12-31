#include <App.hpp>

#include <cassert>
#include <Util.hpp>
#include <Ecl.hpp>
#include "../TempDefs.hpp"

App::App()
{
	strncpy(Path, "c:/", APP_PATH_MAX);
	UplinkStrncpy(UsersPath, Path, APP_PATH_MAX);
	strncpy(Version, "1.31c", APP_VERSION_MAX);
	strncpy(Type, "1.31c", APP_TYPE_MAX);
	strncpy(Date, "01/01/97", APP_DATE_MAX);
	strncpy(Title, "NewApp", APP_TITLE_MAX);
	strncpy(Build, "Version 1.0 (RELEASE), Compiled on 01/01/97", APP_BUILD_SIZE);
}

App::~App()
{
	if (!Closed())
		Close();
}

void App::Print()
{
	App_Print(this);
}

void App::Update()
{
	App_Update(this);
}

const char* App::GetID()
{
	return "APP";
}

void App::Set(const char* path, const char* version, const char* type, const char* date, const char* title)
{
	UplinkAssert(strlen(path) < APP_PATH_MAX);
	UplinkAssert(strlen(version) < APP_VERSION_MAX);
	UplinkAssert(strlen(type) < APP_TYPE_MAX);
	UplinkAssert(strlen(date) < APP_DATE_MAX);
	UplinkAssert(strlen(title) < APP_TITLE_MAX);

	UplinkStrncpy(this->Path, path, APP_PATH_MAX);
	UplinkStrncpy(this->Version, version, APP_VERSION_MAX);
	UplinkStrncpy(this->Type, type, APP_TYPE_MAX);
	UplinkStrncpy(this->Date, date, APP_DATE_MAX);
	UplinkStrncpy(this->Title, title, APP_TITLE_MAX);

	UplinkSnprintf(Build, APP_BUILD_SIZE, "Version %s (%s)\nCompiled on %s\n", this->Version, this->Type, this->Date);

	auto homeDir = getenv("HOME");
	if (homeDir)
	{
		UplinkSnprintf(UsersPath, APP_PATH_MAX, "%s/.uplink/", homeDir);
		UplinkSnprintf(UsersTempPath, APP_PATH_MAX, "%s/.uplink/userstmp/", homeDir);
		UplinkSnprintf(UsersOldPath, APP_PATH_MAX, "%s/.uplink/usersold/", homeDir);
	}
	else
	{
		UplinkSnprintf(UsersPath, APP_PATH_MAX, "%s/.uplink/", path);
		UplinkSnprintf(UsersTempPath, APP_PATH_MAX, "%s/.uplink/userstmp/", path);
		UplinkSnprintf(UsersOldPath, APP_PATH_MAX, "%s/.uplink/usersold/", path);
	}
}

Options* App::GetOptions()
{
	UplinkAssert(options);
	return options;
}

Options* App::GetOptionsOrNull()
{
	return options;
}

void App::Initialise()
{
	options = new Options();
	options->Load(nullptr);
	options->CreateDefaultOptions();
	uptime = (int)EclGetAccurateTime();
	network = new Network();
	mainMenu = new MainMenu();
}

void App::Close()
{
	App_Close(this);
}

bool App::Closed()
{
	return closed;
}

void App::CoreDump()
{
	App_CoreDump();
}
