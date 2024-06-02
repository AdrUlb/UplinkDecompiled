#pragma once

#include <ComputerScreens/GenericScreen.hpp>
#include <Interface/RemoteScreens/RemoteInterfaceScreen.hpp>

struct PhysicalGatewayLocation
{
	const char* City;
	const char* Country;
	int X;
	int Y;
};

class NearestGatewayScreenInterface : public RemoteInterfaceScreen
{
	void Remove() override;
	bool IsVisible() override;
	int ScreenID() override;
	void Create(ComputerScreen* screen) override;
	GenericScreen* GetComputerScreen();
	static void DrawBlack(Button* button, bool highlighted, bool clicked);
	static void DrawMainMap(Button* button, bool highlighted, bool clicked);
	static void DrawLocation(Button* button, bool highlighted, bool clicked);
	static void ClickLocation(Button* button);
};
