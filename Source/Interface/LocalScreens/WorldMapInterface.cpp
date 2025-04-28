#include <Eclipse.hpp>
#include <Interface/LocalScreens/WorldMapInterface.hpp>

#include <GL/gl.h>
#include <Globals.hpp>
#include <Opengl.hpp>

static uint32_t stipplepattern;
static float revelationColour;

int WorldMapInterface::GetLargeMapWidth()
{
	return app->GetOptions().GetOptionValue("graphics_screenwidth") - 46;
}

int WorldMapInterface::GetLargeMapHeight()
{
	return GetLargeMapWidth() / 595.0 * 316.0;
}

WorldMapRect WorldMapInterface::GetLargeMapRect()
{
	return { 23, 50, GetLargeMapWidth(), GetLargeMapHeight() };
}

int WorldMapInterface::GetScaledX(int x, WorldMapInterfaceType worldmapInterfaceType)
{
	UplinkAssert(x >= 0 && x < VirtualWidth);

	if (worldmapInterfaceType == WorldMapInterfaceType::Small)
		return x * EclGetButton("worldmap_smallmap")->Width / VirtualWidth;

	if (worldmapInterfaceType == WorldMapInterfaceType::Large)
		return _zoom * x * WorldMapInterface::GetLargeMapWidth() / VirtualWidth - _zoom * WorldMapInterface::GetLargeMapWidth() * _scrollX;

	return -1;
}

int WorldMapInterface::GetScaledY(int y, WorldMapInterfaceType worldmapInterfaceType)
{
	UplinkAssert(y >= 0 && y < VirtualHeight);

	if (worldmapInterfaceType == WorldMapInterfaceType::Small)
		return y * EclGetButton("worldmap_smallmap")->Height / VirtualHeight;

	if (worldmapInterfaceType == WorldMapInterfaceType::Large)
		return _zoom * y * WorldMapInterface::GetLargeMapHeight() / VirtualHeight - _zoom * WorldMapInterface::GetLargeMapHeight() * _scrollY;

	return -1;
}

static void ConnectDraw(Button* button, bool highlighted, bool clicked)
{
	if (!game->GetWorld().GetPlayer().IsConnected() && WorldMapInterface::IsVisibleWorldMapInterface() != WorldMapInterfaceType::Large)
	{
		clear_draw(button->X, button->Y, button->Width, button->Height);
		return;
	}

	if (game->GetWorld().GetPlayer().IsConnected())
		button->SetCaption("Disconnect");
	else
		button->SetCaption("Connect");

	button_draw(button, highlighted, clicked);
}

static void FullScreenClick(Button* button)
{
	(void)button;
	puts("TODO: implement FullScreenClick()");
}

static void ConnectClick(Button* button)
{
	(void)button;
	puts("TODO: implement ConnectClick()");

	if (!game->GetWorld().GetPlayer().IsConnected())
	{
		if (WorldMapInterface::IsVisibleWorldMapInterface() == WorldMapInterfaceType::Large)
		{
			WorldMapInterface::CreateWorldMapInterface(WorldMapInterfaceType::Small);
			const auto dialler = new PhoneDialler();
			dialler->DialNumber(0x64, 0x64, game->GetWorld().GetPlayer().GetConnection().GetTarget(), PhoneDiallerNextScene::WorldMap, nullptr);
		}
	}
	else
	{
		const auto worldMapInterface = WorldMapInterface::IsVisibleWorldMapInterface();

		game->GetWorld().GetPlayer().GetConnection().Disconnect();
		game->GetWorld().GetPlayer().GetConnection().Reset();
		game->GetInterface().GetRemoteInterface().RunNewLocation();
		game->GetInterface().GetRemoteInterface().RunScreen(1, nullptr);

		if (worldMapInterface == WorldMapInterfaceType::Large)
		{
			WorldMapInterface::RemoveWorldMapInterface();
			WorldMapInterface::CreateWorldMapInterface(WorldMapInterfaceType::Large);
		}
	}
}

static void ConnectMouseDown(Button* button)
{
	(void)button;
	puts("TODO: implement ConnectMouseDown()");
}

