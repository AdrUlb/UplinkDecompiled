#include <Eclipse.hpp>
#include <Globals.hpp>
#include <Interface/RemoteScreens/DisconnectedScreenInterface.hpp>
#include <Opengl.hpp>

static void Click(Button* button)
{
	char var_38[0x18] = {};

	int screenIndex;
	sscanf(button->Name, "disconnectedscreen_click %d %s", &screenIndex, var_38);

	const auto vloc = game->GetWorld().GetVLocation(var_38);

	Computer* computer = nullptr;
	if (vloc != nullptr)
		computer = vloc->GetComputer();

	if (screenIndex != -1)
		game->GetInterface().GetRemoteInterface().RunScreen(screenIndex, computer);
}

void DisconnectedScreenInterface::Create()
{
	if (_screen == nullptr)
	{
		puts("DisconnectedScreenInterface::Create, tried to create when MessageScreen==NULL");
		return;
	}

	Create(_screen);
}

void DisconnectedScreenInterface::Remove()
{
	if (!IsVisible())
		return;

	EclRemoveButton("disconnectedscreen_message");
	EclRemoveButton("disconnectedscreen_loginslost");
	EclRemoveButton("disconnectedscreen_maintitle");
	EclRemoveButton("disconnectedscreen_subtitle");

	char buf[0x99];
	UplinkSnprintf(buf, 0x99, "disconnectedscreen_click %d %s", GetComputerScreen()->GetNextPage(), GetComputerScreen()->GetComputer()->GetIp());
	EclRemoveButton(buf);
}

bool DisconnectedScreenInterface::IsVisible()
{
	return EclGetButton("disconnectedscreen_message") != nullptr;
}

int DisconnectedScreenInterface::ScreenID()
{
	return 34;
}

void DisconnectedScreenInterface::Create(ComputerScreen* screen)
{
	UplinkAssert(screen != nullptr);

	_screen = screen;

	if (IsVisible())
		return;

	EclRegisterButton(80, 60, 350, 25, GetComputerScreen()->GetMainTitle(), "", "disconnectedscreen_maintitle");
	EclRegisterButtonCallbacks("disconnectedscreen_maintitle", DrawMainTitle, nullptr, nullptr, nullptr);
	EclRegisterButton(80, 80, 350, 20, GetComputerScreen()->GetSubTitle(), "", "disconnectedscreen_subtitle");
	EclRegisterButtonCallbacks("disconnectedscreen_subtitle", RemoteInterfaceScreen::DrawSubTitle, nullptr, nullptr, nullptr);
	EclRegisterButton(50, 120, 400, 30, "", "", "disconnectedscreen_message");
	EclRegisterButtonCallbacks("disconnectedscreen_message", textbutton_draw, nullptr, nullptr, nullptr);
	EclRegisterCaptionChange("disconnectedscreen_message", GetComputerScreen()->GetTextMessage(), 2000, nullptr);

	if (DisconnectedScreen::loginslost.Size() > 0)
	{
		EclRegisterButton(50, 180, 400, 200, "", "", "disconnectedscreen_loginslost");
		EclRegisterButtonCallbacks("disconnectedscreen_loginslost", textbutton_draw, nullptr, nullptr, nullptr);
		std::string str = "Your username and password was revoked on these systems:\n\n";

		for (int32_t index = 0; index < DisconnectedScreen::loginslost.Size(); index += 1)
		{
			if (DisconnectedScreen::loginslost.ValidIndex(index) != 0)
			{
				const auto ip = DisconnectedScreen::loginslost.GetData(index);

				UplinkAssert(ip != nullptr);

				const auto vloc = game->GetWorld().GetVLocation(ip);

				if (vloc != nullptr)
				{
					const auto computer = vloc->GetComputer();
					UplinkAssert(computer != nullptr);

					str.append("    - ");
					str.append(computer->GetName());
					str.append("\n");
				}
			}
		}

		EclRegisterCaptionChange("disconnectedscreen_loginslost", str.c_str(), 2000, nullptr);
		DisconnectedScreen::ClearLoginsLost();
	}

	char buf[0x99];
	UplinkSnprintf(buf, 0x99, "disconnectedscreen_click %d %s", GetComputerScreen()->GetNextPage(), GetComputerScreen()->GetComputer()->GetIp());
	EclRegisterButton(270, 380, 100, 20, "OK", "Click to close this screen", buf);
	EclRegisterButtonCallback(buf, Click);
}

bool DisconnectedScreenInterface::ReturnKeyPressed()
{
	const auto nextPage = GetComputerScreen()->GetNextPage();

	if (nextPage != -1)
	{
		game->GetInterface().GetRemoteInterface().RunScreen(nextPage, GetComputerScreen()->GetComputer());
	}

	return true;
}

DisconnectedScreen* DisconnectedScreenInterface::GetComputerScreen() const
{
	UplinkAssert(_screen != nullptr);
	return dynamic_cast<DisconnectedScreen*>(_screen);
}
