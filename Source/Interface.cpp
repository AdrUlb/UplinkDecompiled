#include <Interface.hpp>

#include <Eclipse.hpp>
#include <GL/gl.h>
#include <Gci.hpp>
#include <Globals.hpp>
#include <Opengl.hpp>
#include <Svb.hpp>
#include <Util.hpp>
#include <Interface/RemoteScreens/MessageScreenInterface.hpp>

RemoteInterface::~RemoteInterface()
{
	if (screen != nullptr)
		delete screen;
}

bool RemoteInterface::Load(FILE* file)
{
	if (FileReadData(&previousScreenIndex, 4, 1, file) == 0)
		return false;

	if (!FileReadData(&screenIndex, 4, 1, file))
		return false;

	if (strcmp(game->GetLoadedSavefileVer(), "SAV59") < 0)
	{
		const auto success = FileReadData(&securityName, 0x80, 1, file);
		this->securityName[0x7f] = 0;
		if (!success)
			return false;
	}
	else
	{
		if (!LoadDynamicStringBuf(securityName, 0x80, file))
			return false;
	}

	if (!FileReadData(&securityLevel, 4, 1, file))
		return false;

	if (!VerifyScreen(screenIndex))
		return false;

	if (previousScreenIndex != 0 && !VerifyScreen(previousScreenIndex))
		return false;

	Create();
	return true;
}

void RemoteInterface::Save(FILE* file)
{
	fwrite(&previousScreenIndex, 4, 1, file);
	fwrite(&screenIndex, 4, 1, file);
	SaveDynamicString(securityName, 0x80, file);
	fwrite(&securityLevel, 4, 1, file);
}

void RemoteInterface::Print()
{
	printf("RemoteInterface : CurrentScreenIndex = %d\n, PreviousScreenIndex = %d\n, Security_Name = %s, Security_Level = %d\n", screenIndex,
		   previousScreenIndex, securityName, securityLevel);
}

void RemoteInterface::Update()
{
	if (screen != nullptr)
		screen->Update();
}

const char* RemoteInterface::GetID()
{
	return "RINT";
}

void RemoteInterface::Create()
{
	if (!IsVisible())
		RunScreen(screenIndex, nullptr);
}

bool RemoteInterface::VerifyScreen(int index)
{
	(void)index;
	puts("TODO: implement RemoteInterface::VerifyScreen()");
	return true;
}

bool RemoteInterface::IsVisible()
{
	if (screen != nullptr)
		return GetInterfaceScreen()->IsVisible();

	return false;
}

void RemoteInterface::RunScreen(int screenIndex, Computer* computer)
{
	const auto remoteComputer = game->GetWorld()->GetPlayer()->GetRemoteHost()->GetComputer();
	UplinkAssert(remoteComputer != nullptr);
	if (computer != nullptr && computer != remoteComputer)
	{
		printf(
			"RemoteInterface warning : Trying to run a screen on a different computer ('%s', %s) then the one currently connected to ('%s', %s).\n",
			computer->name, computer->ip, remoteComputer->name, remoteComputer->ip);
		return;
	}

	if (screen != nullptr)
	{
		screen->Remove();
		delete screen;
		screen = nullptr;
	}

	if (WorldMapInterface::IsVisibleWorldMapInterface() == 2)
		WorldMapInterface::CloseWorldMapInterface_Large();

	previousScreenIndex = this->screenIndex;
	this->screenIndex = screenIndex;

	const auto computerScreen = remoteComputer->GetComputerScreen(screenIndex);
	UplinkAssert(computerScreen != nullptr);

	const auto screenObjId = computerScreen->GetOBJECTID();

	switch (screenObjId)
	{
		case UplinkObjectId::MessageScreen:
			screen = new MessageScreenInterface();
			break;
		case UplinkObjectId::MenuScreen:
			puts("TODO: implement RemoteInterface::RunScreen(MenuScreen)");
			break;
		default:
			UplinkAbort("Unrecognised ComputerScreen %d, computer '%s' (%s)", screenObjId, remoteComputer->name, remoteComputer->ip);
	}
	UplinkAssert(screen != nullptr);

	screen->Create(computerScreen);
	SvbShowAllTasks();
}

