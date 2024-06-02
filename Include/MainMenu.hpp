#pragma once

#include <MainMenuScreens/MainMenuScreen.hpp>
#include <UplinkObject.hpp>

class MainMenu : UplinkObject
{
	MainMenuScreenCode _screenCode;
	MainMenuScreen* _screen;

public:
	MainMenu();
	~MainMenu() override;

	void Update() override;
	const char* GetID() override;

	void Create();
	MainMenuScreen* GetMenuScreen();
	MainMenuScreenCode InScreen();
	bool IsVisible();
	void Remove();
	void RunScreen(MainMenuScreenCode code);
};