static void ConnectMouseMove(Button* button)
{
	if (!game->GetWorld().GetPlayer().IsConnected() && WorldMapInterface::IsVisibleWorldMapInterface() != WorldMapInterfaceType::Large)
		return;

	if (game->GetWorld().GetPlayer().IsConnected())
	{
		button->SetTooltip("Break the connection to your current remote host");
		button_highlight(button);
		return;
	}

	button->SetTooltip("Establish the connection to your current remote host");
	button_highlight(button);
}

WorldMapRect::WorldMapRect(const int x, const int y, const int width, const int height) : X(x), Y(y), Width(width), Height(height) {}

WorldMapRect::WorldMapRect(const WorldMapRect& rect) : X(rect.X), Y(rect.Y), Width(rect.Width), Height(rect.Height) {}

WorldMapObjectiveFunction::WorldMapObjectiveFunction(const WorldMapRect& rect) : _rect(rect)
{
	_width = rect.Width;
	_height = rect.Height;
	_array = new int[_width * _height];
	Reset();
}

WorldMapObjectiveFunction::~WorldMapObjectiveFunction()
{
	delete[] _array;
}

void WorldMapObjectiveFunction::Reset()
{
	memset(_array, 0, _width * _height * sizeof(_array[0]));
	_cost = 0;
}

WorldMapLayout::WorldMapLayout(const WorldMapRect& rect) : _rect(rect), _objectiveFunction(rect)
{
	Reset();
}

WorldMapLayout::~WorldMapLayout()
{
	DeleteLocations();
}

void WorldMapLayout::Reset()
{
	ResetLayoutParameters();
	DeleteLocations();
	_layoutComplete = true;
}

void WorldMapLayout::ResetLayoutParameters()
{
	_layoutStarted = false;
	_field_8 = 0;
	_field_4 = 0;
	_field_0 = 0;
	_field_c = 0.0f;
	_field_10 = -1.0f;
	_objectiveFunction.Reset();
}

void WorldMapLayout::AddLocation(int x, int y, const char* computerName, const char* ip, bool isUnknown)
{
	(void)x;
	(void)y;
	(void)computerName;
	(void)ip;
	(void)isUnknown;
	puts("TODO: implement WorldMapLayout::AddLocation()");
}

void WorldMapLayout::DeleteLocations()
{
	for (auto i = 0; i < _labels.Size(); i++)
		delete _labels[i];

	for (auto i = 0; i < _objects.Size(); i++)
		delete _objects[i];

	_labels.Empty();
	_objects.Empty();
}

WorldMapInterfaceObject::~WorldMapInterfaceObject()
{
	if (_ip != nullptr)
		delete[] _ip;
}

WorldMapRect WorldMapInterfaceObject::GetExtent()
{
	return WorldMapRect(_x, _y, 7, 7);
}

float WorldMapInterface::_zoom;
float WorldMapInterface::_scrollX;
float WorldMapInterface::_scrollY;

WorldMapInterface::~WorldMapInterface()
{
	if (_layout != nullptr)
		delete _layout;
}

bool WorldMapInterface::Load(FILE* file)
{
	if (strcmp(game->GetLoadedSavefileVer(), "SAV57") >= 0)
	{
		if (!LoadLList(&this->_savedConnection, file))
			return false;
	}

	return true;
}

void WorldMapInterface::Save(FILE* file)
{
	SaveLList(&_savedConnection, file);
}

void WorldMapInterface::Print()
{
	PrintLList(&_savedConnection);
}

void WorldMapInterface::Update()
{
	static auto called = false;
	if (!called)
	{
		puts("TODO: implement WorldMapInterface::Update()");
		called = true;
	}
}

const char* WorldMapInterface::GetID()
{
	return "WRLDMAPI";
}

void WorldMapInterface::Create()
{
	Create(WorldMapInterfaceType::Large);
}

void WorldMapInterface::Remove()
{
	RemoveWorldMapInterface();
}

bool WorldMapInterface::IsVisible()
{
	return IsVisibleWorldMapInterface() != WorldMapInterfaceType::None;
}

int WorldMapInterface::ScreenID()
{
	return 4;
}

