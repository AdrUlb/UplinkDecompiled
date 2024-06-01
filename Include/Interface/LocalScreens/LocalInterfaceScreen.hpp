#pragma once

#include <Button.hpp>
#include <Interface/InterfaceScreen.hpp>

class LocalInterfaceScreen : public InterfaceScreen
{
public:
	void Create() override;
	void Remove() override;
	bool IsVisible() override;

	static void BackgroundDraw(Button* button, bool highlighted, bool clicked);
	void CreateHeight(int value);
	int ScreenID();
};
