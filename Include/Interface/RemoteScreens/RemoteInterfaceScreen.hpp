#pragma once

#include <Button.hpp>
#include <ComputerScreens/ComputerScreen.hpp>
#include <Interface/InterfaceScreen.hpp>

struct RemoteInterfaceScreen : InterfaceScreen
{
	ComputerScreen* _screen = nullptr;

	void Create() override;
	void Remove() override;
	virtual void Create(ComputerScreen* screen);
	virtual bool ReturnKeyPressed();
	virtual bool EscapeKeyPressed();
	static void DrawMainTitle(Button* button, bool highlighted, bool clicked);
	static void DrawSubTitle(Button* button, bool highlighted, bool clicked);
	ComputerScreen* GetComputerScreen();
	static RemoteInterfaceScreen* GetInterfaceScreen(int id);
};
