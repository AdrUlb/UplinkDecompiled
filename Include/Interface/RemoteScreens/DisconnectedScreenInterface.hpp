#pragma once

#include <ComputerScreens/DisconnectedScreen.hpp>
#include <Interface/RemoteScreens/RemoteInterfaceScreen.hpp>

class DisconnectedScreenInterface : public RemoteInterfaceScreen
{
	void Create() override;
	void Remove() override;
	bool IsVisible() override;
	int ScreenID() override;
	void Create(ComputerScreen* screen) override;
	bool ReturnKeyPressed() override;
	[[nodiscard]] DisconnectedScreen* GetComputerScreen() const;
};
