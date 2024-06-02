#include <Interface/RemoteScreens/DialogScreenInterface.hpp>

#include <Eclipse.hpp>
#include <GL/gl.h>
#include <Gci.hpp>
#include <Globals.hpp>
#include <Opengl.hpp>
#include <ScriptLibrary.hpp>
#include <Util.hpp>
#include <cstdio>

void DialogScreenInterface::Remove()
{
	if (!IsVisible())
		return;

	struct DialogScreen* screen = GetComputerScreen();
	const auto computer = screen->GetComputer();

	UplinkAssert(screen != nullptr);

	EclRemoveButton("dialogscreen");
	EclRemoveButton("dialogscreen_maintitle");
	EclRemoveButton("dialogscreen_subtitle");

	for (auto i = 0; i < screen->GetWidgets()->Size(); i++)
		RemoveWidget(screen->GetWidgets()->GetData(i), computer);
}

bool DialogScreenInterface::IsVisible()
{
	return EclGetButton("dialogscreen") != 0;
}

int DialogScreenInterface::ScreenID()
{
	return 5;
}

void DialogScreenInterface::Create(ComputerScreen* screen)
{
	UplinkAssert(screen != nullptr);

	if (IsVisible())
		return;

	computerScreen = screen;

	EclRegisterButton(0, 0, 0, 0, " ", " ", "dialogscreen");
	EclRegisterButtonCallbacks("dialogscreen", nullptr, nullptr, nullptr, nullptr);
	EclRegisterButton(80, 60, 350, 25, GetComputerScreen()->GetMainTitle(), "", "dialogscreen_maintitle");
	EclRegisterButtonCallbacks("dialogscreen_maintitle", RemoteInterfaceScreen::DrawMainTitle, nullptr, nullptr, nullptr);
	EclRegisterButton(80, 80, 350, 20, GetComputerScreen()->GetSubTitle(), "", "dialogscreen_subtitle");
	EclRegisterButtonCallbacks("dialogscreen_subtitle", RemoteInterfaceScreen::DrawSubTitle, nullptr, nullptr, nullptr);
	const auto dialogScreen = GetComputerScreen();
	const auto computer = dialogScreen->GetComputer();

	for (auto i = 0; i < dialogScreen->GetWidgets()->Size(); i++)
	{
		const auto widget = dialogScreen->GetWidgets()->GetData(i);
		UplinkAssert(widget != nullptr);

		char buttonName[0x40];
		char buttonNameWithIp[0x59];
		UplinkSnprintf(buttonName, 0x40, "%s %d %d", widget->GetName(), widget->GetData1(), widget->GetData2());
		UplinkSnprintf(buttonNameWithIp, 0x59, "%s %d %d %s", widget->GetName(), widget->GetData1(), widget->GetData2(), computer->GetIp());

		switch (widget->GetType())
		{
			case 0:
				puts("TODO: implement DialogScreenInterface::Create()	for widget type 0");
				break;
			case 1:
				EclRegisterButton(widget->GetX(), widget->GetY(), widget->GetWidth(), widget->GetHeight(), widget->GetCaption(), widget->GetTooltip(),
								  buttonName);
				EclRegisterButtonCallbacks(buttonName, button_draw, nullptr, nullptr, nullptr);
				break;
			case 2:
				EclRegisterButton(widget->GetX(), widget->GetY(), widget->GetWidth(), widget->GetHeight(), widget->GetCaption(), widget->GetTooltip(),
								  buttonName);
				EclRegisterButtonCallbacks(buttonName, textbutton_draw, nullptr, nullptr, nullptr);
				break;
			case 3:
				EclRegisterButton(widget->GetX(), widget->GetY(), widget->GetWidth(), widget->GetHeight(), widget->GetCaption(), widget->GetTooltip(),
								  buttonName);
				EclRegisterButtonCallbacks(buttonName, textbutton_draw, 0, button_click, button_highlight);
				EclMakeButtonEditable(buttonName);
				break;
			case 4:
				EclRegisterButton(widget->GetX(), widget->GetY(), widget->GetWidth(), widget->GetHeight(), widget->GetCaption(), widget->GetTooltip(),
								  buttonName);
				EclRegisterButtonCallbacks(buttonName, PasswordBoxDraw, 0, button_click, button_highlight);
				EclMakeButtonEditable(buttonName);
				break;
			case 5:
				EclRegisterButton(widget->GetX(), widget->GetY(), widget->GetWidth(), widget->GetHeight(), widget->GetCaption(), widget->GetTooltip(),
								  buttonNameWithIp);
				EclRegisterButtonCallbacks(buttonNameWithIp, button_draw, NextPageClick, button_click, button_highlight);
				break;
			case 6:
				puts("TODO: implement DialogScreenInterface::Create()	for widget type 6");
				break;
			case 7:
				puts("TODO: implement DialogScreenInterface::Create()	for widget type 7");
				break;
			case 8:
				EclRegisterButton(widget->GetX(), widget->GetY(), widget->GetWidth(), widget->GetHeight(), widget->GetCaption(), widget->GetTooltip(),
								  buttonNameWithIp);
				EclRegisterButtonCallbacks(buttonNameWithIp, button_draw, ScriptButtonClick, button_click, button_highlight);
				break;
			case 9:
				puts("TODO: implement DialogScreenInterface::Create()	for widget type 9");
				break;
		}
	}
}

