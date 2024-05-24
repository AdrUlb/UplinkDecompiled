#pragma once

#include <InterfaceScreen.hpp>

class LocalInterfaceScreen : InterfaceScreen
{
public:
	LocalInterfaceScreen();
	~LocalInterfaceScreen() override;
	void Update() override;
	void Create() override;
	void Remove() override;
	bool IsVisible() override;
	int ScreenID() override;
};
