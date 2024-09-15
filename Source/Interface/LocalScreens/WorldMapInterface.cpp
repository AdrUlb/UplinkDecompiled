#include <Eclipse.hpp>
#include <Interface/LocalScreens/WorldMapInterface.hpp>

#include <GL/gl.h>
#include <Globals.hpp>
#include <Opengl.hpp>

static float zoom;
static float scrollX;
static float scrollY;

static uint32_t stipplepattern;
static float revelationColour;

static int GetLargeMapWidth()
{
	return app->GetOptions().GetOptionValue("graphics_screenwidth") - 46;
}

static int GetLargeMapHeight()
{
	return GetLargeMapWidth() / 595.0 * 316.0;
}

static WorldMapRect GetLargeMapRect()
{
	return {23, 50, GetLargeMapWidth(), GetLargeMapHeight()};
}

static int GetScaledX(const int value, const int arg2)
{
	UplinkAssert(value >= 0 && value < 594);

	if (arg2 == 1)
		return static_cast<int>(static_cast<float>(value) / 594.0f * static_cast<float>(EclGetButton("worldmap_smallmap")->Width));

	if (arg2 == 2)
		return static_cast<int>(static_cast<float>(value) / 594.0f * static_cast<float>(GetLargeMapWidth()) * zoom -
								static_cast<float>(GetLargeMapWidth()) * scrollX * zoom);

	return -1;
}

static int GetScaledY(const int value, const int arg2)
{
	UplinkAssert(value >= 0 && value < 315);

	if (arg2 == 1)
		return static_cast<int32_t>(static_cast<float>(value) / 315.0f * static_cast<float>(EclGetButton("worldmap_smallmap")->Height));

	if (arg2 == 2)
		return static_cast<int32_t>(static_cast<float>(value) / 315.0f * static_cast<float>(GetLargeMapHeight()) * zoom -
									static_cast<float>(GetLargeMapHeight()) * scrollY * zoom);

	return -1;
}

