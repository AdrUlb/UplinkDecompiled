#pragma once

#include <ComputerScreens/DialogScreen.hpp>
#include <Interface/RemoteScreens/RemoteInterfaceScreen.hpp>

class DialogScreenInterface : public RemoteInterfaceScreen
{
	void Remove() override;
	bool IsVisible() override;
	int ScreenID() override;
	void Create(ComputerScreen* screen) override;
	bool ReturnKeyPressed() override;
	bool EscapeKeyPressed() override;
	DialogScreen* GetComputerScreen();
	void RemoveWidget(DialogScreenWidget* widget, Computer* computer);
	static void NextPageClick(Button* button);
	static void PasswordBoxDraw(Button* button, bool highlighted, bool clicked);
	static void ScriptButtonClick(Button* button);
};
