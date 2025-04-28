#include <Interface/RemoteScreens/LinksScreenInterface.hpp>

#include <Eclipse.hpp>
#include <GL/gl.h>
#include <Gci.hpp>
#include <Globals.hpp>
#include <Opengl.hpp>
#include <ScrollBox.hpp>

static Image* _ilink_tif = nullptr;
static Image* _ilink_h_tif = nullptr;
static Image* _ilink_c_tif = nullptr;
static Image* _iadd_tif = nullptr;
static Image* _iadd_h_tif = nullptr;
static Image* _iadd_c_tif = nullptr;

static int _baseoffset = 0;

void UpdateTooltip(struct Button* button, struct VLocation* vlocation)
{
	if (!vlocation->GetDisplayed())
	{
		button->SetTooltip("Toggle the visibility and the color of this link on the map");
		return;
	}

	if (vlocation->GetColored())
	{
		button->SetTooltip("Toggle the visibility of this link on the map");
		return;
	}

	button->SetTooltip("Toggle the color of this link on the map");
}

LinksScreenInterface::LinksScreenInterface()
{
	_ilink_tif = get_assignbitmap("link.tif");
	_ilink_h_tif = get_assignbitmap("link_h.tif");
	_ilink_c_tif = get_assignbitmap("link_c.tif");
	_iadd_tif = get_assignbitmap("add.tif");
	_iadd_h_tif = get_assignbitmap("add_h.tif");
	_iadd_c_tif = get_assignbitmap("add_c.tif");
}

LinksScreenInterface::~LinksScreenInterface()
{
	DeleteLListData(&_filterList);
	_filterList.Empty();

	DeleteLListData(&_displayList);
	_displayList.Empty();

	if (_ilink_tif != nullptr)
	{
		delete _ilink_tif;
		_ilink_tif = nullptr;
	}

	if (_ilink_h_tif != nullptr)
	{
		delete _ilink_h_tif;
		_ilink_h_tif = nullptr;
	}

	if (_ilink_c_tif != nullptr)
	{
		delete _ilink_c_tif;
		_ilink_c_tif = nullptr;
	}

	if (_iadd_tif != nullptr)
	{
		delete _iadd_tif;
		_iadd_tif = nullptr;
	}

	if (_iadd_h_tif != nullptr)
	{
		delete _iadd_h_tif;
		_iadd_h_tif = nullptr;
	}

	if (_iadd_c_tif != nullptr)
	{
		delete _iadd_c_tif;
		_iadd_c_tif = nullptr;
	}
}

void LinksScreenInterface::Remove()
{
	if (!IsVisible())
		return;

	EclRemoveButton("linksscreen_maintitle");
	EclRemoveButton("linksscreen_subtitle");
	EclRemoveButton("linksscreen_iptitle");
	EclRemoveButton("linksscreen_comptitle");

	for (auto i = 0; i < NumLinksOnScreen(); i++)
	{
		char buttonName[0x80];

		UplinkSnprintf(buttonName, 0x80, "linksscreen_link %d", i);
		EclRemoveButton(buttonName);

		if (GetComputerScreen()->GetScreenType() == 2)
		{
			UplinkSnprintf(buttonName, 0x80, "linksscreen_deletelink %d", i);
			EclRemoveButton(buttonName);

			UplinkSnprintf(buttonName, 0x80, "linksscreen_showlink %d", i);
			EclRemoveButton(buttonName);
		}
		else
		{
			UplinkSnprintf(buttonName, 0x80, "linksscreen_addlink %d", i);
			EclRemoveButton(buttonName);
		}
	}
	if (ScrollBox::GetScrollBox("linksscreen_scroll") != nullptr)
	{
		ScrollBox::RemoveScrollBox("linksscreen_scroll");
		EclRemoveButton("linksscreen_topright");
		EclRemoveButton("linksscreen_filter");
		EclRemoveButton("linksscreen_filtertext");
	}

	EclRemoveButton("linksscreen_close");
}

bool LinksScreenInterface::IsVisible()
{
	return EclGetButton("linksscreen_maintitle") != nullptr;
}

int LinksScreenInterface::ScreenID()
{
	return 7;
}