static void ConnectDraw(Button* button, bool highlighted, bool clicked)
{
	if (!game->GetWorld().GetPlayer().IsConnected() && WorldMapInterface::IsVisibleWorldMapInterface() != 2)
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

static void DrawWorldMapSmall(Button* button, bool highlighted, bool clicked)
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

			glVertex2i(GetScaledX(vloc->GetX(), 1) + button->X, GetScaledY(vloc->GetY(), 1) + button->Y);

			if (connection.TraceInProgress() != 0 && (connection.GetTraceProgress() == connection.GetVLocations().Size() - i - 1 &&
													  game->GetWorld().GetPlayer().GetGateway().HasHUDUpgrade(1) != 0))
			{
				glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
				glVertex2i(GetScaledX(vloc->GetX(), 1) + button->X, GetScaledY(vloc->GetY(), 1) + button->Y);
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

			const auto x = GetScaledX(vloc->GetX(), 1) + button->X;
			const auto y = GetScaledY(vloc->GetY(), 1) + button->Y;

			glVertex2i(x - 1, y - 1);
			glVertex2i(x + 2, y - 1);
			glVertex2i(x + 2, y + 2);
			glVertex2i(x - 1, y + 2);

			if (connection.GetTraceProgress() == connection.GetVLocations().Size() - i - 1 &&
				game->GetWorld().GetPlayer().GetGateway().HasHUDUpgrade(1))
				glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		}

		glEnd();
	}
	else
	{
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		const auto x = GetScaledX(game->GetWorld().GetPlayer().GetLocalHost()->GetX(), 1) + button->X;
		const auto y = GetScaledY(game->GetWorld().GetPlayer().GetLocalHost()->GetY(), 1) + button->Y;

		glBegin(GL_QUADS);
		glVertex2i(x - 1, y - 1);
		glVertex2i(x + 2, y - 1);
		glVertex2i(x + 2, y + 2);
		glVertex2i(x - 1, y + 2);
		glEnd();
	}

	int32_t i_2 = 0;
	glBegin(7);

	for (; i_2 < game->GetWorld().GetPlotGenerator().GetRevelationInfected().Size(); i_2 += 1)
	{
		const auto ip = game->GetWorld().GetPlotGenerator().GetRevelationInfected().GetData(i_2);
		UplinkAssert(ip != nullptr);

		const auto vloc = game->GetWorld().GetVLocation(ip);
		UplinkAssert(vloc != nullptr);

		const auto x = GetScaledX(vloc->GetX(), 1) + button->X;
		const auto y = GetScaledY(vloc->GetY(), 1) + button->Y;

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
		if (WorldMapInterface::IsVisibleWorldMapInterface() == 2)
		{
			WorldMapInterface::CreateWorldMapInterface(1);
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

		if (worldMapInterface == 2)
		{
			WorldMapInterface::RemoveWorldMapInterface();
			WorldMapInterface::CreateWorldMapInterface(2);
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
	if (!game->GetWorld().GetPlayer().IsConnected() && WorldMapInterface::IsVisibleWorldMapInterface() != 2)
		return;

	if (game->GetWorld().GetPlayer().IsConnected())
	{
		button->SetTooltip("Break the connection to your cur…");
		button_highlight(button);
		return;
	}

	button->SetTooltip("Establish the connection to your…");
	button_highlight(button);
}

WorldMapRect::WorldMapRect(const int x, const int y, const int width, const int height) : X(x), Y(y), Width(width), Height(height) {}

WorldMapRect::WorldMapRect(const WorldMapRect* rect) : X(rect->X), Y(rect->Y), Width(rect->Width), Height(rect->Height) {}

WorldMapObjectiveFunction::WorldMapObjectiveFunction(const WorldMapRect* rect) : _rect(rect)
{
	_width = rect->Width;
	_height = rect->Height;
	_array = new int[_width * _height];
	Reset();
}

WorldMapObjectiveFunction::~WorldMapObjectiveFunction()
{
	delete[] _array;
}

void WorldMapObjectiveFunction::Reset()
{
	memset(_array, 0, _width * _height * sizeof(int));
	_cost = 0;
}

WorldMapLayout::WorldMapLayout(const WorldMapRect* rect) : _rect(rect), _objectiveFunction(rect)
{
	Reset();
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

void WorldMapLayout::DeleteLocations()
{
	for (auto i = 0; i < _labels.Size(); i++)
		delete _labels.GetData(i);

	for (auto i = 0; i < _objects.Size(); i++)
		delete _objects.GetData(i);

	_labels.Empty();
	_objects.Empty();
}

void WorldMapLayout::AddLocation(int x, int y, const char* computerName, const char* ip, bool isUnknown)
{
	puts("TODO: implement WorldMapLayout::AddLocation()");
}

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
	Create(2);
}

void WorldMapInterface::Remove()
{
	RemoveWorldMapInterface();
}

bool WorldMapInterface::IsVisible()
{
	return IsVisibleWorldMapInterface() != 0;
}

int WorldMapInterface::ScreenID()
{
	return 4;
}

void WorldMapInterface::Create(int id)
{
	const auto rect = GetLargeMapRect();
	_layout = new WorldMapLayout(&rect);
	ProgramLayoutEngine();
	CreateWorldMapInterface(id);
	printf("TODO: implement WorldMapInterface::Create(%d)\n", id);
}

void WorldMapInterface::ProgramLayoutEngine()
{
	_layout->Reset();
	RemoveTempConnectionButton();
	auto& player = game->GetWorld().GetPlayer();
	const auto visibleMapInterface = IsVisibleWorldMapInterface();
	auto tempconCount = 0;

	auto& vlocs = player.GetConnection().GetVLocations();

	for (auto i = 0; i < vlocs.Size(); i++)
	{
		const auto ip = vlocs.GetData(i);
		const auto vloc = game->GetWorld().GetVLocation(ip);

		UplinkAssert(vloc != nullptr);

		const auto oldZoom = zoom;
		const auto oldScrollX = scrollX;
		const auto oldScrollY = scrollY;
		zoom = 1.0f;
		scrollX = 0.0f;
		scrollY = 0.0f;

		const auto isUnknown = i != 0 && (!vloc->GetDisplayed() || !player.HasLink(vloc->GetIp()));

		const auto scaledX = GetScaledX(vloc->GetX(), 2);
		const auto scaledY = GetScaledY(vloc->GetY(), 2);
		_layout->AddLocation(scaledX + 20, scaledY + 47, vloc->GetComputerName(), vloc->GetIp(), isUnknown);

		zoom = oldZoom;
		scrollX = oldScrollX;
		scrollY = oldScrollY;

		if (isUnknown)
		{
			if (visibleMapInterface == 2)
			{
				const auto rect = GetLargeMapRect();

				const auto x = rect.X;
				const auto y = rect.Y;

				char buttonName[0x80];
				UplinkSnprintf(buttonName, 0x80, "worldmaptempcon %d", tempconCount);

				char caption[0x80];
				UplinkStrncpy(caption, vloc->GetIp(), 0x80);

				char tooltip[0x80];
				UplinkSnprintf(tooltip, 0x80, "Connect to IP address %s", vloc->GetIp()) > 0x7f;

				EclRegisterButton(x + GetScaledX(vloc->GetX(), 2) - 3, y + GetScaledY(vloc->GetY(), 2) - 3, 7, 7, caption, tooltip, buttonName);
				// TODO: EclRegisterButtonCallbacks(buttonName, DrawLocation, LocationClick, button_click, button_highlight);
			}
			tempconCount++;
		}
	}

	auto& links = player.GetLinks();
	for (auto i = 0; i < links.Size(); i += 1)
	{
		const auto ip = links.GetData(i);
		const auto vloc = game->GetWorld().GetVLocation(ip);

		UplinkAssert(vloc != nullptr);

		if (!player.GetConnection().LocationIncluded(vloc->GetIp()) && vloc->GetDisplayed())
		{
			const auto oldZoom = zoom;
			const auto oldScrollX = scrollX;
			const auto oldScrollY = scrollY;

			zoom = 1.0f;
			scrollX = 0.0f;
			scrollY = 0.0f;

			_layout->AddLocation(GetScaledX(vloc->GetX(), 2) + 20, GetScaledY(vloc->GetY(), 2) + 47, vloc->GetComputerName(), vloc->GetIp(), false);

			zoom = oldZoom;
			scrollX = oldScrollX;
			scrollY = oldScrollY;
		}
	}

	// TODO: WorldMapInterface::UpdateAccessLevel();

	puts("TODO: implement WorldMapInterface::ProgramLayoutEngine()");
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
	if (IsVisibleWorldMapInterface() != 2)
		return;

	CreateWorldMapInterface(1);
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

void WorldMapInterface::CreateWorldMapInterface(const int type)
{
	if (IsVisibleWorldMapInterface() == type)
		return;

	RemoveWorldMapInterface();

	switch (type)
	{
		case 1:
			CreateWorldMapInterface_Small();
			break;
		case 2:
			CreateWorldMapInterface_Large();
			break;
		default:
			break;
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
