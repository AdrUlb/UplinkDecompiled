#include <Interface/LocalInterface.hpp>

#include <RedShirt.hpp>
#include <Sg.hpp>
#include <Svb.hpp>
#include <Util.hpp>

LocalInterface::~LocalInterface()
{
	if (_screen != nullptr)
		delete _screen;
	if (_hud != nullptr)
		delete _hud;
}

bool LocalInterface::Load(FILE* file)
{
	if (!FileReadData(&_screenCode, 4, 1, file))
		return false;

	if (!FileReadData(&_screenIndex, 4, 1, file))
		return false;

	if (!VerifyScreen(_screenCode, _screenIndex))
		return false;

	if (!GetHUD()->Load(file))
		return false;

	GetHUD()->Create();
	RunScreen(_screenCode, _screenIndex);

	return true;
}

void LocalInterface::Save(FILE* file)
{
	if (_screenCode <= 17)
	{
		auto buf = 0;
		fwrite(&buf, 4, 1, file);
	}
	else
	{
		fwrite(&_screenCode, 4, 1, file);
	}

	fwrite(&_screenIndex, 4, 1, file);
	GetHUD()->Save(file);
}

void LocalInterface::Print()
{
	printf("Local Interface : Current Screen Code = %d, Screen index = %d\n", _screenCode, _screenIndex);
}

void LocalInterface::Update()
{
	GetHUD()->Update();
	if (_screen != nullptr)
		_screen->Update();
}

const char* LocalInterface::GetID()
{
	return "LINT";
}

HUDInterface* LocalInterface::GetHUD()
{
	UplinkAssert(_hud != nullptr);
	return _hud;
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
	if (_screen != nullptr)
	{
		_screen->Remove();
		if (_screen != 0)
			delete _screen;
		_screen = nullptr;
	}

	if (WorldMapInterface::IsVisibleWorldMapInterface() == WorldMapInterfaceType::Large)
		WorldMapInterface::CloseWorldMapInterface_Large();

	_screenCode = code;
	_screenIndex = index;

	UplinkAssert(_hud != nullptr);

	HUDInterface::MoveSelecter(_screenCode, _screenIndex);

	switch (_screenCode)
	{
		case 0:
			SgPlaySound(RsArchiveFileOpen("sounds/close.wav"), "sounds/close.wav");
			break;
		/*case 3:
			_screen = new HWInterface();
			_screen->Create();
			break;
		case 6:
		{
			const auto screen = new MemoryInterface();
			_screen = screen;
			_screen->Create();
			break;
		}
		case 7:
			_screen = new StatusInterface();
			_screen->Create();
			break;
		case 8:
		{
			if (!game->GetWorld().GetPlayer().GetMessages().ValidIndex(_screenIndex))
			{
				_screenCode = 0;
				break;
			}
			const auto screen = new EmailInterface();
			_screen = screen;
			screen->SetMessage(_screenIndex);
			_screen->Create();
			break;
		}
		case 9:
		{
			const auto screen = new FinanceInterface();
			_screen = screen;
			_screen->Create();
		}
		case 10:
		{
			if (!game->GetWorld().GetPlayer().GetMissions().ValidIndex(_screenIndex))
			{
				_screenCode = 0;
				break;
			}
			const auto screen = new MissionInterface();
			_screen = screen;
			screen->SetMission(_screenIndex);
			_screen->Create();
			break;
		}
		case 11:
			_screen = new SendMailInterface();
			_screen->Create();
			break;
		case 12:
			_screen = new CheatInterface();
			_screen->Create();
			break;
		case 13:
			_screen = new EventQueueInterface();
			_screen->Create();
			break;
		case 14:
			_screen = new AnalyserInterface();
			_screen->Create();
			break;
		case 16:
			_screen = new IRCInterface();
			_screen->Create();
			break;
		case 17:
			_screen = new LanInterface();
			_screen->Create();*/
		case 1:
			_screen->Create();
			break;
		case 2:
			_screen->Create();
			break;
		case 4:
			_screen->Create();
			break;
		case 5:
			_screen->Create();
			break;
		case 15:
			_screen->Create();
			break;
		default:
			UplinkAbort("Tried to create a local screen with unknown SCREENCODE %d, code");
	}

	SvbShowAllTasks();
}

int LocalInterface::InScreen()
{
	if (_screen == nullptr)
		return 0;

	return _screen->ScreenID();
}

LocalInterfaceScreen* LocalInterface::GetInterfaceScreen()
{
	UplinkAssert(_screen != nullptr);
	return _screen;
}

bool LocalInterface::IsVisible()
{
	if (InScreen() == 0)
		return false;

	return _screen->IsVisible();
}

void LocalInterface::Create()
{
	GetHUD()->Create();
	RunScreen(_screenCode, _screenIndex);
}

void LocalInterface::Remove()
{
	if (!IsVisible())
		return;

	if (_screen == nullptr)
		return;

	_screen->Remove();

	if (_screen != 0)
		delete _screen;

	_screen = nullptr;
}

void LocalInterface::Reset()
{
	_screenCode = 0;
	_screenIndex = 0;
}