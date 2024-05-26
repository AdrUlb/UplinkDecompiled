#pragma once

#include <Button.hpp>
#include <InterfaceScreen.hpp>

class LocalInterfaceScreen : InterfaceScreen
{
public:
	void Create() override;
	void Remove() override;
	bool IsVisible() override;

	static void BackgroundDraw(Button* button, bool highlighted, bool clicked);
	void CreateHeight(int value);
};
