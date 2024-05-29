#include <Interface.hpp>

#include <Eclipse.hpp>
#include <Globals.hpp>
#include <Util.hpp>

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
	SaveID_END(file);
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
	puts("TODO: implement RemoteInterface::RunScreen()");
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
	SaveID_END(file);
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
	SaveID_END(file);
}

void HUDInterface::Update()
{
	puts("TODO: implement HUDInterface::Update()");
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
		int32_t buf = 0;
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

Interface::Interface()
{
	puts("TODO: implement Interface::Interface()");
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
	puts("TODO: implement Interface::Update");
}

const char* Interface::GetID()
{
	return "INTERFA";
}

void Interface::Create()
{
	puts("TODO: implement Interface::Create");
}

LocalInterface* Interface::GetLocalInterface()
{
	UplinkAssert(localInterface != nullptr);
	return localInterface;
}
