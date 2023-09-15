#pragma once

#include "UplinkObject.hpp"
#include "UplinkDecompiledTempDefs.hpp"

constexpr size_t APP_PATH_MAX = 256;
constexpr size_t APP_VERSION_MAX = 32;
constexpr size_t APP_TYPE_MAX = 32;
constexpr size_t APP_DATE_MAX = 32;
constexpr size_t APP_TITLE_MAX = 64;
constexpr size_t APP_BUILD_MAX = 256;

class App : UplinkObject
{
private:
	char path[APP_PATH_MAX];
	char usersPath[APP_PATH_MAX];
	char usersTempPath[APP_PATH_MAX];
	char usersOldPath[APP_PATH_MAX];
	char version[APP_VERSION_MAX];
	char type[APP_TYPE_MAX];
	char date[APP_DATE_MAX];
	char title[APP_TITLE_MAX];
	char build[APP_BUILD_MAX];
	int uptime;
	int closed;
	Options* options;
	Network* network;
	MainMenu* mainMenu;
	PhoneDialler* phoneDialler;
	char* nextLoadGame;
	int unused;

	App();
	virtual ~App();
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
	void Close();
	bool Closed();
	void CloseGame();
	void CoreDump();
	MainMenu* GetMainMenu();
	Network* GetNetwork();
	Options* GetOptions();
	void Initialise();
	/*DArray<char*>**/ void* ListExistingGames();
	void LoadGame(const char* username);
	void LoadGame();
	void RegisterPhoneDialler(PhoneDialler* phoneDialler);
	void RetireGame(const char* username);
	void SaveGame(const char* userName);
	void Set(const char* path, const char* version, const char* type, const char* date, const char* title);
	void SetNextLoadGame(const char *username);
	void UnRegisterPhoneDialler(PhoneDialler* phoneDialler);
};
