#include "App.hpp"

#include "Util.hpp"

App::App()
{
	strncpy(path, "c:/", APP_PATH_MAX);
	UplinkStrncpy(usersPath, path, APP_PATH_MAX);
	strncpy(version, "1.31c", APP_VERSION_MAX);
	strncpy(type, "1.31c", APP_TYPE_MAX);
	strncpy(date, "01/01/97", APP_DATE_MAX);
	strncpy(title, "NewApp", APP_TITLE_MAX);
	strncpy(build, "Version 1.0 (RELEASE), Compiled on 01/01/97", APP_BUILD_SIZE);
}

void App::Set(const char* path, const char* version, const char* type, const char* date, const char* title)
{
	size_t temp;
	uint temp6;
	char* temp2;
	char* temp3;
	char* temp4;
	char* temp5;

	UplinkAssert(strlen(path) < APP_PATH_MAX);
	UplinkAssert(strlen(version) < APP_VERSION_MAX);
	UplinkAssert(strlen(type) < APP_TYPE_MAX);
	UplinkAssert(strlen(date) < APP_DATE_MAX);
	UplinkAssert(strlen(title) < APP_TITLE_MAX);

	UplinkStrncpy(this->path, path, APP_PATH_MAX);
	UplinkStrncpy(this->version, version, APP_VERSION_MAX);
	UplinkStrncpy(this->type, type, APP_TYPE_MAX);
	UplinkStrncpy(this->date, date, APP_DATE_MAX);
	UplinkStrncpy(this->title, title, APP_TITLE_MAX);

	UplinkSnprintf(build, APP_BUILD_SIZE, "Version %s (%s)\nCompiled on %s\n", this->version, this->type, this->date);

	auto homeDir = getenv("HOME");
	if (homeDir)
	{
		UplinkSnprintf(usersPath, APP_PATH_MAX, "%s/.uplink/", homeDir);
		UplinkSnprintf(usersTempPath, APP_PATH_MAX, "%s/.uplink/userstmp/", homeDir);
		UplinkSnprintf(usersOldPath, APP_PATH_MAX, "%s/.uplink/usersold/", homeDir);
	}
	else
	{
		UplinkSnprintf(usersPath, APP_PATH_MAX, "%s/.uplink/", temp2);
		UplinkSnprintf(usersTempPath, APP_PATH_MAX, "%s/.uplink/userstmp/", temp2);
		UplinkSnprintf(usersOldPath, APP_PATH_MAX, "%s/.uplink/usersold/", temp2);
	}
}
