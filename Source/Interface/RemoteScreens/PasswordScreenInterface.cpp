#include <Interface/RemoteScreens/PasswordScreenInterface.hpp>

#include <Eclipse.hpp>
#include <GL/gl.h>
#include <Gci.hpp>
#include <Globals.hpp>
#include <Opengl.hpp>
#include <RedShirt.hpp>
#include <Sg.hpp>

static void CodeButtonDraw(Button* button, bool highlighted, bool clicked)
{
	UplinkAssert(button != nullptr);

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

	if (!highlighted && !clicked)
		return;

	glBegin(GL_LINE_LOOP);
	glVertex2i(button->X, button->Y);
	glVertex2i(button->X + button->Width, button->Y);
	glVertex2i(button->X + button->Width, button->Y + button->Height);
	glVertex2i(button->X, button->Y + button->Height);
	glEnd();
}

static void PasswordClick(Button* button)
{
	UplinkAssert(button != nullptr);
	struct RemoteInterfaceScreen* rax_3 = game->GetInterface().GetRemoteInterface().GetInterfaceScreen();
	UplinkAssert(rax_3 != nullptr);
	game->GetInterface().GetTaskManager().SetProgramTarget(rax_3->GetComputerScreen(), button->Name, -1);
}

static void AccessCodeClick(Button* button)
{
	(void)button;
	puts("TODO: implement PasswordClick()");
}

void PasswordScreenInterface::Update()
{
	if (!game->GetWorld().GetPlayer().IsConnected())
		return;

	if (strcmp(EclGetButton("passwordscreen_password")->Caption, GetComputerScreen()->GetPassword()) == 0)
	{
		SgPlaySound(RsArchiveFileOpen("sounds/login.wav"), "sounds/login.wav");
		NextPage();
	}
}

void PasswordScreenInterface::Remove()
{
	if (!IsVisible())
		return;

	EclRemoveButton("passwordscreen_maintitle");
	EclRemoveButton("passwordscreen_subtitle");
	EclRemoveButton("passwordscreen_message");
	EclRemoveButton("passwordscreen_password");
	EclRemoveButton("passwordscreen_codestitle");
	EclRemoveButton("passwordscreen_codes");
}

bool PasswordScreenInterface::IsVisible()
{
	return EclGetButton("passwordscreen_message") != nullptr;
}

int PasswordScreenInterface::ScreenID()
{
	return 2;
}

void PasswordScreenInterface::Create(ComputerScreen* screen)
{
	UplinkAssert(screen != nullptr);
	_screen = screen;

	if (IsVisible())
		return;

	EclRegisterButton(150, 120, 350, 25, GetComputerScreen()->GetMainTitle(), "", "passwordscreen_maintitle");
	EclRegisterButtonCallbacks("passwordscreen_maintitle", DrawMainTitle, nullptr, nullptr, nullptr);
	EclRegisterButton(150, 140, 350, 20, GetComputerScreen()->GetSubTitle(), "", "passwordscreen_subtitle");
	EclRegisterButtonCallbacks("passwordscreen_subtitle", RemoteInterfaceScreen::DrawSubTitle, nullptr, nullptr, nullptr);
	EclRegisterButton(168, 185, 220, 110, "", "", "passwordscreen_message");
	button_assignbitmap("passwordscreen_message", "passwordscreen.tif");
	EclRegisterButton(206, 251, 145, 14, "", "Target this password box", "passwordscreen_password");
	EclRegisterButtonCallbacks("passwordscreen_password", CodeButtonDraw, PasswordClick, button_click, button_highlight);
	EclMakeButtonEditable("passwordscreen_password");

	const auto computer = _screen->GetComputer();
	UplinkAssert(computer != nullptr);

	const auto accessCode = game->GetWorld().GetPlayer().GetAccessCodes().LookupTree(computer->GetIp());
	if (accessCode != 0)
	{
		EclRegisterButton(200, 310, 250, 15, "Known Access Codes", "", "passwordscreen_codestitle");
		EclRegisterButtonCallbacks("passwordscreen_codestitle", textbutton_draw, nullptr, nullptr, nullptr);
		EclRegisterButton(200, 330, 250, 15, accessCode->Data, "Use this code", "passwordscreen_codes");
		EclRegisterButtonCallbacks("passwordscreen_codes", textbutton_draw, AccessCodeClick, button_click, button_highlight);
	}
}

PasswordScreen* PasswordScreenInterface::GetComputerScreen()
{
	UplinkAssert(_screen != nullptr);
	return dynamic_cast<PasswordScreen*>(_screen);
}

void PasswordScreenInterface::NextPage()
{
	game->GetInterface().GetRemoteInterface().SetSecurity("Admin", 1);

	const auto log = new AccessLog();
	const auto ghostIp = game->GetWorld().GetPlayer().GetConnection().GetGhost();
	log->SetProperties(game->GetWorld().GetCurrentDate(), ghostIp, "PLAYER", 0, 1);
	log->SetData1("Password authentication accepted");

	const auto computer = GetComputerScreen()->GetComputer();
	computer->GetLogBank().AddLog(log, -1);

	const auto nextPage = GetComputerScreen()->GetNextPage();
	if (nextPage == -1)
		return;

	game->GetInterface().GetRemoteInterface().RunScreen(nextPage, computer);
}
