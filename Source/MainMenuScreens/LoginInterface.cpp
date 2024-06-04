#include <MainMenuScreens/LoginInterface.hpp>

#include <Eclipse.hpp>
#include <GL/gl.h>
#include <Gci.hpp>
#include <Globals.hpp>
#include <Opengl.hpp>
#include <ScriptLibrary.hpp>
#include <Sg.hpp>

static void LargeTextBoxDraw(Button* button, bool highlighted, bool clicked)
{
	(void)highlighted;
	(void)clicked;
	SetColour("TitleText");
	GciDrawText(button->X, button->Y + (button->Height / 2) + 5, button->Caption, 7);
}

static void UserIDButtonDraw(Button* button, bool highlighted, bool clicked)
{
	UplinkAssert(button != nullptr);

	const auto screenHeight = app->GetOptions()->GetOptionValue("graphics_screenheight");
	glScissor(button->X, screenHeight - button->Y - button->Height, button->Width, button->Height);
	glEnable(GL_SCISSOR_TEST);

	SetColour("PasswordBoxBackground");
	glBegin(GL_QUADS);
	glVertex2i(button->X, button->Y);
	glVertex2i(button->Width + button->X - 1, button->Y);
	glVertex2i(button->Width + button->X - 1, button->Y + button->Height);
	glVertex2i(button->X, button->Y + button->Height);
	glEnd();

	SetColour("DefaultText");
	text_draw(button, highlighted, clicked);

	if (highlighted || clicked)
		border_draw(button);

	glDisable(GL_SCISSOR_TEST);
}

static void CodeButtonDraw(Button* button, bool highlighted, bool clicked)
{
	UplinkAssert(button != nullptr);

	const int screenHeight = app->GetOptions()->GetOptionValue("graphics_screenheight");

	glScissor(button->X, screenHeight + -button->Y - button->Height, button->Width, button->Height);
	glEnable(GL_SCISSOR_TEST);

	SetColour("PasswordBoxBackground");
	glBegin(GL_QUADS);
	glVertex2i(button->X, button->Y);
	glVertex2i(button->Width + button->X - 1, button->Y);
	glVertex2i(button->Width + button->X - 1, button->Y + button->Height);
	glVertex2i(button->X, button->Y + button->Height);
	glEnd();

	SetColour("DefaultText");
	const auto text = new char[strlen(button->Caption) + 1];
	const auto captionLength = strlen(button->Caption);
	for (size_t i = 0; i < captionLength; i++)
		text[i] = '*';
	text[captionLength] = 0;
	GciDrawText(button->X + 10, button->Y + 10, text, 2);
	delete[] text;

	if (highlighted != 0 || clicked != 0)
		border_draw(button);

	glDisable(GL_SCISSOR_TEST);
}

static void CommsClick(Button* button)
{
	(void)button;
	UplinkAssert(app->GetNetwork()->GetStatus() == 1);

	EclReset();

	UplinkAbort("TODO: implement LoginInterface::CommsClick()");
	// app->GetNetwork()->GetClient()->SetClientType(1);
}

static void StatusClick(Button* button)
{
	(void)button;
	UplinkAssert(app->GetNetwork()->GetStatus() == 1);

	EclReset();

	UplinkAbort("TODO: implement LoginInterface::CommsClick()");
	// app->GetNetwork()->GetClient()->SetClientType(2);
}

static void NewGameClick(Button* button)
{
	(void)button;
	tooltip_update(" ");

	app->GetOptions()->SetOptionValue("game_firsttime", 1);
	app->GetMainMenu()->RunScreen(MainMenuScreenCode::FirstTimeLoading);

	GciTimerFunc(2000, ScriptLibrary::RunScript, 30);

	if (app->GetOptions()->IsOptionEqualTo("sound_musicenabled", 1))
		SgPlaylist_Play("main");
}

static void RetireAgentClick(Button* button)
{
	(void)button;
	puts("TODO: implement LoginInterface::RetireAgentClick()");
}

static void OptionsClick(Button* button)
{
	(void)button;
	app->GetMainMenu()->RunScreen(MainMenuScreenCode::Options);
}

static void ExitGameClick(Button* button)
{
	(void)button;
	app->Close();
}

static void ProceedClick(Button* button)
{
	(void)button;

	UplinkAssert(EclGetButton("userid_name") != nullptr);

	char name[0x100];
	UplinkStrncpy(name, EclGetButton("userid_name")->Caption, 0x100);

	app->GetMainMenu()->RunScreen(MainMenuScreenCode::Loading);
	app->SetNextLoadGame(name);

	GciTimerFunc(1, ScriptLibrary::RunScript, 91);

	if (app->GetOptions()->IsOptionEqualTo("sound_musicenabled", 1))
		SgPlaylist_Play("main");
}

