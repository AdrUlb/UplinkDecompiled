#pragma once

#include <MainMenuScreens/MainMenuScreen.hpp>

class LoginInterface : public MainMenuScreen
{
public:
	void Create() override;
	void Remove() override;
	bool IsVisible() override;
	MainMenuScreenCode ScreenID() override;
	bool ReturnKeyPressed() override;
};
