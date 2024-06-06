#include <Interface/RemoteInterface.hpp>

#include <ComputerScreens/GenericScreen.hpp>
#include <Globals.hpp>
#include <Interface/RemoteScreens/CodeCardScreenInterface.hpp>
#include <Interface/RemoteScreens/DialogScreenInterface.hpp>
#include <Interface/RemoteScreens/LinksScreenInterface.hpp>
#include <Interface/RemoteScreens/MenuScreenInterface.hpp>
#include <Interface/RemoteScreens/MessageScreenInterface.hpp>
#include <Interface/RemoteScreens/NearestGatewayScreenInterface.hpp>
#include <Interface/RemoteScreens/PasswordScreenInterface.hpp>
#include <LanComputer.hpp>
#include <LanMonitor.hpp>
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

ComputerScreen* RemoteInterface::GetComputerScreen()
{
	UplinkAssert(_screen != nullptr);
	return _screen->GetComputerScreen();
}

int RemoteInterface::GetScreenIndex()
{
	return _screenIndex;
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

void RemoteInterface::SetSecurity(const char* name, int level)
{
	UplinkStrncpy(_securityName, name, 0x80);
	_securityLevel = level;
}

void RemoteInterface::RunScreen(int screenIndex, Computer* computer)
{
	const auto remoteComputer = game->GetWorld().GetPlayer().GetRemoteHost()->GetComputer();
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
		case UplinkObjectId::GenericScreen:
		{
			const auto type = dynamic_cast<GenericScreen*>(computerScreen)->GetType();
			switch (type)
			{
				case 31:
					_screen = new NearestGatewayScreenInterface();
					break;
				case 33:
					_screen = new CodeCardScreenInterface();
					break;
				default:
					UplinkAbort("Unrecognised GenericScreen %d, computer '%s' (%s)", type, remoteComputer->GetName(), remoteComputer->GetIp());
					break;
			}
			break;
		}
		case UplinkObjectId::MessageScreen:
			_screen = new MessageScreenInterface();
			break;
		case UplinkObjectId::PasswordScreen:
			_screen = new PasswordScreenInterface();
			break;
		case UplinkObjectId::MenuScreen:
			_screen = new MenuScreenInterface();
			break;
		case UplinkObjectId::DialogScreen:
			_screen = new DialogScreenInterface();
			break;
		case UplinkObjectId::LinksScreen:
			_screen = new LinksScreenInterface();
			break;
		default:
			UplinkAbort("Unrecognised ComputerScreen %d, computer '%s' (%s)", screenObjId, remoteComputer->GetName(), remoteComputer->GetIp());
	}
	UplinkAssert(_screen != nullptr);

	_screen->Create(computerScreen);
	SvbShowAllTasks();
}

void RemoteInterface::RunNewLocation()
{
	if (_screen != nullptr)
	{
		_screen->Remove();
		delete _screen;
		_screen = nullptr;
	}

	_previousScreenIndex = 0;
	_screenIndex = 0;
	strncpy(_securityName, "Guest", 0x80);
	_securityLevel = 10;

	const auto vlocation = game->GetWorld().GetPlayer().GetRemoteHost();
	if (vlocation->GetOBJECTID() == UplinkObjectId::VlocationSpecial)
	{
		_screenIndex = dynamic_cast<VLocationSpecial*>(game->GetWorld().GetPlayer().GetRemoteHost())->GetScreenIndex();
	}
	else
	{
		const auto computer = game->GetWorld().GetComputer(vlocation->GetComputerName());
		if (computer != nullptr && computer->GetOBJECTID() == UplinkObjectId::LanComputer)
		{
			const auto lanComputer = dynamic_cast<LanComputer*>(computer);
			if (lanComputer->GetSystems().ValidIndex(LanMonitor::GetCurrentSelected()))
			{
				int32_t screenIndex = lanComputer->GetSystems().GetData(LanMonitor::GetCurrentSelected())->ScreenIndex;
				if (screenIndex != -1)
					_screenIndex = screenIndex;
			}
		}
	}

	Create();

	if (!GetComputerScreen()->GetComputer()->GetRunning())
	{
		game->GetWorld().GetPlayer().GetConnection().Disconnect();
		game->GetWorld().GetPlayer().GetConnection().Reset();
		game->GetInterface().GetRemoteInterface().RunNewLocation();
		game->GetInterface().GetRemoteInterface().RunScreen(8, nullptr);
	}

	if (!GetComputerScreen()->GetComputer()->GetIsExternallyOpen())
	{
		const auto ghostVlocation = game->GetWorld().GetVLocation(game->GetWorld().GetPlayer().GetConnection().GetGhost());
		UplinkAssert(ghostVlocation != nullptr);

		const auto ghostComputer = ghostVlocation->GetComputer();
		UplinkAssert(ghostComputer != nullptr);

		if (strcmp(ghostComputer->GetCompanyName(), GetComputerScreen()->GetComputer()->GetCompanyName()) != 0)
		{
			game->GetWorld().GetPlayer().GetConnection().Disconnect();
			game->GetWorld().GetPlayer().GetConnection().Reset();
			game->GetInterface().GetRemoteInterface().RunNewLocation();
			game->GetInterface().GetRemoteInterface().RunScreen(9, nullptr);
		}
	}
}