static void UserNameClick(Button* button)
{
	EclRegisterCaptionChange("userid_name", button->Caption, nullptr);
	EclRegisterCaptionChange("userid_code", "PASSWORD", nullptr);
}

static void CreateExistingGames()
{
	const auto games = App::ListExistingGames();

	auto yOffset = 0;

	for (auto i = 0; i < games->Size(); i++)
	{
		if (!games->ValidIndex(i))
			continue;

		const auto caption = games->GetData(i);

		char name[0x20];
		UplinkSnprintf(name, 0x20, "username %d", i);
		const auto x = GetScaledXPosition(170);
		const auto y = GetScaledYPosition(300) + yOffset;
		EclRegisterButton(GetScaledXPosition(35), y, x, 0xf, caption, "Log in as this Agent", name);
		EclRegisterButtonCallbacks(name, textbutton_draw, UserNameClick, button_click, button_highlight);

		delete[] caption;

		yOffset += 18;
	}

	delete games;
}

static void RemoveExistingGames()
{
	char name[0x20];

	for (auto i = 0; true; i++)
	{
		UplinkSnprintf(name, 0x20, "username %d", i);

		if (EclGetButton(name) == nullptr)
			break;

		EclRemoveButton(name);
	}
}

void LoginInterface::Create()
{
	if (IsVisible())
		return;

	if (app->GetOptions()->IsOptionEqualTo("sound_musicenabled", 1))
		SgPlaylist_Play("main");

	MainMenuScreen::Create();

	const auto screenWidth = app->GetOptions()->GetOptionValue("graphics_screenwidth");
	const auto screenHeight = app->GetOptions()->GetOptionValue("graphics_screenheight");

	if (app->GetNetwork()->GetStatus() == 1)
	{
		EclRegisterButton(20, 20, 110, 20, "COMMS", "Run as a dumb client, showing the communications status", "client_comms");
		EclRegisterButtonCallback("client_comms", CommsClick);
		EclRegisterButton(20, 50, 110, 20, "STATUS", "Run as a dumb client, showing your characters status", "client_status");
		EclRegisterButtonCallback("client_status", StatusClick);
	}

	const auto optionsX = GetScaledXPosition(600);
	const auto optionsY = GetScaledYPosition(270);
	EclRegisterButton(optionsX, optionsY, 32, 32, "", "Create a new user's account", "newgame");
	button_assignbitmaps("newgame", "mainmenu/newgame.tif", "mainmenu/newgame_h.tif", "mainmenu/newgame_c.tif");
	EclRegisterButton(optionsX - 95, optionsY + 5, 0x64, 20, "New User", "Create a new user's account", "newgame_text");
	EclRegisterButtonCallbacks("newgame_text", LargeTextBoxDraw, NewGameClick, button_click, button_highlight);
	EclRegisterButton(optionsX, optionsY + 40, 32, 32, "", "Remove agents from the roster", "retireagent");
	button_assignbitmaps("retireagent", "mainmenu/loadgame.tif", "mainmenu/loadgame_h.tif", "mainmenu/loadgame_c.tif");
	EclRegisterButton(optionsX - 68, optionsY + 45, 50, 20, "Retire", "Remove an agent from the roster", "retireagent_text");
	EclRegisterButtonCallbacks("retireagent_text", LargeTextBoxDraw, RetireAgentClick, button_click, button_highlight);

	if (app->GetNetwork()->GetStatus() == 1)
	{
		EclRegisterButtonCallbacks("newgame", imagebutton_draw, nullptr, nullptr, nullptr);
		EclGetButton("newgame")->ImageNormal->SetAlpha(0.5f);
		EclRegisterButtonCallbacks("retireagent", imagebutton_draw, nullptr, nullptr, nullptr);
		EclGetButton("retireagent")->ImageNormal->SetAlpha(0.5f);
	}
	else
	{
		EclRegisterButtonCallbacks("newgame", imagebutton_draw, NewGameClick, button_click, button_highlight);
		EclRegisterButtonCallbacks("retireagent", imagebutton_draw, RetireAgentClick, button_click, button_highlight);
	}

	EclRegisterButton(optionsX, optionsY + 80, 32, 32, "", "Advanced options", "options");
	button_assignbitmaps("options", "mainmenu/options.tif", "mainmenu/options_h.tif", "mainmenu/options_c.tif");
	EclRegisterButtonCallbacks("options", imagebutton_draw, OptionsClick, button_click, button_highlight);
	EclRegisterButton(optionsX - 81, optionsY + 85, 80, 20, "Options", "Advanced options", "options_text");
	EclRegisterButtonCallbacks("options_text", LargeTextBoxDraw, OptionsClick, button_click, button_highlight);
	EclRegisterButton(optionsX, optionsY + 120, 32, 32, "", "Exit Uplink", "exitgame");
	button_assignbitmaps("exitgame", "mainmenu/exitgame.tif", "mainmenu/exitgame_h.tif", "mainmenu/exitgame_c.tif");
	EclRegisterButtonCallbacks("exitgame", imagebutton_draw, ExitGameClick, button_click, button_highlight);
	EclRegisterButton(optionsX - 45, optionsY + 125, 50, 20, "Exit", "Exit Uplink", "exitgame_text");
	EclRegisterButtonCallbacks("exitgame_text", LargeTextBoxDraw, ExitGameClick, button_click, button_highlight);

	const auto loginX = GetScaledXPosition(320);
	const auto loginY = GetScaledYPosition(265);
	EclRegisterButton(loginX - 110, loginY, 220, 120, "", "", "userid_image");
	button_assignbitmap("userid_image", "userid.tif");
	EclRegisterButton(loginX - 51, loginY + 55, 147, 15, "", "Enter your userID here", "userid_name");
	EclRegisterButtonCallbacks("userid_name", UserIDButtonDraw, 0, button_click, button_highlight);
	EclRegisterButton(loginX - 51, loginY + 81, 147, 15, "", "Enter your access code here", "userid_code");
	EclRegisterButtonCallbacks("userid_code", CodeButtonDraw, 0, button_click, button_highlight);
	EclRegisterButton(loginX - 110, loginY + 125, 120, 15, "", "", "userid_message");
	EclRegisterButtonCallbacks("userid_message", textbutton_draw, nullptr, nullptr, nullptr);
	EclRegisterButton(loginX + 30, loginY + 125, 80, 15, "Proceed", "Click here when done", "userid_proceed");
	button_assignbitmaps("userid_proceed", "proceed.tif", "proceed_h.tif", "proceed_c.tif");
	EclRegisterButtonCallback("userid_proceed", ProceedClick);
	EclRegisterButton(loginX - 110, GetScaledYPosition(200), 240, GetScaledYPosition(50), "", "", "texthelp");
	EclRegisterButtonCallbacks("texthelp", textbutton_draw, nullptr, nullptr, nullptr);

	EclRegisterCaptionChange("texthelp",
							 "If you are registered as an Uplink Agent,\n"
							 "enter your username and password below.\n"
							 "Otherwise, click on the New User button.",
							 nullptr);
	EclRegisterButton(GetScaledXPosition(35), GetScaledYPosition(270), GetScaledXPosition(170), GetScaledYPosition(20), "Valid User Names", "",
					  "usernames_title");
	EclRegisterButtonCallbacks("usernames_title", LargeTextBoxDraw, 0, 0, nullptr);
	CreateExistingGames();
	EclMakeButtonEditable("userid_name");
	EclMakeButtonEditable("userid_code");
	EclRegisterButton(screenWidth - 40, screenHeight - 15, 40, 15, versionNumberString, "", "login_int_version");
	EclRegisterButtonCallbacks("login_int_version", textbutton_draw, nullptr, nullptr, nullptr);
}

