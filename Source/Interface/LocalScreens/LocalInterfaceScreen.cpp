#include <Interface/LocalScreens/LocalInterfaceScreen.hpp>

#include <Eclipse.hpp>
#include <GL/gl.h>
#include <Globals.hpp>
#include <Opengl.hpp>

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

int LocalInterfaceScreen::ScreenID()
{
	return 0;
}