void LinksScreenInterface::Create(ComputerScreen* screen)
{
	UplinkAssert(screen);
	_screen = screen;

	if (IsVisible())
		return;

	EclRegisterButton(80, 60, 350, 25, GetComputerScreen()->GetMainTitle(), "", "linksscreen_maintitle");
	EclRegisterButtonCallbacks("linksscreen_maintitle", RemoteInterfaceScreen::DrawMainTitle, 0, 0, nullptr);
	EclRegisterButton(80, 80, 350, 20, GetComputerScreen()->GetSubTitle(), "", "linksscreen_subtitle");
	EclRegisterButtonCallbacks("linksscreen_subtitle", RemoteInterfaceScreen::DrawSubTitle, 0, 0, nullptr);
	struct Image* closeImage = get_assignbitmap("close.tif");
	struct Image* closeHighlightImage = get_assignbitmap("close_h.tif");
	struct Image* closeClickedImage = get_assignbitmap("close_c.tif");

	const auto numLinks = NumLinksOnScreen();
	int y = 145;
	for (auto i = 0; i < numLinks; i++)
	{
		char buf[0x80];
		UplinkSnprintf(buf, 0x80, "linksscreen_link %d", i);
		EclRegisterButton(30, y, GetScaledYPosition(375), 14, "", "Connect to this computer", buf);
		EclRegisterButtonCallbacks(buf, LinkDraw, LinkClick, LinkMouseDown, LinkMouseMove);

		if (GetComputerScreen()->GetScreenType() == 2)
		{
			UplinkSnprintf(buf, 0x80, "linksscreen_deletelink %d", i);
			EclRegisterButton(15, y, 13, 13, "", "Delete this link", buf);
			button_assignbitmaps(buf, closeImage, closeHighlightImage, closeClickedImage);
			EclRegisterButtonCallbacks(buf, DeleteLinkDraw, DeleteLinkClick, button_click, button_highlight);

			UplinkSnprintf(buf, 0x80, "linksscreen_showlink %d", i);
			EclRegisterButton((GetScaledYPosition(375) + 34), y, 13, 13, "", "", buf);
			EclRegisterButtonCallbacks(buf, ShowLinkDraw, ShowLinkClick, button_click, ShowLinkMouseMove);
		}
		else
		{
			UplinkSnprintf(buf, 0x80, "linksscreen_addlink %d", i);
			EclRegisterButton(15, y, 13, 13, "", "Store this link", buf);
			button_assignbitmaps(buf, _iadd_tif, _iadd_h_tif, _iadd_c_tif);
			EclRegisterButtonCallbacks(buf, AddLinkDraw, AddLinkClick, button_click, button_highlight);
		}

		y += 15;
	}

	if (closeImage != nullptr)
		delete closeImage;

	if (closeHighlightImage != nullptr)
		delete closeHighlightImage;

	if (closeClickedImage != nullptr)
		delete closeClickedImage;

	const auto vlocations = game->GetWorld().GetVLocations().ConvertToDArray();
	if (GetComputerScreen()->GetScreenType() == 1)
	{
		LList<const char*> displayedLinks;
		for (auto i = 0; i < vlocations->Size(); i++)
		{
			if (vlocations->ValidIndex(i) && vlocations->GetData(i)->GetListed())
				displayedLinks.PutData(vlocations->GetData(i)->GetIp());
		}
		SetFullList(&displayedLinks);
	}
	else if (GetComputerScreen()->GetScreenType() == 2)
	{
		SetFullList(&game->GetWorld().GetPlayer().GetLinks());
	}
	else if (GetComputerScreen()->GetScreenType() == 3)
	{
		LList<const char*> displayedLinks;
		const auto companyName = GetComputerScreen()->GetComputer()->GetCompanyName();

		for (auto i = 0; i < vlocations->Size(); i++)
		{
			if (!vlocations->ValidIndex(i))
				continue;

			if (vlocations->GetData(i)->GetOBJECTID() == UplinkObjectId::VlocationSpecial)
				continue;

			displayedLinks.PutData(vlocations->GetData(i)->GetIp());
		}

		SetFullList(&displayedLinks);
		ApplyFilter(companyName);
		SetFullList();
	}

	delete vlocations;

	CreateScrollBarAndFilter();
	if (GetComputerScreen()->GetNextPage() != -1)
	{
		EclRegisterButton(GetScaledYPosition(375) + 20, NumLinksOnScreen() * 15 + 150, 45, 15, "Close", "Close this screen", "linksscreen_close");
		EclRegisterButtonCallback("linksscreen_close", CloseClick);
	}
}

