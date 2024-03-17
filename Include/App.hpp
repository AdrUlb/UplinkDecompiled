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
	int32_t startTime;
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

	void Close();
	void CloseGame();
	bool Closed();
	static void CoreDump();
	MainMenu* GetMainMenu();
	Network* GetNetwork();
	Options* GetOptions();
	void Initialise();
	DArray<char*>* ListExistingGames();
	void LoadGame();
	void LoadGame(const char* name);
	void RegisterPhoneDialler(PhoneDiallerScreen* phoneDiallerScreen);
	void RetireGame(const char* name);
	void SaveGame(char const* name);
	void Set(char* newPath, char* newVersion, char* newType, char* newDate, char* newTitle);
	void SetNextLoadGame(char const* name);
	void UnRegisterPhoneDialler(PhoneDiallerScreen* screen);

	inline bool OptionsValid()
	{
		return options != nullptr;
	}
};
