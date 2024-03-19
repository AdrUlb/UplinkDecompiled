#pragma once

#include <DArray.hpp>
#include <MainMenu.hpp>
#include <Network.hpp>
#include <Options.hpp>
#include <PhoneDiallerScreen.hpp>
#include <UplinkObject.hpp>
#include <cstdint>

class App : UplinkObject
{
public:
	static constexpr size_t PATH_MAX = 0x100;
	static constexpr size_t VERSION_MAX = 0x20;
	static constexpr size_t TYPE_MAX = 0x20;
	static constexpr size_t DATE_MAX = 0x20;
	static constexpr size_t TITLE_MAX = 0x40;
	static constexpr size_t BUILD_MAX = 0x100;

	char path[PATH_MAX];
	char usersPath[PATH_MAX];
	char usersTempPath[PATH_MAX];
	char usersOldPath[PATH_MAX];
	char version[VERSION_MAX];
	char type[TYPE_MAX];
	char date[DATE_MAX];
	char title[TITLE_MAX];
	char build[BUILD_MAX];
private:
	int startTime;
	bool closed;
	Options* options;
	Network* network;
	MainMenu* mainMenu;
	PhoneDiallerScreen* phoneDiallerScreen;
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
	DArray<char*>* ListExistingGames();
	
	MainMenu* GetMainMenu();
	Network* GetNetwork();
	Options* GetOptions();

	void RegisterPhoneDialler(PhoneDiallerScreen* phoneDiallerScreen);
	void UnRegisterPhoneDialler(PhoneDiallerScreen* phoneDiallerScreen);
	
	static void CoreDump();

	inline Options* GetOptionsOrNull()
	{
		return options;
	}
};
