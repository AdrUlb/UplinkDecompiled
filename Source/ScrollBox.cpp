#include <ScrollBox.hpp>

#include <Eclipse.hpp>
#include <GL/gl.h>
#include <Opengl.hpp>
#include <Util.hpp>
#include <cstring>

static char currentGrab[0x100] = "None";

static LList<ScrollBox*> scrollBoxes;

void ScrollBox::SetName(const char* name)
{
	UplinkStrncpy(Name, name, 0x40);
}

void ScrollBox::SetDimensions(int x, int y, int width, int height)
{
	X = x;
	Y = y;
	Width = width;
	Height = height;
}

void ScrollBox::SetNumItems(int num)
{
	NumItems = num;

	if (num < CurrentIndex + WindowSize)
	{
		if (num >= WindowSize)
			CurrentIndex = num - WindowSize;
		else
			CurrentIndex = 0;

		if (Callback != nullptr)
			Callback(Name, CurrentIndex);
	}

	UpdateInterface();
}

void ScrollBox::UpdateInterface()
{
	// TODO: this function seemingly does nothing
}

void ScrollBox::SetWindowSize(int size)
{
	WindowSize = size;
	UpdateInterface();
}

void ScrollBox::SetCurrentIndex(int index)
{
	CurrentIndex = index;
	UpdateInterface();
}

void ScrollBox::SetCallback(ScrollChangeCallback callback)
{
	Callback = callback;
}

void ScrollBox::CreateInterface()
{
	char scrollUp[0x100];
	UplinkSnprintf(scrollUp, 0x100, "scrollup %s", Name);

	char scrollDown[0x100];
	UplinkSnprintf(scrollDown, 0x100, "scrolldown %s", Name);

	char scrollBar[0x100];
	UplinkSnprintf(scrollBar, 0x100, "scrollbar %s", Name);

	EclRegisterButton(X, Y, 15, 15, "^", scrollUp);
	EclRegisterButton(X, Y + 15, 15, Height - 30, " ", scrollBar);
	EclRegisterButton(X, Height + Y - 15, 15, 15, "v", scrollDown);
	button_assignbitmaps(scrollUp, "up.tif", "up_h.tif", "up_c.tif");
	button_assignbitmaps(scrollDown, "down.tif", "down_h.tif", "down_c.tif");
	EclRegisterButtonCallback(scrollUp, ClickScrollUp);
	EclRegisterButtonCallback(scrollDown, ClickScrollDown);
	EclRegisterButtonCallbacks(scrollBar, DrawScrollBar, ClickScrollBar, MouseDownScrollBar, MouseMoveScrollBar);
	InterfaceCreated = true;
}

void ScrollBox::RemoveInterface()
{
	if (!InterfaceCreated)
		return;

	char buttonName[0x100];

	UplinkSnprintf(buttonName, 0x100, "scrollup %s", Name);
	EclRemoveButton(buttonName);

	UplinkSnprintf(buttonName, 0x100, "scrolldown %s", Name);
	EclRemoveButton(buttonName);

	UplinkSnprintf(buttonName, 0x100, "scrollbar %s", Name);
	EclRemoveButton(buttonName);

	if (strcmp(currentGrab, buttonName) == 0)
		UnGrabScrollBar();

	InterfaceCreated = false;
}

void ScrollBox::ScrollUp()
{
	if (CurrentIndex <= 0)
		return;

	CurrentIndex--;
	UpdateInterface();
	if (Callback != nullptr)
		Callback(Name, CurrentIndex);
}

void ScrollBox::ScrollDown()
{
	if (WindowSize + CurrentIndex >= NumItems)
		return;

	CurrentIndex++;
	UpdateInterface();
	if (Callback != nullptr)
		Callback(Name, CurrentIndex);
}

bool ScrollBox::IsGrabInProgress()
{
	return strcmp(currentGrab, "None") != 0;
}

void ScrollBox::UpdateGrabScroll()
{
	// Probably unreachable
	puts("TODO: implement ScrollBox::UpdateGrabScroll()");
}

void ScrollBox::GrabScrollBar(const char* name)
{
	UplinkStrncpy(currentGrab, name, 0x100);
}

void ScrollBox::UnGrabScrollBar()
{
	strncpy(currentGrab, "None", 0x100);
}

