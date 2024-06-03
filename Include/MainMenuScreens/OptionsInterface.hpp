#pragma once

#include <MainMenuScreens/MainMenuScreen.hpp>

class OptionsInterface : public MainMenuScreen
{
public:
	void Create() override;
	void Remove() override;
	bool IsVisible() override;
	MainMenuScreenCode ScreenID() override;
};
