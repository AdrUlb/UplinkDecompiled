#pragma once

#include <MainMenu.hpp>

class FirstTimeLoadingInterface : public MainMenuScreen
{
public:
	void Create() override;
	void Remove() override;
	void Update() override;
	bool IsVisible() override;
	MainMenuScreenCode ScreenID() override;
	bool ReturnKeyPressed() override;
};
