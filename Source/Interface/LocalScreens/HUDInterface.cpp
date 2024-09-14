#include <Interface/LocalScreens/HUDInterface.hpp>

#include <Eclipse.hpp>
#include <GL/gl.h>
#include <Globals.hpp>
#include <Opengl.hpp>
#include <RedShirt.hpp>
#include <Sg.hpp>
#include <Svb.hpp>
#include <cstring>

static void AnalyserClick(Button*)
{
	puts("TODO: implement AnalyserClick()");
}

static void IRCClick(Button*)
{
	puts("TODO: implement IRCClick()");
}

static void LANClick(Button*)
{
	puts("TODO: implement LANClick()");
}

static HUDUpgrade hudUpgrades[8] = {
	{2, "Analyser", "Show the connection analyser", "hud_analyser", "hud/analyser.tif", "hud/analyser_h.tif", "hud/analyser_c.tif", AnalyserClick},
	{4, "CClient", "Show the IRC Client", "hud_ircclient", "hud/irc.tif", "hud/irc_h.tif", "hud/irc_c.tif", IRCClick},
	{8, "LANView", "Show the LAN Viewer", "hud_lanview", "hud/lan.tif", "hud/lan_h.tif", "hud/lan_c.tif", LANClick}};

static void ToolbarButtonDraw(Button* button, bool highlighted, bool clicked)
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	imagebutton_draw(button, highlighted, clicked);
}

static void SpeedButtonDraw(Button* button, bool highlighted, bool clicked)
{
	int speed;
	sscanf(button->Name, "hud_speed %d", &speed);
	if (game->GetGameSpeed() == speed)
	{
		imagebutton_draw(button, highlighted, true);
	}
	else
	{
		imagebutton_draw(button, highlighted, clicked);
	}
}

static void SoftwareClick(Button* button)
{
	(void)button;
	puts("TODO: implement SoftwareClick()");
}

static void HardwareClick(Button* button)
{
	(void)button;
	puts("TODO: implement HardwareClick()");
}

static void MemoryClick(Button* button)
{
	(void)button;
	puts("TODO: implement MemoryClick()");
}

static void StatusClick(Button* button)
{
	(void)button;
	puts("TODO: implement StatusClick()");
}

static void FinanceClick(Button* button)
{
	(void)button;
	puts("TODO: implement FinanceClick()");
}

static void SendMailClick(Button* button)
{
	(void)button;
	puts("TODO: implement SendMailClick()");
}

static void MainMenuClick(Button* button)
{
	(void)button;
	HUDInterface::CloseGame();
}

static void PauseButtonClick(Button* button)
{
	(void)button;
	puts("TODO: implement PauseButtonClick()");
}

static void SpeedButtonClick(Button* button)
{
	(void)button;
	puts("TODO: implement SpeedButtonClick()");
}

static void MissionClick(Button* button)
{
	(void)button;
	puts("TODO: implement MissionClick()");
}

static void MissionHighlight(Button* button)
{
	(void)button;
	puts("TODO: implement MissionHighlight()");
}

static void EmailClick(Button* button)
{
	(void)button;
	puts("TODO: implement EmailClick()");
}

static void EmailHighlight(Button* button)
{
	(void)button;
	puts("TODO: implement EmailHighlight()");
}

void SWInterface::Update()
{
	static auto called = false;
	if (!called)
	{
		puts("TODO: implement SWInterface::Update()");
		called = true;
	}
}

void SWInterface::Create()
{
	puts("TODO: implement SWInterface::Create()");
}

void SWInterface::Remove()
{
	puts("TODO: implement SWInterface::Remove()");
}

bool SWInterface::IsVisible()
{
	puts("TODO: implement SWInterface::IsVisible()");
	return false;
}

bool SWInterface::IsVisibleSoftwareMenu()
{
	return EclGetButton("hud_swmenu 1") != nullptr;
}

void SWInterface::ToggleSoftwareMenu()
{
	puts("TODO: implement SWInterface::ToggleSoftwareMenu()");
}

void GatewayInterface::Update()
{
	puts("TODO: implement GatewayInterface::Update()");
}

