#pragma once

#include <DArray.hpp>
#include <MainMenu.hpp>
#include <Network.hpp>
#include <Options.hpp>
#include <PhoneDialler.hpp>
#include <UplinkObject.hpp>
#include <cstdint>

static constexpr size_t APP_PATH_MAX = 0x100;
static constexpr size_t APP_VERSION_MAX = 0x20;
static constexpr size_t APP_TYPE_MAX = 0x20;
static constexpr size_t APP_DATE_MAX = 0x20;
static constexpr size_t APP_TITLE_MAX = 0x40;
static constexpr size_t APP_BUILD_MAX = 0x100;

class App : UplinkObject
{
public:
	char path[APP_PATH_MAX];
	char usersPath[APP_PATH_MAX];
	char usersTempPath[APP_PATH_MAX];
	char usersOldPath[APP_PATH_MAX];
	char version[APP_VERSION_MAX];
	char type[APP_TYPE_MAX];
	char date[APP_DATE_MAX];
	char title[APP_TITLE_MAX];
	char build[APP_BUILD_MAX];

private:
	int startTime;
	bool closed;
	Options* options;
	Network* network;
	MainMenu* mainMenu;
	PhoneDialler* phoneDiallerScreen;
	char* nextLoadGame;
	bool checkCodecard;

public:
	App();
	~App() override;
	const char* GetID() override;
	void Print() override;
	void Update() override;

	void Initialise();
	void Set(const char* newPath, const char* newVersion, const char* newType, const char* newDate, const char* newTitle);

	void Close();
	void CloseGame();
	bool Closed();

	void SetNextLoadGame(char const* name);
	void LoadGame();
	void LoadGame(const char* name);
	void SaveGame(char const* name);
	void RetireGame(const char* name);

	MainMenu* GetMainMenu();
	Network* GetNetwork();
	Options* GetOptions();

	void RegisterPhoneDialler(PhoneDialler* phoneDiallerScreen);
	void UnRegisterPhoneDialler(PhoneDialler* phoneDiallerScreen);

	static void CoreDump();
	static DArray<char*>* ListExistingGames();

	inline Options* GetOptionsOrNull()
	{
		return options;
	}
};