void WorldMapInterface::Create(const WorldMapInterfaceType type)
{
	const auto& mapRect = GetLargeMapRect();
	_layout = new WorldMapLayout(mapRect);
	ProgramLayoutEngine();
	CreateWorldMapInterface(type);
}

void WorldMapInterface::ProgramLayoutEngine()
{
	_layout->Reset();

	RemoveTempConnectionButton();

	auto& player = game->GetWorld().GetPlayer();

	const auto isVisibleWorldMapInterface = IsVisibleWorldMapInterface();

	auto buttonIndex = 0;

	for (auto i = 0; i < player.GetConnection().GetVLocations().Size(); i += 1)
	{
		const auto ip = player.GetConnection().GetVLocations()[i];
		struct VLocation* vloc = game->GetWorld().GetVLocation(ip);

		UplinkAssert(vloc != nullptr);

		const auto oldZoom = _zoom;
		const auto oldScrollX = _scrollX;
		const auto oldScrollY = _scrollY;

		_zoom = 1;
		_scrollX = 0;
		_scrollY = 0;

		bool var_1e5_1;

		const auto vlocHidden = !vloc->GetDisplayed() || !player.HasLink(vloc->GetIp());

		var_1e5_1 = i != 0 && vlocHidden;

		int32_t rax_5 = GetScaledY(vloc->GetY(), WorldMapInterfaceType::Large);
		int32_t rax_6 = GetScaledX(vloc->GetX(), WorldMapInterfaceType::Large);
		_layout->AddLocation(rax_6 + 20, rax_5 + 47, vloc->GetComputerName(), vloc->GetIp(), var_1e5_1);

		_zoom = oldZoom;
		_scrollX = oldScrollX;
		_scrollY = oldScrollY;

		if (var_1e5_1)
		{
			if (isVisibleWorldMapInterface == WorldMapInterfaceType::Large)
			{
				const auto largeMapRect = GetLargeMapRect();
				char name[0x80];
				char caption[0x80];
				char tooltip[0x80];

				UplinkSnprintf(name, 0x80, "worldmaptempcon %d", (uint64_t)buttonIndex);
				UplinkStrncpy(caption, vloc->GetIp(), 0x80);
				UplinkSnprintf(tooltip, 0x80, "Connect to IP address %s", vloc->GetIp());

				EclRegisterButton(largeMapRect.X + WorldMapInterface::GetScaledX(vloc->GetX(), WorldMapInterfaceType::Large) - 3,
					largeMapRect.Y + WorldMapInterface::GetScaledY(vloc->GetY(), WorldMapInterfaceType::Large) - 3, 7, 7, caption, tooltip, name);

				EclRegisterButtonCallbacks(name, DrawLocation, LocationClick, button_click, button_highlight);
			}

			buttonIndex++;
		}
	}

	for (auto i = 0; i < player.GetLinks().Size(); i++)
	{
		const auto ip = player.GetLinks()[i];
		const auto vlocation = game->GetWorld().GetVLocation(ip);

		UplinkAssert(vlocation != nullptr);

		if (!player.GetConnection().LocationIncluded(vlocation->GetIp()) && vlocation->GetDisplayed())
		{
			const auto oldZoom = _zoom;
			const auto oldScrollX = _scrollX;
			const auto oldScrolly = _scrollY;

			_zoom = 1;
			_scrollX = 0;
			_scrollY = 0;

			const auto scaledX = WorldMapInterface::GetScaledX(vlocation->GetX(), WorldMapInterfaceType::Large);
			const auto scaledY = WorldMapInterface::GetScaledY(vlocation->GetY(), WorldMapInterfaceType::Large);

			_layout->AddLocation(scaledX + 20, scaledY + 47, vlocation->GetComputerName(), vlocation->GetIp(), false);

			_zoom = oldZoom;
			_scrollX = oldScrollX;
			_scrollY = oldScrolly;
		}
	}

	UpdateAccessLevel();
}

WorldMapInterfaceType WorldMapInterface::IsVisibleWorldMapInterface()
{
	if (EclGetButton("worldmap_smallmap") != nullptr)
		return WorldMapInterfaceType::Small;

	if (EclGetButton("worldmap_largemap") != nullptr)
		return WorldMapInterfaceType::Large;

	return WorldMapInterfaceType::None;
}