bool LinksScreenInterface::ReturnKeyPressed()
{
	puts("TODO: implement LinksScreenInterface::ReturnKeyPressed()");
	return false;
}

LinksScreen* LinksScreenInterface::GetComputerScreen()
{
	UplinkAssert(_screen != nullptr);
	return dynamic_cast<LinksScreen*>(_screen);
}

int LinksScreenInterface::NumLinksOnScreen()
{
	return (app->GetOptions().GetOptionValue("graphics_screenheight") - 265) / 15;
}

void LinksScreenInterface::SetFullList()
{
	DeleteLListData(&_displayList);
	_displayList.Empty();

	for (auto i = 0; i < _filterList.Size(); i++)
	{
		UplinkAssert(strlen(_filterList.GetData(i)) < 0x18);

		char* name = new char[0x18];
		strncpy(name, _filterList.GetData(i), 0x18);
		_displayList.PutData(name);
	}

	ApplyFilter(nullptr);
}

void LinksScreenInterface::SetFullList(LList<const char*>* links)
{
	UplinkAssert(links != nullptr);

	DeleteLListData(&_displayList);
	DeleteLListData(&_filterList);

	_displayList.Empty();
	_filterList.Empty();

	for (auto i = 0; i < links->Size(); i++)
	{
		UplinkAssert(strlen(links->GetData(i)) < 0x18);

		const auto link = new char[0x18];
		strncpy(link, links->GetData(i), 0x18);
		_displayList.PutData(link);
	}

	ApplyFilter(nullptr);
}

void LinksScreenInterface::SetFullList(LList<char*>* links)
{
	SetFullList(reinterpret_cast<LList<const char*>*>(links));
}

void LinksScreenInterface::ApplyFilter(const char* filter)
{
	DeleteLListData(&_filterList);
	_filterList.Empty();

	if (filter != nullptr)
	{
		const auto lowerFilter = LowerCaseString(filter);
		const auto filterLength = strlen(lowerFilter);

		for (auto i = 0; i < _displayList.Size(); i++)
		{
			const auto ip = _displayList.GetData(i);
			const auto vlocation = game->GetWorld().GetVLocation(ip);

			if (vlocation == nullptr)
				UplinkAbort("Cannot find location for ip %s", ip);

			const auto lowerComputerName = LowerCaseString(vlocation->GetComputerName());
			bool found;
			if (GetComputerScreen()->GetScreenType() == 3)
			{
				found = strncmp(lowerComputerName, lowerFilter, filterLength) == 0;
			}
			else
			{
				found = strstr(lowerComputerName, lowerFilter) != 0;
			}

			if (found)
			{
				const auto str = new char[0x18];
				UplinkStrncpy(str, ip, 0x18);
				_filterList.PutData(str);
			}

			if (lowerComputerName != 0)
				delete[] lowerComputerName;
		}

		delete[] lowerFilter;
	}
	else
	{
		for (auto i = 0; i < _displayList.Size(); i++)
		{
			const auto str = new char[0x18];
			UplinkStrncpy(str, _displayList.GetData(i), 0x18);
			_filterList.PutData(str);
		}
	}

	_baseoffset = 0;

	CreateScrollBarAndFilter();
	const auto scrollbox = ScrollBox::GetScrollBox("linksscreen_scroll");
	if (scrollbox != 0)
		scrollbox->SetNumItems(_filterList.Size());
}

