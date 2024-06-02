#pragma once

#include <ComputerScreens/GenericScreen.hpp>
#include <Interface/RemoteScreens/RemoteInterfaceScreen.hpp>

class CodeCardScreenInterface : public RemoteInterfaceScreen
{
	int codeVar1;
	int codeVar2;

	void Remove() override;
	bool IsVisible() override;
	int ScreenID() override;
	void Create(ComputerScreen* screen) override;
	bool ReturnKeyPressed() override;
	GenericScreen* GetComputerScreen();
	void ProceedClick(Button* button);
};
