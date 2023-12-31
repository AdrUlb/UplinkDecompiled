#pragma once

#include <cstddef>
#include <cstdio>
#include <UplinkObject.hpp>
#include <DArray.hpp>
#include <Options.hpp>
#include <Network.hpp>
#include <MainMenu.hpp>
#include <PhoneDialler.hpp>
#include <Graphics.hpp>

constexpr size_t APP_PATH_MAX = 0x100;
constexpr size_t APP_VERSION_MAX = 0x20;
constexpr size_t APP_TYPE_MAX = 0x20;
constexpr size_t APP_DATE_MAX = 0x20;
constexpr size_t APP_TITLE_MAX = 0x40;
constexpr size_t APP_BUILD_SIZE = 0x100;

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
	char Build[APP_BUILD_SIZE];

private:
	int uptime = 0;
	bool closed = false;
	Options* options = nullptr;
	Network* network = nullptr;
	MainMenu* mainMenu = nullptr;
	PhoneDialler* phoneDialler = nullptr;
	char* nextLoadGame = nullptr;
	bool _unknown = false;

public:
	App();
	virtual ~App();
private:
	void Print() override;
	void Update() override;
	const char* GetID() override;
public:
	void Set(const char* path, const char* version, const char* type, const char* date, const char* title);
	void Close();
	bool Closed();
	void CloseGame();
	static void CoreDump();
	MainMenu* GetMainMenu();
	Network* GetNetwork();
public:
	Options* GetOptions();
	Options* GetOptionsOrNull();
	void Initialise();
private:
	DArray<char*>* ListExistingGames();
	void LoadGame(const char* username);
	void LoadGame();
	void RegisterPhoneDialler(PhoneDialler* phoneDialler);
	void RetireGame(const char* username);
	void SaveGame(const char* userName);
	void SetNextLoadGame(const char* username);
	void UnRegisterPhoneDialler(PhoneDialler* phoneDialler);
};