void WorldMapInterface::CloseWorldMapInterface_Large()
{
	if (WorldMapInterface::IsVisibleWorldMapInterface() != WorldMapInterfaceType::Large)
		return;

	WorldMapInterface::CreateWorldMapInterface(WorldMapInterfaceType::Small);
}

void WorldMapInterface::CreateWorldMapInterface_Small()
{
	const auto screenWidth = app->GetOptions().GetOptionValue("graphics_screenwidth");
	app->GetOptions().GetOptionValue("graphics_screenheight");
	const auto width = screenWidth * 0.29;
	const auto height = width / 188.0 * 100.0;
	EclRegisterButton(screenWidth - width - 3, 3, width, height, "", "Global Communications", "worldmap_smallmap");
	if (game->GetWorldMapType() == 1)
		button_assignbitmap("worldmap_smallmap", "worldmapsmall_defcon.tif");
	else
		button_assignbitmap("worldmap_smallmap", "worldmapsmall.tif");

	EclGetButton("worldmap_smallmap")->ImageNormal->Scale(width, height);
	EclRegisterButtonCallbacks("worldmap_smallmap", DrawWorldMapSmall, FullScreenClick, button_click, button_highlight);
	EclRegisterButton(screenWidth - width - 3, height + 4, width, 15, "", "", "worldmap_connect");
	EclRegisterButtonCallbacks("worldmap_connect", ConnectDraw, ConnectClick, ConnectMouseDown, ConnectMouseMove);
	EclRegisterButton(screenWidth - 3, 0, 3, 3, "", "Global Communications", "worldmap_toprightclick");
	// TODO: EclRegisterButtonCallbacks("worldmap_toprightclick", nullptr, FullScreenClick, button_click, button_highlight);

	puts("TODO: implement WorldMapInterface::CreateWorldMapInterface_Small()");
}

void WorldMapInterface::CreateWorldMapInterface_Large()
{
	puts("TODO: implement WorldMapInterface::CreateWorldMapInterface_Large()");
}

void WorldMapInterface::CreateWorldMapInterface(WorldMapInterfaceType type)
{
	if (type == IsVisibleWorldMapInterface())
		return;

	RemoveWorldMapInterface();

	if (type == WorldMapInterfaceType::Small)
	{
		CreateWorldMapInterface_Small();
		return;
	}

	if (type == WorldMapInterfaceType::Large)
	{
		CreateWorldMapInterface_Large();
		return;
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
	const auto visibleInterface = IsVisibleWorldMapInterface();

	if (visibleInterface == WorldMapInterfaceType::None)
		return;

	if (visibleInterface == WorldMapInterfaceType::Small)
	{
		EclRemoveButton("worldmap_smallmap");
		EclRemoveButton("worldmap_connect");
		EclRemoveButton("worldmap_toprightclick");
		return;
	}

	if (visibleInterface == WorldMapInterfaceType::Large)
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
		const auto links = &game->GetWorld().GetPlayer().GetLinks();

		for (auto i = 0; i < links->Size(); i++)
		{
			const auto ip = links->GetData(i);
			char str[0x80];
			UplinkSnprintf(str, 0x80, "worldmap %s", game->GetWorld().GetVLocation(ip)->GetIp());
			EclRemoveButton(str);
		}
		RemoveTempConnectionButton();
		EclRemoveButton("worldmap_texthelp");
		EclRemoveButton("worldmap 127.0.0.1");
	}
}

void WorldMapInterface::UpdateAccessLevel()
{
	puts("TODO: implement WorldMapInterface::UpdateAccessLevel()");
}

