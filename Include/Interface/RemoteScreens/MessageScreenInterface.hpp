#pragma once

#include <Interface/RemoteScreens/RemoteInterfaceScreen.hpp>
#include <ComputerScreens/MessageScreen.hpp>

class MessageScreenInterface : public RemoteInterfaceScreen
{
	void Remove() override;
	bool IsVisible() override;
	int ScreenID() override;
	void Create(ComputerScreen* screen) override;
	bool ReturnKeyPressed() override;
	MessageScreen* GetComputerScreen();
	static void Click(Button* button);
	static void MailMeClick(Button* button);
};
