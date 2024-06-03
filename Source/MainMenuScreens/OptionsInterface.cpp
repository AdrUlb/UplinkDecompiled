#include <MainMenuScreens/OptionsInterface.hpp>

#include <Eclipse.hpp>
#include <Globals.hpp>
#include <Opengl.hpp>

static void GameOptionsClick(Button* button)
{
	(void)button;
}

static void GraphicsOptionsClick(Button* button)
{
	(void)button;
	puts("TODO: implement OptionsInterface::GraphicsOptionsClick()");
}

static void SoundOptionsClick(Button* button)
{
	(void)button;
	puts("TODO: implement OptionsInterface::SoundOptionsClick()");
}

static void ReturnToMainMenuClick(Button* button)
{
	(void)button;
	app->GetMainMenu()->RunScreen(MainMenuScreenCode::Login);
}

static void ThemeOptionsClick(Button* button)
{
	(void)button;
	puts("TODO: implement OptionsInterface::ThemeOptionsClick()");
}

void OptionsInterface::Create()
{
	if (IsVisible())
		return;

	MainMenuScreen::Create();
	int32_t w = app->GetOptions()->GetOptionValue("graphics_screenwidth");
	int32_t h = app->GetOptions()->GetOptionValue("graphics_screenheight") - 50;
	EclRegisterButton(-832, h, 32, 32, "", "Edit GAME options", "gameoptions");
	EclRegisterButton(-672, h, 32, 32, "", "Edit GRAPHICS options", "graphicsoptions");
	EclRegisterButton(-512, h, 32, 32, "", "Edit SOUND options", "soundoptions");
	EclRegisterButton(-352, h, 32, 32, "", "Edit THEME options", "themeoptions");
	EclRegisterButton(-32, h, 32, 32, "", "Return to Main Menu", "mainmenu");
	button_assignbitmaps("gameoptions", "mainmenu/gameoptions.tif", "mainmenu/gameoptions_h.tif", "mainmenu/gameoptions_c.tif");
	button_assignbitmaps("graphicsoptions", "mainmenu/graphicsoptions.tif", "mainmenu/graphicsoptions_h.tif", "mainmenu/graphicsoptions_c.tif");
	button_assignbitmaps("soundoptions", "mainmenu/soundoptions.tif", "mainmenu/soundoptions_h.tif", "mainmenu/soundoptions_c.tif");
	button_assignbitmaps("themeoptions", "mainmenu/theme.tif", "mainmenu/theme_h.tif", "mainmenu/theme_c.tif");
	button_assignbitmaps("mainmenu", "mainmenu/exitgame.tif", "mainmenu/exitgame_h.tif", "mainmenu/exitgame_c.tif");
	EclRegisterButtonCallback("gameoptions", GameOptionsClick);
	EclRegisterButtonCallback("graphicsoptions", GraphicsOptionsClick);
	EclRegisterButtonCallback("soundoptions", SoundOptionsClick);
	EclRegisterButtonCallback("mainmenu", ReturnToMainMenuClick);
	EclRegisterButtonCallback("themeoptions", ThemeOptionsClick);
	EclRegisterMovement("mainmenu", (w - 40), h, 975, nullptr);
	EclRegisterMovement("themeoptions", (w - 80), h, 1200, nullptr);
	EclRegisterMovement("soundoptions", (w - 120), h, 1425, nullptr);
	EclRegisterMovement("graphicsoptions", (w - 160), h, 1650, nullptr);
	EclRegisterMovement("gameoptions", (w - 200), h, 1875, nullptr);
}

void OptionsInterface::Remove()
{
	if (!IsVisible())
		return;

	MainMenuScreen::Remove();
	EclRemoveButton("gameoptions");
	EclRemoveButton("graphicsoptions");
	EclRemoveButton("soundoptions");
	EclRemoveButton("themeoptions");
	EclRemoveButton("mainmenu");
}

bool OptionsInterface::IsVisible()
{
	return EclGetButton("gameoptions") != nullptr;
}

MainMenuScreenCode OptionsInterface::ScreenID()
{
	return MainMenuScreenCode::Options;
}
