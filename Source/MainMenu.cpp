#include <MainMenu.hpp>

#include <Eclipse.hpp>
#include <Globals.hpp>
#include <MainMenuScreens/FirstTimeLoadingInterface.hpp>
#include <Opengl.hpp>
#include <RedShirt.hpp>
#include <Util.hpp>

bool MainMenu::IsVisible()
{
	return InScreen() != MainMenuScreenCode::Unknown;
}

void MainMenu::Remove()
{
	if (!IsVisible())
		return;

	if (screen == nullptr)
		return;

	screen->Remove();
	delete screen;
	screen = nullptr;
}

void MainMenu::RunScreen(MainMenuScreenCode code)
{
	(void)code;

	if (screen != nullptr)
	{
		screen->Remove();
		delete screen;
		screen = nullptr;
	}

	app->CloseGame();
	screenCode = code;

	switch (code)
	{
		case MainMenuScreenCode::FirstTimeLoading:
			screen = new FirstTimeLoadingInterface();
			break;
		default:
			printf("TODO: implement MainMenu::RunScreen(%d)\n", static_cast<int>(code));
			UplinkAbort("Tried to create a local screen with unknown SCREENCODE");
	}

	screen->Create();
}
