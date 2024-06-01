#pragma once

#include <Button.hpp>
#include <ComputerScreens/MenuScreen.hpp>
#include <Interface/RemoteScreens/RemoteInterfaceScreen.hpp>

class MenuScreenInterface : public RemoteInterfaceScreen
{
	void Remove() override;
	bool IsVisible() override;
	int ScreenID() override;
	void Create(ComputerScreen* screen) override;
	MenuScreen* GetComputerScreen();
	static void DrawMenuOptionDimmed(Button* button, bool highlighted, bool clicked);
	static void ClickMenuScreenOption(Button* button);
	static void DrawMenuOption(Button* button, bool highlighted, bool clicked);
};
