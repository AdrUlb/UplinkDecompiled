#include <MainMenuScreens/MainMenuScreen.hpp>

#include <Eclipse.hpp>
#include <Globals.hpp>
#include <Opengl.hpp>
#include <Util.hpp>

MainMenuScreen::MainMenuScreen()
{
	buttons = new DArray<char*>();
}

MainMenuScreen::~MainMenuScreen()
{
	DeleteDArrayDataD(buttons);
	delete buttons;
	buttons = nullptr;
}

void MainMenuScreen::Create()
{
	app->GetOptions()->GetOptionValue("graphics_screenwidth");
	app->GetOptions()->GetOptionValue("graphics_screenheight");
	RegisterButton(GetScaledXPosition(320) - 170, 75, 425, 60, "", "mainmenu_background");
	button_assignbitmap("mainmenu_background", "mainmenu/uplinklogo.tif");
	EclRegisterButtonCallbacks("mainmenu_background", imagebutton_draw, 0, 0, nullptr);
}

void MainMenuScreen::Remove()
{
	UplinkAbort("TODO: implement MainMenuScreen::Remove");
}

void MainMenuScreen::Update() {}

bool MainMenuScreen::IsVisible()
{
	return false;
}
MainMenuScreenCode MainMenuScreen::ScreenID()
{
	return MainMenuScreenCode::Unknown;
}
bool MainMenuScreen::ReturnKeyPressed()
{
	return false;
}

void MainMenuScreen::RegisterButton(int x, int y, int width, int height, const char* caption, const char* name)
{
	EclRegisterButton(x, y, width, height, caption, name);
	const auto data = new char[strlen(name) + 1];
	strcpy(data, name);
	buttons->PutData(data);
}

MainMenu::MainMenu() : _screenCode(MainMenuScreenCode::Unknown), _screen(nullptr) {}

MainMenu::~MainMenu()
{
	if (_screen)
		delete _screen;
}

void MainMenu::Update()
{
	if (_screen != nullptr)
		_screen->Update();
}

const char* MainMenu::GetID()
{
	return "MMI";
}

void MainMenu::Create()
{
	UplinkAbort("TODO: implement MainMenu::Create()");
}

MainMenuScreen* MainMenu::GetMenuScreen()
{
	UplinkAbort("TODO: implement MainMenu::GetMenuScreen()");
}

MainMenuScreenCode MainMenu::InScreen()
{
	if (_screen == nullptr)
		return MainMenuScreenCode::Unknown;

	return _screen->ScreenID();
}