void LinksScreenInterface::CreateScrollBarAndFilter()
{
	const auto numLinks = NumLinksOnScreen();
	if (_filterList.Size() >= NumLinksOnScreen() && ScrollBox::GetScrollBox("linksscreen_scroll") == nullptr)
	{
		const auto ipCount = _filterList.Size();
		ScrollBox::CreateScrollBox("linksscreen_scroll", GetScaledYPosition(375) + 50, 145, 15, numLinks * 15, ipCount, numLinks, 0, ScrollChange);
		EclRegisterButton((GetScaledYPosition(375) + 50), 125, 15, 15, "", "", "linksscreen_topright");
		EclRegisterButton(15, (NumLinksOnScreen() * 15) + 150, 135, 15, "Filter", "Click here to apply the filter", "linksscreen_filter");
		EclRegisterButtonCallback("linksscreen_filter", FilterClick);

		const auto scaledY = GetScaledYPosition(375);

		auto width = scaledY - 90;
		if (GetComputerScreen()->GetNextPage() != -1)
		{
			width = scaledY - 140;
		}

		EclRegisterButton(155, ((NumLinksOnScreen() * 15) + 150), width, 15, "", "Enter filter pattern here", "linksscreen_filtertext");
		EclRegisterButtonCallbacks("linksscreen_filtertext", FilterDraw, 0, button_click, button_highlight);
		EclMakeButtonEditable("linksscreen_filtertext");
	}

	if ((GetComputerScreen()->GetScreenType() != 2 || _filterList.Size() >= numLinks) && EclGetButton("linksscreen_iptitle") == nullptr)
	{
		EclRegisterButton(15, 125, 135, 15, "IP Address", "Shows the IP address of the location", "linksscreen_iptitle");
		EclRegisterButton(155, 125, (GetScaledYPosition(375) - 107), 15, "Location name", "Shows the computer name at that location", "linksscreen_comptitle");
	}
}

void LinksScreenInterface::AfterPhoneDialler(const char* ip)
{
	UplinkAssert(ip != nullptr);
	game->GetWorld().GetPlayer().GetConnection().Disconnect();
	game->GetWorld().GetPlayer().GetConnection().Reset();
	game->GetWorld().GetPlayer().GetConnection().AddVLocation(ip);
	game->GetWorld().GetPlayer().GetConnection().Connect();
	game->GetInterface().GetRemoteInterface().RunNewLocation();
}

void LinksScreenInterface::LinkDraw(Button* button, bool highlighted, bool clicked)
{
	(void)clicked;

	UplinkAssert(button != nullptr);

	clear_draw(button->X, button->Y, button->Width, button->Height);

	int index;
	sscanf(button->Name, "linksscreen_link %d", &index);

	index += _baseoffset;

	const auto linksScreen = dynamic_cast<LinksScreenInterface*>(game->GetInterface().GetRemoteInterface().GetInterfaceScreen());
	if (!linksScreen->_displayList.ValidIndex(index))
		return;

	char* ip = linksScreen->_displayList.GetData(index);
	if (ip != nullptr)
	{
		glBegin(GL_QUADS);
		if (index % 2 != 0)
		{
			SetColour("DarkPanelA");
			glVertex2i(button->X, button->Y);
			SetColour("DarkPanelB");
			glVertex2i(button->X + button->Width, button->Y);
			SetColour("DarkPanelA");
			glVertex2i(button->X + button->Width, button->Y + button->Height);
			SetColour("DarkPanelB");
			glVertex2i(button->X, button->Y + button->Height);
		}
		else
		{
			SetColour("DarkPanelB");
			glVertex2i(button->X, button->Y);
			SetColour("DarkPanelA");
			glVertex2i(button->X + button->Width, button->Y);
			SetColour("DarkPanelB");
			glVertex2i(button->X + button->Width, button->Y + button->Height);
			SetColour("DarkPanelA");
			glVertex2i(button->X, button->Y + button->Height);
		}
		glEnd();

		SetColour("DefaultText");
		GciDrawText(button->X + 10, button->Y + 10, ip);

		char text[0x40];
		if (game->GetWorld().GetVLocation(ip) == 0)
		{
			strncpy(text, "This link has expired", 0x40);
		}
		else
		{
			UplinkStrncpy(text, game->GetWorld().GetVLocation(ip)->GetComputerName(), 0x40);
		}

		GciDrawText(button->X + 120, button->Y + 10, text);
		if (highlighted)
		{
			SetColour("PanelHighlightBorder");
			border_draw(button);
		}
	}
}

