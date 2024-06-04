#include <Interface/RemoteScreens/LinksScreenInterface.hpp>

#include <Eclipse.hpp>
#include <GL/gl.h>
#include <Gci.hpp>
#include <Globals.hpp>
#include <Opengl.hpp>
#include <ScrollBox.hpp>

static Image* ilink_tif = nullptr;
static Image* ilink_h_tif = nullptr;
static Image* ilink_c_tif = nullptr;
static Image* iadd_tif = nullptr;
static Image* iadd_h_tif = nullptr;
static Image* iadd_c_tif = nullptr;

static int baseoffset = 0;

LinksScreenInterface::LinksScreenInterface()
{
	ilink_tif = get_assignbitmap("link.tif");
	ilink_h_tif = get_assignbitmap("link_h.tif");
	ilink_c_tif = get_assignbitmap("link_c.tif");
	iadd_tif = get_assignbitmap("add.tif");
	iadd_h_tif = get_assignbitmap("add_h.tif");
	iadd_c_tif = get_assignbitmap("add_c.tif");
}

LinksScreenInterface::~LinksScreenInterface()
{
	DeleteLListData(&_filterList);
	_filterList.Empty();

	DeleteLListData(&_displayList);
	_displayList.Empty();

	if (ilink_tif != nullptr)
	{
		delete ilink_tif;
		ilink_tif = nullptr;
	}

	if (ilink_h_tif != nullptr)
	{
		delete ilink_h_tif;
		ilink_h_tif = nullptr;
	}

	if (ilink_c_tif != nullptr)
	{
		delete ilink_c_tif;
		ilink_c_tif = nullptr;
	}

	if (iadd_tif != nullptr)
	{
		delete iadd_tif;
		iadd_tif = nullptr;
	}

	if (iadd_h_tif != nullptr)
	{
		delete iadd_h_tif;
		iadd_h_tif = nullptr;
	}

	if (iadd_c_tif != nullptr)
	{
		delete iadd_c_tif;
		iadd_c_tif = nullptr;
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

	const auto numLinksOnScreen = NumLinksOnScreen();
	int y = 145;
	for (auto i = 0; i < numLinksOnScreen; i++)
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
			button_assignbitmaps(buf, iadd_tif, iadd_h_tif, iadd_c_tif);
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
	return (app->GetOptions()->GetOptionValue("graphics_screenheight") - 265) / 15;
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
	(void)filter;
	puts("TODO: LinksScreenInterface::ApplyFilter()");
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
		EclRegisterButton(155, 125, (GetScaledYPosition(375) - 107), 15, "Location name", "Shows the computer name at that location",
						  "linksscreen_comptitle");
	}
}

void LinksScreenInterface::LinkDraw(Button* button, bool highlighted, bool clicked)
{
	UplinkAssert(button != nullptr);

	clear_draw(button->X, button->Y, button->Width, button->Height);

	int index;
	sscanf(button->Name, "linksscreen_link %d", &index);

	index += baseoffset;
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

void LinksScreenInterface::DeleteLinkDraw(Button* button, bool highlighted, bool clicked)
{
	(void)button;
	(void)highlighted;
	(void)clicked;

	static auto called = false;
	if (!called)
	{
		puts("TODO: implement LinksScreenInterface::DeleteLinkDraw()");
		called = true;
	}
}

void LinksScreenInterface::ShowLinkDraw(Button* button, bool highlighted, bool clicked)
{
	(void)button;
	(void)highlighted;
	(void)clicked;

	static auto called = false;
	if (!called)
	{
		puts("TODO: implement LinksScreenInterface::ShowLinkDraw()");
		called = true;
	}
}

void LinksScreenInterface::AddLinkDraw(Button* button, bool highlighted, bool clicked)
{
	(void)button;
	(void)highlighted;
	(void)clicked;
	puts("TODO: implement LinksScreenInterface::AddLinkDraw()");
}

void LinksScreenInterface::FilterDraw(Button* button, bool highlighted, bool clicked)
{
	(void)button;
	(void)highlighted;
	(void)clicked;
	puts("TODO: implement LinksScreenInterface::FilterDraw()");
}

void LinksScreenInterface::LinkClick(Button* button)
{
	(void)button;
	puts("TODO: implement LinksScreenInterface::LinkClick()");
}

void LinksScreenInterface::DeleteLinkClick(Button* button)
{
	(void)button;
	puts("TODO: implement LinksScreenInterface::DeleteLinkClick()");
}

void LinksScreenInterface::ShowLinkClick(Button* button)
{
	(void)button;
	puts("TODO: implement LinksScreenInterface::ShowLinkClick()");
}

void LinksScreenInterface::AddLinkClick(Button* button)
{
	(void)button;
	puts("TODO: implement LinksScreenInterface::AddLinkClick()");
}

void LinksScreenInterface::CloseClick(Button* button)
{
	(void)button;
	puts("TODO: implement LinksScreenInterface::CloseClick()");
}

void LinksScreenInterface::LinkMouseDown(Button* button)
{
	(void)button;
	puts("TODO: implement LinksScreenInterface::LinkMouseDown()");
}

void LinksScreenInterface::LinkMouseMove(Button* button)
{
	(void)button;
	puts("TODO: implement LinksScreenInterface::LinkMouseMove()");
}

void LinksScreenInterface::ShowLinkMouseMove(Button* button)
{
	(void)button;
	puts("TODO: implement LinksScreenInterface::ShowLinkMouseMove()");
}

void LinksScreenInterface::FilterClick(Button* button)
{
	(void)button;
	puts("TODO: implement LinksScreenInterface::FilterClick()");
}

void LinksScreenInterface::ScrollChange(const char* scrollboxName, int currentIndex)
{
	(void)scrollboxName;
	(void)currentIndex;
	puts("TODO: implement LinksScreenInterface::ScrollChange()");
}