void GatewayInterface::Create()
{
	puts("TODO: implement GatewayInterface::Create()");
}

void GatewayInterface::Remove()
{
	puts("TODO: implement GatewayInterface::Remove()");
}

bool GatewayInterface::IsVisible()
{
	puts("TODO: implement GatewayInterface::IsVisible()");
	return false;
}

int GatewayInterface::ScreenID()
{
	return 15;
}

HUDInterface::~HUDInterface()
{
	if (highlightedToolbarButton != nullptr)
		delete[] highlightedToolbarButton;

	if (image != nullptr)
		delete image;
}

bool HUDInterface::Load(FILE* file)
{
	if (strcmp(game->GetLoadedSavefileVer(), "SAV57") >= 0)
	{
		if (!worldMapInterface.Load(file))
			return false;
	}

	return true;
}

void HUDInterface::Save(FILE* file)
{
	worldMapInterface.Save(file);
}

void HUDInterface::Update()
{
	if (!IsVisible())
		return;

	const auto screenWidth = app->GetOptions().GetOptionValue("graphics_screenwidth");
	const auto screenHeight = app->GetOptions().GetOptionValue("graphics_screenheight");

	char var_b8[0x80];
	UplinkStrncpy(var_b8, game->GetWorld().GetPlayer().GetRemoteHost()->GetIp(), 0x80);
	EclGetButton("hud_location")->SetCaption(var_b8);
	EclGetButton("hud_date")->SetCaption(game->GetWorld().GetCurrentDate().GetLongString());
	char* name;
	char var_1b8[0x80];

	auto messageX = screenWidth - 30;
	for (auto i = 0; i < game->GetWorld().GetPlayer().GetMessages().Size(); i++)
	{
		UplinkSnprintf(var_1b8, 0x80, "hud_message %d", i);

		if (EclGetButton(var_1b8) != nullptr)
			continue;

		EclRegisterButton(222, screenHeight - 41, 24, 24, "", "Read this message", var_1b8);
		button_assignbitmaps(var_1b8, "hud/email.tif", "hud/email_h.tif", "hud/email_c.tif");
		EclRegisterButtonCallbacks(var_1b8, imagebutton_draw, EmailClick, button_click, EmailHighlight);
		EclRegisterMovement(var_1b8, messageX, screenHeight - 41, 1000, nullptr);
		SgPlaySound(RsArchiveFileOpen("sounds/newmail.wav"), "sounds/newmail.wav");
		messageX -= 30;
	}

	char var_138[0x80];
	for (auto i = game->GetWorld().GetPlayer().GetMessages().Size(); true; i++)
	{
		UplinkSnprintf(var_138, 0x80, "hud_message %d", i);
		if (EclGetButton(var_138) == nullptr)
			break;

		EclRemoveButton(var_138);
	}

	auto missionX = screenWidth - 30 - (game->GetWorld().GetPlayer().GetMessages().Size() * 30);

	for (auto i = 0; i < game->GetWorld().GetPlayer().GetMissions().Size(); i++)
	{
		UplinkSnprintf(var_1b8, 0x80, "hud_mission %d", i);

		const auto button = EclGetButton(var_1b8);
		if (button == nullptr)
		{
			name = var_1b8;
			EclRegisterButton(222, screenHeight - 41, 24, 24, "", "View this mission", name);
			button_assignbitmaps(var_1b8, "hud/mission.tif", "hud/mission_h.tif", "hud/mission_c.tif");
			EclRegisterButtonCallbacks(var_1b8, imagebutton_draw, MissionClick, button_click, MissionHighlight);
			EclRegisterMovement(var_1b8, missionX, screenHeight - 41, 1000, nullptr);
			SgPlaySound(RsArchiveFileOpen("sounds/newmail.wav"), "sounds/newmail.wav");
		}
		else if (EclGetButton(var_1b8) != nullptr && missionX != EclGetButton(var_1b8)->X && EclIsAnimationActive(var_1b8) == -1)
			EclRegisterMovement(var_1b8, missionX, screenHeight - 41, 300, nullptr);

		missionX -= 30;
	}

	for (auto i = game->GetWorld().GetPlayer().GetMissions().Size(); true; i++)
	{
		UplinkSnprintf(var_138, 0x80, "hud_mission %d", i);
		if (EclGetButton(var_138) == nullptr)
			break;

		EclRemoveButton(var_138);
	}
	if (game->GetWorld().GetPlayer().GetGateway().HasHUDUpgrade(2) && !IsUpgradeVisible(2))
		AddUpgrade(2);

	if (game->GetWorld().GetPlayer().GetGateway().HasHUDUpgrade(4) && !IsUpgradeVisible(4))
		AddUpgrade(4);

	if (game->GetWorld().GetPlayer().GetGateway().HasHUDUpgrade(8) && !IsUpgradeVisible(8))
		AddUpgrade(8);

	worldMapInterface.Update();
	softwareInterface.Update();
	SvbUpdateInterface();
}