void LoginInterface::Remove()
{
	remove_msgbox();

	if (!IsVisible())
		return;

	MainMenuScreen::Remove();
	if (app->GetNetwork()->GetStatus() == 1)
	{
		EclRemoveButton("client_comms");
		EclRemoveButton("client_status");
	}
	EclRemoveButton("newgame");
	EclRemoveButton("newgame_text");
	EclRemoveButton("retireagent");
	EclRemoveButton("retireagent_text");
	EclRemoveButton("options");
	EclRemoveButton("options_text");
	EclRemoveButton("exitgame");
	EclRemoveButton("exitgame_text");
	EclRemoveButton("userid_image");
	EclRemoveButton("userid_name");
	EclRemoveButton("userid_code");
	EclRemoveButton("userid_message");
	EclRemoveButton("userid_proceed");
	EclRemoveButton("login_int_version");
	EclRemoveButton("texthelp");
	EclRemoveButton("usernames_title");
	RemoveExistingGames();
}

bool LoginInterface::IsVisible()
{
	return EclGetButton("newgame") != 0;
}

MainMenuScreenCode LoginInterface::ScreenID()
{
	return MainMenuScreenCode::Login;
}

bool LoginInterface::ReturnKeyPressed()
{
	ProceedClick(nullptr);
	return true;
}
