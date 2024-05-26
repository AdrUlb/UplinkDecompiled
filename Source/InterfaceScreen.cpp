#include <InterfaceScreen.hpp>

#include <DArray.hpp>
#include <Eclipse.hpp>
#include <GL/gl.h>
#include <Globals.hpp>
#include <Opengl.hpp>

InterfaceScreen::InterfaceScreen()
{
	buttonNames = new DArray<char*>();
}

InterfaceScreen::~InterfaceScreen()
{
	DeleteDArrayDataD(buttonNames);
	if (buttonNames != nullptr)
	{
		delete buttonNames;
		buttonNames = nullptr;
	}
}

void InterfaceScreen::RegisterButton(int x, int y, int width, int height, const char* caption, const char* name)
{
	EclRegisterButton(x, y, width, height, caption, name);

	const auto buttonName = new char[strlen(name) + 1];
	strcpy(buttonName, name);

	buttonNames->PutData(buttonName);
}

void InterfaceScreen::RegisterButton(int x, int y, int width, int height, const char* caption, const char* tooltip, const char* name)
{
	EclRegisterButton(x, y, width, height, caption, tooltip, name);

	const auto buttonName = new char[strlen(name) + 1];
	strcpy(buttonName, name);

	buttonNames->PutData(buttonName);
}

void InterfaceScreen::Create() {}

void InterfaceScreen::Remove()
{

	for (auto i = 0; i < buttonNames->Size(); i++)
	{
		if (!buttonNames->ValidIndex(i))
			continue;

		const auto name = buttonNames->GetData(i);
		if (name != nullptr && name[0] != 0)
			EclRemoveButton(name);
	}
}

bool InterfaceScreen::IsVisible()
{
	return false;
}

int InterfaceScreen::ScreenID()
{
	return 0;
}

void LocalInterfaceScreen::Create()
{
	CreateHeight(-1);
}

void LocalInterfaceScreen::Remove()
{
	if (!IsVisible())
		return;

	EclRemoveButton("localint_background");
}

bool LocalInterfaceScreen::IsVisible()
{
	return false;
}

void LocalInterfaceScreen::BackgroundDraw(Button* button, bool highlighted, bool clicked)
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
	SetColour("PanelBorder");
	border_draw(button);
}

void LocalInterfaceScreen::CreateHeight(int value)
{
	if (IsVisible())
		return;

	const auto screenWidth = app->GetOptions()->GetOptionValue("graphics_screenwidth");

	double zmm1 = screenWidth * 0.29;
	int width = zmm1;

	if (value == -1)
		value = GetScaledYPosition(300);

	EclRegisterButton(screenWidth - width - 3, (zmm1 / 188.0 * 100.0) + 30.0, width, value, "", "", "localint_background");

	EclRegisterButtonCallbacks("localint_background", BackgroundDraw, 0, 0, nullptr);
}