bool DialogScreenInterface::ReturnKeyPressed()
{
	const auto screen = GetComputerScreen();
	UplinkAssert(screen != nullptr);
	char* returnKeyButton = screen->_returnKeyButtonName;

	if (returnKeyButton == nullptr)
		return false;

	for (auto i = 0; i < screen->GetWidgets()->Size(); i++)
	{
		const auto widget = screen->GetWidgets()->GetData(i);

		UplinkAssert(widget != nullptr);

		if (strcmp(widget->GetName(), returnKeyButton) != 0)
			continue;

		char s[0x59];
		UplinkSnprintf(s, 0x59, "%s %d %d", widget->GetName(), widget->GetData1(), widget->GetData2());
		auto button = EclGetButton(s);
		if (button == nullptr)
		{
			UplinkSnprintf(s, 0x59, "%s %d %d %s", widget->GetName(), widget->GetData1(), widget->GetData2(), screen->GetComputer()->GetIp());
			button = EclGetButton(s);
		}
		button->MouseUp();

		return true;
	}

	return false;
}

bool DialogScreenInterface::EscapeKeyPressed()
{
	puts("TODO: implement DialogScreenInterface::EscapeKeyPressed()");
	return false;
}

DialogScreen* DialogScreenInterface::GetComputerScreen()
{
	UplinkAssert(computerScreen != nullptr);
	return dynamic_cast<DialogScreen*>(computerScreen);
}

void DialogScreenInterface::RemoveWidget(DialogScreenWidget* widget, Computer* computer)
{
	UplinkAssert(widget != nullptr);
	UplinkAssert(computer != nullptr);

	char buttonName[0x40];
	UplinkSnprintf(buttonName, 0x40, "%s %d %d", widget->GetName(), widget->GetData1(), widget->GetData2());
	EclRemoveButton(buttonName);

	char buttonNameWithIp[0x59];
	UplinkSnprintf(buttonNameWithIp, 0x59, "%s %d %d %s", widget->GetName(), widget->GetData1(), widget->GetData2(), computer->GetIp());
	EclRemoveButton(buttonNameWithIp);
}

void DialogScreenInterface::NextPageClick(Button* button)
{
	UplinkAssert(button != nullptr);

	char widgetName[0x60];
	int data1;
	int data2;
	char ip[0x18];
	sscanf(button->Name, "%s %d %d %s", widgetName, &data1, &data2, ip);

	const auto screenIndex = data1;
	struct VLocation* vlocation = game->GetWorld()->GetVLocation(ip);

	struct Computer* computer = nullptr;
	if (vlocation != 0)
		computer = vlocation->GetComputer();

	if (screenIndex != -1)
		game->GetInterface()->GetRemoteInterface()->RunScreen(screenIndex, computer);
}

void DialogScreenInterface::PasswordBoxDraw(Button* button, bool highlighted, bool clicked)
{
	UplinkAssert(button != nullptr);

	int32_t rax_2 = app->GetOptions()->GetOptionValue("graphics_screenheight");

	glScissor(button->X, rax_2 - button->Y - button->Height, button->Width, button->Height);
	glEnable(GL_SCISSOR_TEST);

	clear_draw(button->X, button->Y, button->Width, button->Height);
	glColor4f(1.0f, 1.0f, 1.0f, 0.85f);

	const auto textLen = strlen(button->Caption);
	char* text = new char[textLen + 1];
	for (size_t i = 0; i < textLen; i++)
		text[i] = '*';

	// FIXME: sometimes text doesn't render correctly when scissor test is enabled, even when it is contained within the scissor box
	glDisable(GL_SCISSOR_TEST);
	text[textLen] = 0;
	GciDrawText(button->X + 10, button->Y + 10, text, 2);
	delete[] text;

	if (highlighted || clicked)
		border_draw(button);

	glDisable(GL_SCISSOR_TEST);
}

void DialogScreenInterface::ScriptButtonClick(Button* button)
{
	UplinkAssert(button != nullptr);
	char widgetName[0x40];
	int data1;
	int data2;
	char ip[0x18];
	sscanf(button->Name, "%s %d %d %s", widgetName, &data1, &data2, ip);
	const auto script = data1;
	const auto screenIndex = data2;

	if (script != -1)
		ScriptLibrary::RunScript(script);

	const auto vlocation = game->GetWorld()->GetVLocation(ip);

	Computer* computer = nullptr;
	if (vlocation != nullptr)
		computer = vlocation->GetComputer();

	if (screenIndex != -1)
		game->GetInterface()->GetRemoteInterface()->RunScreen(screenIndex, computer);
}