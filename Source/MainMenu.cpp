#include <MainMenu.hpp>

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

MainMenu::MainMenu() : screenCode(MainMenuScreenCode::Unknown), screen(nullptr) {}

MainMenu::~MainMenu()
{
	if (screen)
		delete screen;
}

void MainMenu::Update()
{
	UplinkAbort("TODO: implement MainMenu::Update()");
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
	UplinkAbort("TODO: implement MainMenu::InScreen()");
}

bool MainMenu::IsVisible()
{
	UplinkAbort("TODO: implement MainMenu::IsVisible()");
}

void MainMenu::Remove()
{
	UplinkAbort("TODO: implement MainMenu::Remove()");
}

void MainMenu::RunScreen(MainMenuScreenCode code)
{
	(void)code;
	UplinkAbort("TODO: implement MainMenu::RunScreen(MainMenuScreenCode)");
}
