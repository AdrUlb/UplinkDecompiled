#include <Interface/RemoteScreens/MenuScreenInterface.hpp>

#include <Eclipse.hpp>
#include <Gci.hpp>
#include <Globals.hpp>
#include <Opengl.hpp>
#include <Util.hpp>

void MenuScreenInterface::Remove()
{
	if (!IsVisible())
		return;

	EclRemoveButton("menuscreeninterface");
	EclRemoveButton("menuscreen_maintitle");
	EclRemoveButton("menuscreen_subtitle");

	const auto ip = GetComputerScreen()->GetComputer()->ip;

	for (auto index = 0; index < GetComputerScreen()->NumOptions(); index++)
	{
		char image[0x39];
		char text[0x39];

		snprintf(image, 0x39, "menuscreenimage %d %d %s", index, GetComputerScreen()->GetNextPage(index), ip);
		snprintf(text, 0x39, "menuscreentext %d %d %s", index, GetComputerScreen()->GetNextPage(index), ip);

		EclRemoveButton(image);
		EclRemoveButton(text);
	}
}

bool MenuScreenInterface::IsVisible()
{
	return EclGetButton("menuscreeninterface") != 0;
}

int MenuScreenInterface::ScreenID()
{
	return 3;
}

void MenuScreenInterface::Create(ComputerScreen* screen)
{
	UplinkAssert(screen != nullptr);

	computerScreen = screen;

	if (IsVisible())
		return;

	EclRegisterButton(0, 0, 0, 0, "", "", "menuscreeninterface");
	EclRegisterButtonCallbacks("menuscreeninterface", nullptr, nullptr, nullptr, nullptr);

	EclRegisterButton(80, 60, 350, 25, GetComputerScreen()->GetMainTitle(), "", "menuscreen_maintitle");
	EclRegisterButtonCallbacks("menuscreen_maintitle", DrawMainTitle, nullptr, nullptr, nullptr);

	EclRegisterButton(80, 80, 350, 20, GetComputerScreen()->GetSubTitle(), "", "menuscreen_subtitle");
	EclRegisterButtonCallbacks("menuscreen_subtitle", DrawSubTitle, nullptr, nullptr, nullptr);

	const auto ip = GetComputerScreen()->GetComputer()->ip;

	int32_t yOffset = 0;
	int32_t timeInc = 1000.0 / GetComputerScreen()->NumOptions();
	int32_t time = 0;

	for (auto i = 0; i < GetComputerScreen()->NumOptions(); i++)
	{
		char image[0x39];
		char text[0x39];
		UplinkSnprintf(text, 57, "menuscreenimage %d %d %s", i, GetComputerScreen()->GetNextPage(i), ip);
		UplinkSnprintf(image, 57, "menuscreentext %d %d %s", i, GetComputerScreen()->GetNextPage(i), ip);

		time += timeInc;

		if (game->GetInterface()->GetRemoteInterface()->securityLevel < GetComputerScreen()->GetSecurity(i))
		{
			EclRegisterButton(-350, 120 + yOffset, 16, 16, "", GetComputerScreen()->GetTooltip(i), text);
			button_assignbitmap(text, "menuscreenoption.tif");
			EclGetButton(text)->ImageNormal->SetAlpha(0.5f);
			EclRegisterButtonCallbacks(text, imagebutton_draw, nullptr, nullptr, nullptr);

			EclRegisterButton(-300, 115 + yOffset, 300, 32, GetComputerScreen()->GetCaption(i), GetComputerScreen()->GetTooltip(i), image);
			EclRegisterButtonCallbacks(image, DrawMenuOptionDimmed, nullptr, nullptr, nullptr);
			EclRegisterMovement(text, 100, 130 + yOffset, time, nullptr);
			EclRegisterMovement(image, 140, 125 + yOffset, time, nullptr);
		}
		else
		{
			EclRegisterButton(-350, yOffset + 120, 16, 16, "", GetComputerScreen()->GetTooltip(i), image);
			button_assignbitmaps(image, "menuscreenoption.tif", "menuscreenoption_h.tif", "menuscreenoption_c.tif");
			EclRegisterButtonCallback(image, ClickMenuScreenOption);

			EclRegisterButton(-300, yOffset + 115, 300, 32, GetComputerScreen()->GetCaption(i), GetComputerScreen()->GetTooltip(i), text);
			EclRegisterButtonCallbacks(text, DrawMenuOption, ClickMenuScreenOption, button_click, button_highlight);
			EclRegisterMovement(image, 100, 130 + yOffset, time, nullptr);
			EclRegisterMovement(text, 140, 125 + yOffset, time, nullptr);
		}

		yOffset += 30;
	}
}

MenuScreen* MenuScreenInterface::GetComputerScreen()
{
	UplinkAssert(computerScreen != nullptr);
	return dynamic_cast<MenuScreen*>(computerScreen);
}

void MenuScreenInterface::DrawMenuOptionDimmed(Button* button, bool highlighted, bool clicked)
{
	(void)highlighted;
	(void)clicked;
	SetColour("DimmedText");
	GciDrawText(button->X, button->Y + 20, button->Caption, 7);
}

void MenuScreenInterface::ClickMenuScreenOption(Button* button)
{
	char var_58[0x18];
	int var_20;
	int screenIndex;
	char var_38[0x18];
	sscanf(button->Name, "%s %d %d %s", var_58, &var_20, &screenIndex, var_38);

	const auto vlocation = game->GetWorld()->GetVLocation(var_38);

	Computer* computer = nullptr;
	if (vlocation != nullptr)
		computer = vlocation->GetComputer();

	if (screenIndex != -1)
		game->GetInterface()->GetRemoteInterface()->RunScreen(screenIndex, computer);
}

void MenuScreenInterface::DrawMenuOption(Button* button, bool highlighted, bool clicked)
{
	(void)highlighted;
	(void)clicked;
	SetColour("MenuText");
	GciDrawText(button->X, button->Y + 20, button->Caption, 7);
}