const char* HUDInterface::GetID()
{
	return "HUD_INT";
}

void HUDInterface::Create()
{
	if (IsVisible())
		return;

	const auto screenHeight = app->GetOptions().GetOptionValue("graphics_screenheight");
	worldMapInterface.Create(1);
	EclRegisterButton(3, screenHeight - 70, 53, 53, "", "Run a software application", "hud_software");
	button_assignbitmaps("hud_software", "hud/software.tif", "hud/software_h.tif", "hud/software_c.tif");
	EclRegisterButtonCallback("hud_software", SoftwareClick);
	EclRegisterButton(60, screenHeight - 42, 24, 24, "Hardware", "Show the hardware installed in your gateway", "hud_hardware");
	button_assignbitmaps("hud_hardware", "hud/hardware.tif", "hud/hardware_h.tif", "hud/hardware_c.tif");
	EclRegisterButtonCallbacks("hud_hardware", ToolbarButtonDraw, HardwareClick, button_click, button_highlight);
	EclRegisterButton(60, screenHeight - 42, 24, 24, "Memory", "Show your memory banks", "hud_memory");
	button_assignbitmaps("hud_memory", "hud/memory.tif", "hud/memory_h.tif", "hud/memory_c.tif");
	EclRegisterButtonCallbacks("hud_memory", ToolbarButtonDraw, MemoryClick, button_click, button_highlight);
	EclRegisterButton(60, screenHeight - 42, 24, 24, "Status", "Show your status in the World", "hud_status");
	button_assignbitmaps("hud_status", "hud/status.tif", "hud/status_h.tif", "hud/status_c.tif");
	EclRegisterButtonCallbacks("hud_status", ToolbarButtonDraw, StatusClick, button_click, button_highlight);
	EclRegisterButton(60, screenHeight - 42, 24, 24, "Finances", "Show your financial situation", "hud_finance");
	button_assignbitmaps("hud_finance", "hud/finance.tif", "hud/finance_h.tif", "hud/finance_c.tif");
	EclRegisterButtonCallbacks("hud_finance", ToolbarButtonDraw, FinanceClick, button_click, button_highlight);
	EclRegisterButton(60, screenHeight - 42, 24, 24, "New Mail", "Send an email", "hud_sendmail");
	button_assignbitmaps("hud_sendmail", "hud/newmail.tif", "hud/newmail_h.tif", "hud/newmail_c.tif");
	EclRegisterButtonCallbacks("hud_sendmail", ToolbarButtonDraw, SendMailClick, button_click, button_highlight);
	EclRegisterMovement("hud_hardware", 60, screenHeight - 42, 200, nullptr);
	EclRegisterMovement("hud_memory", 87, screenHeight - 42, 300, nullptr);
	EclRegisterMovement("hud_status", 114, screenHeight - 42, 400, nullptr);
	EclRegisterMovement("hud_finance", 141, screenHeight - 42, 500, nullptr);
	EclRegisterMovement("hud_sendmail", 168, screenHeight - 42, 600, nullptr);
	EclRegisterButton(3, 4, 13, 13, "", "Close any connections and log off", "hud_mainmenu");
	button_assignbitmaps("hud_mainmenu", "close.tif", "close_h.tif", "close_c.tif");
	EclRegisterButtonCallback("hud_mainmenu", MainMenuClick);
	EclRegisterButton(0, 0, 3, 3, "", "Close any connections and log off", "hud_topleftclick");
	EclRegisterButtonCallbacks("hud_topleftclick", nullptr, MainMenuClick, button_click, button_highlight);
	EclRegisterButton(20, 3, 147, 15, "", "Shows the current Date and Time", "hud_date");
	EclRegisterButton(170, 3, 90, 15, "", "Shows your current Uplink location", "hud_location");
	EclRegisterButtonCallbacks("hud_date", button_draw, 0, 0, nullptr);
	EclRegisterButtonCallbacks("hud_location", button_draw, 0, 0, nullptr);
	EclRegisterButton(263, 3, 15, 15, "", "Run at PAUSED speed", "hud_speed 0");
	button_assignbitmaps("hud_speed 0", "hud/speed0.tif", "hud/speed0_h.tif", "hud/speed0_c.tif");
	EclRegisterButtonCallbacks("hud_speed 0", SpeedButtonDraw, PauseButtonClick, button_click, button_highlight);
	EclRegisterButton(280, 3, 15, 15, "", "Run at NORMAL speed", "hud_speed 1");
	button_assignbitmaps("hud_speed 1", "hud/speed1.tif", "hud/speed1_h.tif", "hud/speed1_c.tif");
	EclRegisterButtonCallbacks("hud_speed 1", SpeedButtonDraw, SpeedButtonClick, button_click, button_highlight);
	EclRegisterButton(297, 3, 15, 15, "", "Run at FAST speed", "hud_speed 2");
	button_assignbitmaps("hud_speed 2", "hud/speed2.tif", "hud/speed2_h.tif", "hud/speed2_c.tif");
	EclRegisterButtonCallbacks("hud_speed 2", SpeedButtonDraw, SpeedButtonClick, button_click, button_highlight);
	EclRegisterButton(314, 3, 15, 15, "", "Run at VERYFAST speed", "hud_speed 3");
	button_assignbitmaps("hud_speed 3", "hud/speed3.tif", "hud/speed3_h.tif", "hud/speed3_c.tif");
	EclRegisterButtonCallbacks("hud_speed 3", SpeedButtonDraw, SpeedButtonClick, button_click, button_highlight);
	SvbCreateInterface(335, 15);
}

