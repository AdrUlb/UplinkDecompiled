#include <MainMenu.hpp>

#include <Util.hpp>

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

void Create()
{
	UplinkAbort("TODO: implement MainMenu::Create()");
}

MainMenuScreen* GetMenuScreen()
{
	UplinkAbort("TODO: implement MainMenu::GetMenuScreen()");
}

MainMenuScreenCode InScreen()
{
	UplinkAbort("TODO: implement MainMenu::InScreen()");
}

bool IsVisible()
{
	UplinkAbort("TODO: implement MainMenu::IsVisible()");
}

void Remove()
{
	UplinkAbort("TODO: implement MainMenu::Remove()");
}

void RunScreen(MainMenuScreenCode code)
{
	(void)code;
	UplinkAbort("TODO: implement MainMenu::RunScreen(MainMenuScreenCode)");
}
