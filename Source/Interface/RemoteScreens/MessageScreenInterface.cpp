#include <Interface/RemoteScreens/MessageScreenInterface.hpp>

#include <Computer.hpp>
#include <Eclipse.hpp>
#include <Globals.hpp>
#include <Opengl.hpp>
#include <Util.hpp>

void MessageScreenInterface::Create()
{
	if (computerScreen == nullptr)
	{
		puts("MessageScreenInterface::Create, tried to create when MessageScreen==NULL");
		return;
	}

	Create(computerScreen);
}

void MessageScreenInterface::Remove()
{
	if (!IsVisible())
		return;

	EclRemoveButton("messagescreen_message");
	EclRemoveButton("messagescreen_maintitle");
	EclRemoveButton("messagescreen_subtitle");
	const auto computer = GetComputerScreen()->GetComputer();
	char buf[0x99];
	UplinkSnprintf(buf, 0x99, "messagescreen_click %d %s", GetComputerScreen()->nextPage, computer->ip);

	EclRemoveButton(buf);
	EclRemoveButton("messagescreen_mailme");
}

bool MessageScreenInterface::IsVisible()
{
	return EclGetButton("messagescreen_message") != nullptr;
}

int MessageScreenInterface::ScreenID()
{
	return 1;
}

void MessageScreenInterface::Create(ComputerScreen* screen)
{
	UplinkAssert(screen != nullptr);

	computerScreen = screen;

	if (IsVisible())
		return;

	EclRegisterButton(80, 60, 350, 25, GetComputerScreen()->mainTitle, "", "messagescreen_maintitle");
	EclRegisterButtonCallbacks("messagescreen_maintitle", DrawMainTitle, 0, 0, nullptr);
	EclRegisterButton(80, 80, 350, 20, GetComputerScreen()->subTitle, "", "messagescreen_subtitle");
	EclRegisterButtonCallbacks("messagescreen_subtitle", DrawSubTitle, 0, 0, nullptr);
	EclRegisterButton(50, 120, 400, 245, "", "", "messagescreen_message");
	EclRegisterButtonCallbacks("messagescreen_message", textbutton_draw, 0, 0, nullptr);
	EclRegisterCaptionChange("messagescreen_message", GetComputerScreen()->textMessage, 2000, nullptr);

	char s[0x99];
	if (GetComputerScreen()->buttonMessage != nullptr)
	{
		const auto buttonMessageLength = strlen(GetComputerScreen()->buttonMessage);
		const auto widthStuff = buttonMessageLength + (buttonMessageLength * 4);
		UplinkSnprintf(s, 0x99, "messagescreen_click %d %s", GetComputerScreen()->nextPage, GetComputerScreen()->GetComputer()->ip);
		char* buttonMessage = GetComputerScreen()->buttonMessage;
		EclRegisterButton(320 - (((widthStuff * 2) + 0x14) / 2), 370, ((widthStuff * 2) + 20), 20, GetComputerScreen()->buttonMessage, buttonMessage,
						  s);
		EclRegisterButtonCallback(s, Click);
	}

	if (GetComputerScreen()->mailThisToMe)
	{
		const auto computer = GetComputerScreen()->GetComputer();
		UplinkSnprintf(s, 0x99, "messagescreen_click %d %s", GetComputerScreen()->nextPage, computer->ip);
		const auto rax_22 = EclGetButton(s);
		UplinkAssert(rax_22 != nullptr);
		EclRegisterButton(rax_22->X - 120, rax_22->Y, 100, 20, "Mail this to me", "Click to send this information to yourself in an email",
						  "messagescreen_mailme");
		EclRegisterButtonCallback("messagescreen_mailme", MailMeClick);
	}
}

bool MessageScreenInterface::ReturnKeyPressed()
{
	if (GetComputerScreen()->nextPage != -1)
	{
		struct Computer* computer = GetComputerScreen()->GetComputer();
		game->GetInterface()->GetRemoteInterface()->RunScreen(GetComputerScreen()->nextPage, computer);
	}
	return true;
}

MessageScreen* MessageScreenInterface::GetComputerScreen()
{
	UplinkAssert(computerScreen != nullptr);
	return dynamic_cast<MessageScreen*>(computerScreen);
}

void MessageScreenInterface::Click(Button* button)
{
	int screenIndex;
	char ip[0x18] = {0};
	sscanf(button->Name, "messagescreen_click %d %s", &screenIndex, ip);

	const auto vlocation = game->GetWorld()->GetVLocation(ip);

	Computer* computer = nullptr;
	if (vlocation != nullptr)
		computer = vlocation->GetComputer();

	if (screenIndex != -1)
		game->GetInterface()->GetRemoteInterface()->RunScreen(screenIndex, computer);
}

void MessageScreenInterface::MailMeClick(Button* button)
{
	(void)button;
	puts("TODO: implement MessageScreenInterface::MailMeClick()");
}