ScrollBox* ScrollBox::GetScrollBox(const char* name)
{

	for (int i = 0; i < scrollBoxes.Size(); i++)
	{
		if (strcmp(scrollBoxes.GetData(i)->Name, name) == 0)
			scrollBoxes.GetData(i);
	}
	return nullptr;
}

void ScrollBox::CreateScrollBox(const char* name, int x, int y, int width, int height, int numItems, int windowSize, int currentIndex,
								ScrollChangeCallback callback)
{
	const auto scrollBox = new ScrollBox();
	scrollBox->SetName(name);
	scrollBox->SetDimensions(x, y, width, height);
	scrollBox->SetNumItems(numItems);
	scrollBox->SetWindowSize(windowSize);
	scrollBox->SetCurrentIndex(currentIndex);
	scrollBox->SetCallback(callback);
	scrollBoxes.PutData(scrollBox);
	scrollBox->CreateInterface();
}

void ScrollBox::RemoveScrollBox(const char* name)
{

	for (auto i = 0; i < scrollBoxes.Size(); i++)
	{
		const auto scrollBox = scrollBoxes.GetData(i);

		if (strcmp(scrollBox->Name, name) != 0)
			continue;

		scrollBox->RemoveInterface();
		delete scrollBox;
		scrollBoxes.RemoveData(i);
	}
}

void ScrollBox::ClickScrollUp(Button* button)
{
	char str[0x40];
	char name[0x80];
	sscanf(button->Name, "%s %s", str, name);
	const auto scrollbox = ScrollBox::GetScrollBox(name);
	if (scrollbox != nullptr)
		scrollbox->ScrollUp();
}

void ScrollBox::ClickScrollDown(Button* button)
{
	char str[0x40];
	char name[0x80];
	sscanf(button->Name, "%s %s", str, name);
	const auto scrollbox = ScrollBox::GetScrollBox(name);
	if (scrollbox != nullptr)
		scrollbox->ScrollDown();
}

void ScrollBox::ClickScrollBar(Button* button)
{
	(void)button;
	puts("TODO: implement ScrollBox::ClickScrollBar()");
}

void ScrollBox::MouseDownScrollBar(Button* button)
{
	button_click(button);

	char str[0x40];
	char name[0x80];

	sscanf(button->Name, "%s %s", str, name);
	GrabScrollBar(name);
}

void ScrollBox::MouseMoveScrollBar(Button* button)
{
	button_highlight(button);
}

void ScrollBox::DrawScrollBar(Button* button, bool highlighted, bool clicked)
{
	(void)highlighted;
	(void)clicked;

	glBegin(GL_QUADS);

	SetColour("PanelBackgroundA");
	glVertex2i(button->X, button->Y + button->Height);

	SetColour("PanelBackgroundB");
	glVertex2i(button->X, button->Y);

	SetColour("PanelBackgroundA");
	glVertex2i(button->X + button->Width, button->Y);

	SetColour("PanelBackgroundB");
	glVertex2i(button->X + button->Width, button->Y + button->Height);

	glEnd();

	char str[0x40];
	char name[0x80];
	sscanf(button->Name, "%s %s", str, name);

	const auto scrollbox = ScrollBox::GetScrollBox(name);
	if (scrollbox != nullptr)
	{
		const auto rbx_1 = (static_cast<float>(scrollbox->CurrentIndex) / scrollbox->NumItems * (scrollbox->Height - 30)) + (scrollbox->Y + 15);

		const int height = static_cast<float>(scrollbox->Height - 30) * scrollbox->WindowSize / scrollbox->NumItems;

		const auto height_1 = std::min(button->Height, height);

		glBegin(GL_QUADS);

		SetColour("ButtonNormalA");
		glVertex2i(scrollbox->X, height_1 + rbx_1);

		SetColour("ButtonNormalB");
		glVertex2i(scrollbox->X, rbx_1);

		SetColour("ButtonNormalA");
		glVertex2i(scrollbox->X + scrollbox->Width, rbx_1);

		SetColour("ButtonNormalB");
		glVertex2i(scrollbox->X + scrollbox->Width, height_1 + rbx_1);

		glEnd();
	}

	SetColour("PanelBorder");
	border_draw(button);
}