RemoteInterfaceScreen* RemoteInterface::GetInterfaceScreen()
{
	UplinkAssert(screen != 0);
	return screen;
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

void SWInterface::Update()
{
	puts("TODO: implement SWInterface::Update()");
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

WorldMapInterface::~WorldMapInterface()
{
	if (layout != nullptr)
		delete layout;
}

bool WorldMapInterface::Load(FILE* file)
{
	if (strcmp(game->GetLoadedSavefileVer(), "SAV57") >= 0)
	{
		if (!LoadLList(&this->savedConnection, file))
			return false;
	}

	return true;
}

void WorldMapInterface::Save(FILE* file)
{
	SaveLList(&savedConnection, file);
}

void WorldMapInterface::Print()
{
	PrintLList(&savedConnection);
}

void WorldMapInterface::Update()
{
	puts("TODO: implement WorldMapInterface::Update()");
}

const char* WorldMapInterface::GetID()
{
	return "WRLDMAPI";
}

void WorldMapInterface::Create()
{
	puts("TODO: implement WorldMapInterface::Create()");
}

void WorldMapInterface::Remove()
{
	puts("TODO: implement WorldMapInterface::Remove()");
}

bool WorldMapInterface::IsVisible()
{
	puts("TODO: implement WorldMapInterface::IsVisible()");
	return false;
}

int WorldMapInterface::ScreenID()
{
	return 4;
}

int WorldMapInterface::IsVisibleWorldMapInterface()
{
	if (EclGetButton("worldmap_smallmap") != nullptr)
		return 1;

	if (EclGetButton("worldmap_largemap") != nullptr)
		return 2;

	return 0;
}

void WorldMapInterface::CloseWorldMapInterface_Large()
{
	if (WorldMapInterface::IsVisibleWorldMapInterface() != 2)
		return;

	WorldMapInterface::CreateWorldMapInterface(1);
}

void WorldMapInterface::CreateWorldMapInterface_Small()
{
	const auto screenWidth = app->GetOptions()->GetOptionValue("graphics_screenwidth");
	app->GetOptions()->GetOptionValue("graphics_screenheight");
	auto width = screenWidth * 0.29;
	auto height = width / 188.0 * 100.0;
	EclRegisterButton(screenWidth - width - 3, 3, width, height, "", "Global Communications", "worldmap_smallmap");
	if (game->GetWorldMapType() == 1)
		button_assignbitmap("worldmap_smallmap", "worldmapsmall_defcon.tif");
	else
		button_assignbitmap("worldmap_smallmap", "worldmapsmall.tif");

	EclGetButton("worldmap_smallmap")->ImageNormal->Scale(width, height);
	// EclRegisterButtonCallbacks("worldmap_smallmap", DrawWorldMapSmall, FullScreenClick, button_click, button_highlight);
	EclRegisterButton(screenWidth - width - 3, height + 4, width, 15, "", "", "worldmap_connect");
	// EclRegisterButtonCallbacks("worldmap_connect", ConnectDraw, ConnectClick, ConnectMouseDown, ConnectMouseMove);
	EclRegisterButton(screenWidth - 3, 0, 3, 3, "", "Global Communications", "worldmap_toprightclick");
	// EclRegisterButtonCallbacks("worldmap_toprightclick", nullptr, FullScreenClick, button_click, button_highlight);

	puts("TODO: implement WorldMapInterface::CreateWorldMapInterface_Small()");
}

void WorldMapInterface::CreateWorldMapInterface_Large()
{
	puts("TODO: implement WorldMapInterface::CreateWorldMapInterface_Large()");
}

void WorldMapInterface::CreateWorldMapInterface(int type)
{
	if (IsVisibleWorldMapInterface() != type)
	{
		RemoveWorldMapInterface();

		if (type == 1)
		{
			CreateWorldMapInterface_Small();
			return;
		}

		if (type == 2)
		{
			CreateWorldMapInterface_Large();
			return;
		}
	}
}

void WorldMapInterface::RemoveTempConnectionButton()
{

	for (auto i = 0; true; i++)
	{
		char buf[0x80];
		UplinkSnprintf(buf, 0x80, "worldmaptempcon %d", i);

		if (EclGetButton(buf) == nullptr)
			break;

		EclRemoveButton(buf);
	}
}

void WorldMapInterface::RemoveWorldMapInterface()
{
	auto visibleInterface = WorldMapInterface::IsVisibleWorldMapInterface();

	if (visibleInterface == 0)
		return;

	if (visibleInterface == 1)
	{
		EclRemoveButton("worldmap_smallmap");
		EclRemoveButton("worldmap_connect");
		EclRemoveButton("worldmap_toprightclick");
		return;
	}

	if (visibleInterface == 2)
	{
		EclRemoveButton("worldmap_largemap");
		EclRemoveButton("worldmap_saveconnection");
		EclRemoveButton("worldmap_loadconnection");
		EclRemoveButton("worldmap_zoomout");
		EclRemoveButton("worldmap_zoom");
		EclRemoveButton("worldmap_zoomin");
		EclRemoveButton("worldmap_connect");
		EclRemoveButton("worldmap_cancel");
		EclRemoveButton("worldmap_close");
		EclRemoveButton("worldmap_scrollleft");
		EclRemoveButton("worldmap_scrollright");
		EclRemoveButton("worldmap_scrollup");
		EclRemoveButton("worldmap_scrolldown");
		const auto links = &game->GetWorld()->GetPlayer()->links;

		for (auto i = 0; i < links->Size(); i++)
		{
			char* ip = links->GetData(i);
			char str[0x80];
			UplinkSnprintf(str, 0x80, "worldmap %s", game->GetWorld()->GetVLocation(ip)->ip);
			EclRemoveButton(str);
		}
		RemoveTempConnectionButton();
		EclRemoveButton("worldmap_texthelp");
		EclRemoveButton("worldmap 127.0.0.1");
	}
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
	static auto called = false;
	if (!called)
	{
		puts("TODO: HUDInterface::Update()");
		called = true;
	}
}

const char* HUDInterface::GetID()
{
	return "HUD_INT";
}

void HUDInterface::Create()
{
	puts("TODO: implement HUDInterface::Create()");
}

bool HUDInterface::IsVisible()
{
	return EclGetButton("hud_software") != nullptr;
}

int HUDInterface::ScreenID()
{
	return 1;
}

LocalInterface::~LocalInterface()
{
	if (screen != nullptr)
		delete screen;
	if (hud != nullptr)
		delete hud;
}

bool LocalInterface::Load(FILE* file)
{
	if (!FileReadData(&screenCode, 4, 1, file))
		return false;

	if (!FileReadData(&screenIndex, 4, 1, file))
		return false;

	if (!VerifyScreen(screenCode, screenIndex))
		return false;

	if (!GetHUD()->Load(file))
		return false;

	GetHUD()->Create();
	RunScreen(screenCode, screenIndex);

	return true;
}

void LocalInterface::Save(FILE* file)
{
	if (screenCode <= 17)
	{
		auto buf = 0;
		fwrite(&buf, 4, 1, file);
	}
	else
	{
		fwrite(&screenCode, 4, 1, file);
	}

	fwrite(&screenIndex, 4, 1, file);
	GetHUD()->Save(file);
}

void LocalInterface::Print()
{
	printf("Local Interface : Current Screen Code = %d, Screen index = %d\n", screenCode, screenIndex);
}

void LocalInterface::Update()
{
	GetHUD()->Update();
	if (screen != nullptr)
		screen->Update();
}

const char* LocalInterface::GetID()
{
	return "LINT";
}

HUDInterface* LocalInterface::GetHUD()
{
	UplinkAssert(hud != nullptr);
	return hud;
}

bool LocalInterface::VerifyScreen(int screenCode, int screenIndex)
{
	(void)screenIndex;
	if (screenCode <= 0x11)
	{
		// TODO: Figure out what each of the bits is
		if (((1 << screenCode) & 0b110111111111001001) != 0)
			return true;
	}
	printf("Print Abort: %s ln %d : ", __FILE__, __LINE__);
	printf("Tried to create a local screen with unknown SCREENCODE %d\n", screenCode);
	return false;
}

void LocalInterface::RunScreen(int code, int index)
{
	(void)code;
	(void)index;
	puts("TODO: implement Interface::RunScreen()");
}

Interface::~Interface()
{
	puts("TODO: implement Interface::~Interface()");
}

bool Interface::Load(FILE* file)
{
	(void)file;
	puts("TODO: implement Interface::Load");
	return false;
}

void Interface::Save(FILE* file)
{
	(void)file;
	puts("TODO: implement Interface::Save");
}

void Interface::Print()
{
	puts("TODO: implement Interface::Print");
}

void Interface::Update()
{
	GetLocalInterface()->Update();
	GetRemoteInterface()->Update();

	static auto called = false;
	if (!called)
	{
		puts("TODO: implement Interface::Update");
		called = true;
	}
}

const char* Interface::GetID()
{
	return "INTERFA";
}

void Interface::Create()
{
	Interface::GetRemoteInterface()->Create();

	if (!app->GetOptions()->IsOptionEqualTo("game_firsttime", 1))
	{
		puts("TODO: implement Interface::Create");
	}
}

LocalInterface* Interface::GetLocalInterface()
{
	UplinkAssert(localInterface != nullptr);
	return localInterface;
}

RemoteInterface* Interface::GetRemoteInterface()
{
	UplinkAssert(remoteInterface != nullptr);
	return remoteInterface;
}

TaskManager* Interface::GetTaskManager()
{
	UplinkAssert(taskManager != nullptr);
	return taskManager;
}