void WorldMapInterface::DrawWorldMapSmall(Button* button, bool highlighted, bool clicked)
{
	imagebutton_draw(button, highlighted, clicked);

	if (game->GetWorld().GetPlayer().IsConnected())
	{
		auto& connection = game->GetWorld().GetPlayer().GetConnection();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glLineWidth(2.0f);
		glLineStipple(1, stipplepattern);
		glEnable(GL_LINE_STIPPLE);
		glBegin(GL_LINE_STRIP);

		for (auto i = 0; i < connection.GetVLocations().Size(); i++)
		{
			const auto ip = connection.GetVLocations().GetData(i);
			const auto vloc = game->GetWorld().GetVLocation(ip);

			UplinkAssert(vloc != nullptr);

			glVertex2i(GetScaledX(vloc->GetX(), WorldMapInterfaceType::Small) + button->X, GetScaledY(vloc->GetY(), WorldMapInterfaceType::Small) + button->Y);

			if (connection.TraceInProgress() != 0 &&
				(connection.GetTraceProgress() == connection.GetVLocations().Size() - i - 1 && game->GetWorld().GetPlayer().GetGateway().HasHUDUpgrade(1) != 0))
			{
				glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
				glVertex2i(
					GetScaledX(vloc->GetX(), WorldMapInterfaceType::Small) + button->X, GetScaledY(vloc->GetY(), WorldMapInterfaceType::Small) + button->Y);
			}
		}

		glEnd();

		glLineWidth(1.0f);
		glDisable(GL_LINE_STIPPLE);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);

		for (auto i = 0; i < connection.GetVLocations().Size(); i++)
		{
			const auto ip = connection.GetVLocations().GetData(i);
			const auto vloc = game->GetWorld().GetVLocation(ip);

			UplinkAssert(vloc != nullptr);

			const auto x = GetScaledX(vloc->GetX(), WorldMapInterfaceType::Small) + button->X;
			const auto y = GetScaledY(vloc->GetY(), WorldMapInterfaceType::Small) + button->Y;

			glVertex2i(x - 1, y - 1);
			glVertex2i(x + 2, y - 1);
			glVertex2i(x + 2, y + 2);
			glVertex2i(x - 1, y + 2);

			if (connection.GetTraceProgress() == connection.GetVLocations().Size() - i - 1 && game->GetWorld().GetPlayer().GetGateway().HasHUDUpgrade(1))
				glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		}

		glEnd();
	}
	else
	{
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		const auto x = GetScaledX(game->GetWorld().GetPlayer().GetLocalHost()->GetX(), WorldMapInterfaceType::Small) + button->X;
		const auto y = GetScaledY(game->GetWorld().GetPlayer().GetLocalHost()->GetY(), WorldMapInterfaceType::Small) + button->Y;

		glBegin(GL_QUADS);
		glVertex2i(x - 1, y - 1);
		glVertex2i(x + 2, y - 1);
		glVertex2i(x + 2, y + 2);
		glVertex2i(x - 1, y + 2);
		glEnd();
	}

	glBegin(GL_QUADS);

	for (auto i = 0; i < game->GetWorld().GetPlotGenerator().GetRevelationInfected().Size(); i++)
	{
		const auto ip = game->GetWorld().GetPlotGenerator().GetRevelationInfected().GetData(i);
		UplinkAssert(ip != nullptr);

		const auto vloc = game->GetWorld().GetVLocation(ip);
		UplinkAssert(vloc != nullptr);

		const auto x = GetScaledX(vloc->GetX(), WorldMapInterfaceType::Small) + button->X;
		const auto y = GetScaledY(vloc->GetY(), WorldMapInterfaceType::Small) + button->Y;

		glColor4f(revelationColour, 0.0f, 0.0f, 1.0f);
		glVertex2i(x - 3, y - 3);
		glVertex2i(x + 4, y - 3);
		glVertex2i(x + 4, y + 4);
		glVertex2i(x - 3, y + 4);

		revelationColour -= 0.09f;

		if (revelationColour < 0.0f)
			revelationColour = 1.0f;
	}

	glEnd();

	if (highlighted || clicked)
		glColor4f(0.4f, 0.4f, 0.8f, 1.0f);
	else
		glColor3ub(81, 138, 215);

	border_draw(button);
}

void WorldMapInterface::DrawLocation(Button* button, bool highlighted, bool clicked)
{
	(void)button;
	(void)highlighted;
	(void)clicked;
	puts("TODO: implement WorldMapInterface::DrawLocation()");
}
void WorldMapInterface::LocationClick(Button* button)
{
	(void)button;
	puts("TODO: implement WorldMapInterface::LocationClick()");
}
