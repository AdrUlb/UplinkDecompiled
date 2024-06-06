#pragma once

#include <ComputerScreens/PasswordScreen.hpp>
#include <Interface/RemoteScreens/RemoteInterfaceScreen.hpp>

class PasswordScreenInterface : public RemoteInterfaceScreen
{
	void Update() override;
	void Remove() override;
	bool IsVisible() override;
	int ScreenID() override;
	void Create(ComputerScreen* screen) override;
	PasswordScreen* GetComputerScreen();
	void NextPage();
};
