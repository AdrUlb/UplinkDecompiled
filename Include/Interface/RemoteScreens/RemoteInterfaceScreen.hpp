#pragma once

#include <Button.hpp>
#include <ComputerScreens/ComputerScreen.hpp>
#include <Interface/InterfaceScreen.hpp>

struct RemoteInterfaceScreen : InterfaceScreen
{
	ComputerScreen* computerScreen = nullptr;

	void Create() override;
	void Remove() override;
	virtual void Create(ComputerScreen* screen);
	virtual bool ReturnKeyPressed();
	virtual bool EscapeKeyPressed();
	static void DrawMainTitle(Button* button, bool highlighted, bool clicked);
	static void DrawSubTitle(Button* button, bool highlighted, bool clicked);
};