bool HUDInterface::IsVisible()
{
	return EclGetButton("hud_software") != nullptr;
}

int HUDInterface::ScreenID()
{
	return 1;
}

void HUDInterface::MoveSelecter(int screenCode, int screenIndex)
{
	(void)screenCode;
	(void)screenIndex;
	puts("TODO: implement HUDInterface::MoveSelecter()");
}

void HUDInterface::CloseGame()
{
	SgPlaySound(RsArchiveFileOpen("sounds/close.wav"), "sounds/close.wav");
	game->GetWorld().GetPlayer().GetConnection().Disconnect();
	game->GetWorld().GetPlayer().GetConnection().Reset();
	game->GetInterface().GetRemoteInterface().RunNewLocation();
	game->GetInterface().GetRemoteInterface().RunScreen(0, nullptr);
	app->SaveGame(game->GetWorld().GetPlayer().GetHandle());
	game->SetGameSpeed(0);
	EclReset();
	app->GetMainMenu().RunScreen(MainMenuScreenCode::Login);
}

HUDUpgrade* HUDInterface::GetUpgrade(char upgrade)
{
	for (auto i = 0; i < 8; i++)
		if (hudUpgrades[i].Id == upgrade)
			return &hudUpgrades[i];

	return nullptr;
}

bool HUDInterface::IsUpgradeVisible(char upgrade)
{
	for (auto i = 0; i < 8; i++)
	{
		if (_upgrades[i] == upgrade)
			return GetUpgrade(upgrade) != nullptr;
	}

	return false;
}

void HUDInterface::AddUpgrade(char upgrade)
{
	if (IsUpgradeVisible(upgrade))
		return;

	puts("TODO: implement HUDInterface::AddUpgrade()");
}