void LinksScreenInterface::AddLinkDraw(Button* button, bool highlighted, bool clicked)
{
	UplinkAssert(button != nullptr);

	int index;
	sscanf(button->Name, "linksscreen_addlink %d", &index);

	index += _baseoffset;

	const auto linksScreen = dynamic_cast<LinksScreenInterface*>(game->GetInterface().GetRemoteInterface().GetInterfaceScreen());
	if (!linksScreen->_filterList.ValidIndex(index))
	{
		clear_draw(button->X, button->Y, button->Width, button->Height);
		return;
	}

	char* ip = linksScreen->_filterList.GetData(index);
	if (ip == nullptr)
	{
		clear_draw(button->X, button->Y, button->Width, button->Height);
		return;
	}

	if (game->GetWorld().GetPlayer().HasLink(ip))
	{
		clear_draw(button->X, button->Y, button->Width, button->Height);
		return;
	}

	imagebutton_draw(button, highlighted, clicked);
}

void LinksScreenInterface::DeleteLinkDraw(Button* button, bool highlighted, bool clicked)
{
	UplinkAssert(button != nullptr);

	int index;
	sscanf(button->Name, "linksscreen_deletelink %d", &index);

	index += _baseoffset;

	const auto linksScreen = dynamic_cast<LinksScreenInterface*>(game->GetInterface().GetRemoteInterface().GetInterfaceScreen());
	if (linksScreen->_filterList.ValidIndex(index) && linksScreen->_filterList.GetData(index) != nullptr)
	{
		imagebutton_draw(button, highlighted, clicked);
	}
	else
		clear_draw(button->X, button->Y, button->Width, button->Height);
}

void LinksScreenInterface::ShowLinkDraw(Button* button, bool highlighted, bool clicked)
{
	UplinkAssert(button != nullptr);

	int index;
	sscanf(button->Name, "linksscreen_showlink %d", &index);

	index += _baseoffset;

	const auto linksScreen = dynamic_cast<LinksScreenInterface*>(game->GetInterface().GetRemoteInterface().GetInterfaceScreen());
	if (!linksScreen->_filterList.ValidIndex(index))
	{
		clear_draw(button->X, button->Y, button->Width, button->Height);
		return;
	}

	const auto ip = linksScreen->_filterList.GetData(index);
	if (ip == nullptr)
	{
		clear_draw(button->X, button->Y, button->Width, button->Height);
		return;
	}

	const auto vlocation = game->GetWorld().GetVLocation(ip);
	if (!vlocation->GetDisplayed())
	{
		clear_draw(button->X, button->Y, button->Width, button->Height);
		return;
	}

	if (vlocation->GetColored())
	{
		imagebutton_draw(button, highlighted, clicked, _iadd_tif, _iadd_h_tif, _iadd_c_tif);
	}
	else
	{
		imagebutton_draw(button, highlighted, clicked, _ilink_tif, _ilink_h_tif, _ilink_c_tif);
	}
}

void LinksScreenInterface::FilterDraw(Button* button, bool highlighted, bool clicked)
{
	UplinkAssert(button != nullptr);

	textbutton_draw(button, highlighted, clicked);
	glColor3f(1.0f, 1.0f, 1.0f);
	border_draw(button);
}

void LinksScreenInterface::LinkMouseMove(Button* button)
{
	UplinkAssert(button != nullptr);

	int index;
	sscanf(button->Name, "linksscreen_link %d", &index);

	index += _baseoffset;

	if (dynamic_cast<LinksScreenInterface*>(game->GetInterface().GetRemoteInterface().GetInterfaceScreen())->_filterList.ValidIndex(index))
		button_highlight(button);
}

void LinksScreenInterface::ShowLinkMouseMove(Button* button)
{
	UplinkAssert(button != nullptr);

	int index;
	sscanf(button->Name, "linksscreen_showlink %d", &index);
	index += _baseoffset;

	const auto linksScreen = dynamic_cast<LinksScreenInterface*>(game->GetInterface().GetRemoteInterface().GetInterfaceScreen());
	const auto ip = linksScreen->_filterList.GetData(index);
	if (ip != nullptr)
		UpdateTooltip(button, game->GetWorld().GetVLocation(ip));

	button_highlight(button);
}

