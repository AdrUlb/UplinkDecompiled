#include <Interface/RemoteInterface.hpp>

#include <Globals.hpp>
#include <Interface/RemoteScreens/DialogScreenInterface.hpp>
#include <Interface/RemoteScreens/MenuScreenInterface.hpp>
#include <Interface/RemoteScreens/MessageScreenInterface.hpp>
#include <Svb.hpp>
#include <Util.hpp>

RemoteInterface::~RemoteInterface()
{
	if (_screen != nullptr)
		delete _screen;
}

bool RemoteInterface::Load(FILE* file)
{
	if (FileReadData(&_previousScreenIndex, 4, 1, file) == 0)
		return false;

	if (!FileReadData(&_screenIndex, 4, 1, file))
		return false;

	if (strcmp(game->GetLoadedSavefileVer(), "SAV59") < 0)
	{
		const auto success = FileReadData(&_securityName, 0x80, 1, file);
		this->_securityName[0x7f] = 0;
		if (!success)
			return false;
	}
	else
	{
		if (!LoadDynamicStringBuf(_securityName, 0x80, file))
			return false;
	}

	if (!FileReadData(&_securityLevel, 4, 1, file))
		return false;

	if (!VerifyScreen(_screenIndex))
		return false;

	if (_previousScreenIndex != 0 && !VerifyScreen(_previousScreenIndex))
		return false;

	Create();
	return true;
}

void RemoteInterface::Save(FILE* file)
{
	fwrite(&_previousScreenIndex, 4, 1, file);
	fwrite(&_screenIndex, 4, 1, file);
	SaveDynamicString(_securityName, 0x80, file);
	fwrite(&_securityLevel, 4, 1, file);
}

void RemoteInterface::Print()
{
	printf("RemoteInterface : CurrentScreenIndex = %d\n, PreviousScreenIndex = %d\n, Security_Name = %s, Security_Level = %d\n", _screenIndex,
		   _previousScreenIndex, _securityName, _securityLevel);
}

void RemoteInterface::Update()
{
	if (_screen != nullptr)
		_screen->Update();
}

const char* RemoteInterface::GetID()
{
	return "RINT";
}

void RemoteInterface::Create()
{
	if (!IsVisible())
		RunScreen(_screenIndex, nullptr);
}

bool RemoteInterface::VerifyScreen(int index)
{
	(void)index;
	puts("TODO: implement RemoteInterface::VerifyScreen()");
	return true;
}

bool RemoteInterface::IsVisible()
{
	if (_screen != nullptr)
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
			computer->GetName(), computer->GetIp(), remoteComputer->GetName(), remoteComputer->GetIp());
		return;
	}

	if (_screen != nullptr)
	{
		_screen->Remove();
		delete _screen;
		_screen = nullptr;
	}

	if (WorldMapInterface::IsVisibleWorldMapInterface() == 2)
		WorldMapInterface::CloseWorldMapInterface_Large();

	_previousScreenIndex = this->_screenIndex;
	this->_screenIndex = screenIndex;

	const auto computerScreen = remoteComputer->GetComputerScreen(screenIndex);
	UplinkAssert(computerScreen != nullptr);

	const auto screenObjId = computerScreen->GetOBJECTID();

	switch (screenObjId)
	{
		case UplinkObjectId::MessageScreen:
			_screen = new MessageScreenInterface();
			break;
		case UplinkObjectId::MenuScreen:
			_screen = new MenuScreenInterface();
			break;
		case UplinkObjectId::DialogScreen:
			_screen = new DialogScreenInterface();
			break;
		default:
			UplinkAbort("Unrecognised ComputerScreen %d, computer '%s' (%s)", screenObjId, remoteComputer->GetName(), remoteComputer->GetIp());
	}
	UplinkAssert(_screen != nullptr);

	_screen->Create(computerScreen);
	SvbShowAllTasks();
}

int RemoteInterface::GetSecurityLevel()
{
	return _securityLevel;
}

RemoteInterfaceScreen* RemoteInterface::GetInterfaceScreen()
{
	UplinkAssert(_screen != 0);
	return _screen;
}
