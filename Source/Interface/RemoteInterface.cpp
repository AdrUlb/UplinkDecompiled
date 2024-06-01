#include <Interface/RemoteInterface.hpp>

#include <Globals.hpp>
#include <Interface/RemoteScreens/MenuScreenInterface.hpp>
#include <Interface/RemoteScreens/MessageScreenInterface.hpp>
#include <Svb.hpp>
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
			screen = new MenuScreenInterface();
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