void LinksScreenInterface::LinkClick(Button* button)
{
	if (WorldMapInterface::IsVisibleWorldMapInterface() == WorldMapInterfaceType::Large)
		return;

	UplinkAssert(button != nullptr);

	int index;
	sscanf(button->Name, "linksscreen_link %d", &index);

	index += _baseoffset;
	const auto linksScreen = dynamic_cast<LinksScreenInterface*>(RemoteInterfaceScreen::GetInterfaceScreen(7));
	if (!linksScreen->_filterList.ValidIndex(index))
		return;

	const auto ip = linksScreen->_filterList.GetData(index);
	game->GetWorld().GetPlayer().GetConnection().Disconnect();
	game->GetWorld().GetPlayer().GetConnection().Reset();

	const auto dialler = new PhoneDialler();
	dialler->DialNumber(100, 100, ip, PhoneDiallerNextScene::Links, nullptr);
}

void LinksScreenInterface::DeleteLinkClick(Button* button)
{
	(void)button;
	puts("TODO: implement LinksScreenInterface::DeleteLinkClick()");
}

void LinksScreenInterface::ShowLinkClick(Button* button)
{
	if (WorldMapInterface::IsVisibleWorldMapInterface() == WorldMapInterfaceType::Large)
		return;

	UplinkAssert(button != nullptr);

	int index;
	sscanf(button->Name, "linksscreen_showlink %d", &index);

	index += _baseoffset;

	const auto linksScreen = dynamic_cast<LinksScreenInterface*>(RemoteInterfaceScreen::GetInterfaceScreen(7));
	char* ip = linksScreen->_filterList.GetData(index);
	if (ip == nullptr)
		return;

	const auto vlocation = game->GetWorld().GetVLocation(ip);
	if (!vlocation->GetDisplayed())
	{
		vlocation->SetDisplayed(true);
	}
	else if (vlocation->GetColored() == 0)
	{
		vlocation->SetColorDisplayed(true);
	}
	else
	{
		vlocation->SetDisplayed(false);
		vlocation->SetColorDisplayed(false);
	}

	UpdateTooltip(button, vlocation);
	tooltip_update(button->Tooltip);
}

void LinksScreenInterface::AddLinkClick(Button* button)
{
	if (WorldMapInterface::IsVisibleWorldMapInterface() == WorldMapInterfaceType::Large)
		return;

	UplinkAssert(button != nullptr);

	int32_t index;
	sscanf(button->Name, "linksscreen_addlink %d", &index);

	index += _baseoffset;
	const auto linksScreen = dynamic_cast<LinksScreenInterface*>(RemoteInterfaceScreen::GetInterfaceScreen(7));
	if (!linksScreen->_filterList.ValidIndex(index))
		return;

	const auto ip = linksScreen->_filterList.GetData(index);
	if (ip == nullptr)
		return;

	if (!game->GetWorld().GetPlayer().HasLink(ip))
		game->GetWorld().GetPlayer().GiveLink(ip);
}

void LinksScreenInterface::CloseClick(Button* button)
{
	(void)button;
	const auto linksScreen = dynamic_cast<LinksScreenInterface*>(RemoteInterfaceScreen::GetInterfaceScreen(7));
	UplinkAssert(linksScreen != nullptr);

	const auto nextPage = linksScreen->GetComputerScreen()->GetNextPage();

	if (nextPage == -1)
		return;

	const auto computer = linksScreen->GetComputerScreen()->GetComputer();
	game->GetInterface().GetRemoteInterface().RunScreen(nextPage, computer);
}

void LinksScreenInterface::LinkMouseDown(Button* button)
{
	UplinkAssert(button != nullptr);

	int index;
	sscanf(button->Name, "linksscreen_link %d", &index);

	index += _baseoffset;
	const auto linksScreen = dynamic_cast<LinksScreenInterface*>(game->GetInterface().GetRemoteInterface().GetInterfaceScreen());
	if (linksScreen->_filterList.ValidIndex(index) != 0)
		button_click(button);
}

void LinksScreenInterface::FilterClick(Button* button)
{
	UplinkAssert(button != nullptr);

	const auto linksScreen = dynamic_cast<LinksScreenInterface*>(game->GetInterface().GetRemoteInterface().GetInterfaceScreen());
	UplinkAssert(linksScreen != nullptr);
	const auto caption = EclGetButton("linksscreen_filtertext")->Caption;
	if (caption[0] != 0)
	{
		linksScreen->ApplyFilter(caption);
	}
	else
		linksScreen->ApplyFilter(nullptr);
}

void LinksScreenInterface::ScrollChange(const char* scrollboxName, int baseIndex)
{
	(void)scrollboxName;
	_baseoffset = baseIndex;
}
