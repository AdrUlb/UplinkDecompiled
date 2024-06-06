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
	char Path[APP_PATH_MAX];
	char UsersPath[APP_PATH_MAX];
	char UsersTempPath[APP_PATH_MAX];
	char UsersOldPath[APP_PATH_MAX];
	char Version[APP_VERSION_MAX];
	char Type[APP_TYPE_MAX];
	char Date[APP_DATE_MAX];
	char Title[APP_TITLE_MAX];
	char Build[APP_BUILD_MAX];

private:
	int _startTime;
	bool _closed;
	Options* _options;
	Network* _network;
	MainMenu* _mainMenu;
	PhoneDialler* _phoneDialler;
	char* _nextLoadGame;
	bool _checkCodecard;

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
	MainMenu& GetMainMenu();
	Network& GetNetwork();
	Options& GetOptions();
	bool GetCheckCodeCard();
	void RegisterPhoneDialler(PhoneDialler* phoneDiallerScreen);
	void UnRegisterPhoneDialler(PhoneDialler* phoneDiallerScreen);

	static void CoreDump();
	static DArray<char*>* ListExistingGames();

	inline Options* GetOptionsOrNull()
	{
		return _options;
	}
};
