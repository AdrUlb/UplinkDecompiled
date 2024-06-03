#include <MainMenu.hpp>

#include <Eclipse.hpp>
#include <Globals.hpp>
#include <MainMenuScreens/FirstTimeLoadingInterface.hpp>
#include <MainMenuScreens/LoginInterface.hpp>
#include <MainMenuScreens/OptionsInterface.hpp>
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

	if (_screen == nullptr)
		return;

	_screen->Remove();
	delete _screen;
	_screen = nullptr;
}

void MainMenu::RunScreen(MainMenuScreenCode code)
{
	(void)code;

	if (_screen != nullptr)
	{
		_screen->Remove();
		delete _screen;
		_screen = nullptr;
	}

	app->CloseGame();
	_screenCode = code;

	switch (code)
	{
		case MainMenuScreenCode::Login:
			_screen = new LoginInterface();
			break;
		case MainMenuScreenCode::FirstTimeLoading:
			_screen = new FirstTimeLoadingInterface();
			break;
		case MainMenuScreenCode::Options:
			_screen = new OptionsInterface();
			break;
		default:
			printf("TODO: implement MainMenu::RunScreen(%d)\n", static_cast<int>(code));
			UplinkAbort("Tried to create a local screen with unknown SCREENCODE %d", static_cast<int>(code));
	}

	_screen->Create();
}
