#include <Interface/RemoteScreens/RemoteInterfaceScreen.hpp>

#include <GL/gl.h>
#include <Gci.hpp>
#include <Globals.hpp>
#include <Opengl.hpp>

void RemoteInterfaceScreen::Create()
{
	if (_screen == nullptr)
	{
		printf("Error : RemoteInterfaceScreen::Create, cs==NULL, ScreenID=%d\n", ScreenID());
		return;
	}

	Create(_screen);
}

void RemoteInterfaceScreen::Remove() {}

void RemoteInterfaceScreen::Create(ComputerScreen* screen)
{
	_screen = screen;
}

bool RemoteInterfaceScreen::ReturnKeyPressed()
{
	return false;
}

bool RemoteInterfaceScreen::EscapeKeyPressed()
{
	return false;
}

void RemoteInterfaceScreen::DrawMainTitle(Button* button, bool highlighted, bool clicked)
{
	(void)highlighted;
	(void)clicked;
	const auto screenHeight = app->GetOptions().GetOptionValue("graphics_screenheight");
	glScissor(button->X, screenHeight - button->Y - button->Height, button->Width, button->Height);
	glEnable(GL_SCISSOR_TEST);
	SetColour("DefaultText");

	// FIXME: sometimes text doesn't render correctly when scissor test is enabled, even when it is contained within the scissor box
	glDisable(GL_SCISSOR_TEST);

	GciDrawText(button->X, button->Y + (button->Height / 2) + 5, button->Caption, 7);
	glDisable(GL_SCISSOR_TEST);
}

void RemoteInterfaceScreen::DrawSubTitle(Button* button, bool highlighted, bool clicked)
{
	(void)highlighted;
	(void)clicked;
	const auto screenHeight = app->GetOptions().GetOptionValue("graphics_screenheight");
	glScissor(button->X, screenHeight - button->Y - button->Height, button->Width, button->Height);
	glEnable(GL_SCISSOR_TEST);
	SetColour("DefaultText");

	// FIXME: sometimes text doesn't render correctly when scissor test is enabled, even when it is contained within the scissor box
	glDisable(GL_SCISSOR_TEST);

	GciDrawText(button->X, button->Y + (button->Height / 2) + 5, button->Caption, 6);
	glDisable(GL_SCISSOR_TEST);
}

ComputerScreen* RemoteInterfaceScreen::GetComputerScreen()
{
	return _screen;
}

RemoteInterfaceScreen* RemoteInterfaceScreen::GetInterfaceScreen(int id)
{
	const auto ret = game->GetInterface().GetRemoteInterface().GetInterfaceScreen();
	UplinkAssert(ret != nullptr);
	UplinkAssert(ret->ScreenID() == id);
	return ret;
}
