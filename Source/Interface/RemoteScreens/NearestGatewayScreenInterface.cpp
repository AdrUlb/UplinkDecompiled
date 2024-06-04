#include <Interface/RemoteScreens/NearestGatewayScreenInterface.hpp>

#include <Eclipse.hpp>
#include <GL/gl.h>
#include <Gci.hpp>
#include <Globals.hpp>
#include <Opengl.hpp>

static PhysicalGatewayLocation PHYSICALGATEWAYLOCATIONS[] = {
	{"London", "United Kingdom", 282, 69}, {"Tokyo", "Japan", 514, 104},  {"Los Angeles", "USA", 91, 125},	{"New York", "USA", 171, 98},
	{"Chicago", "USA", 138, 88},		   {"Moscow", "Russia", 331, 70}, {"Hong Kong", "China", 485, 133}, {"Sydney", "Australia", 547, 244}};

static PhysicalGatewayLocation PHYSICALGATEWAYLOCATIONS_DEFCON[] = {
	{"London", "United Kingdom", 295, 75}, {"Tokyo", "Japan", 527, 103},	 {"Los Angeles", "USA", 99, 106},
	{"New York", "USA", 175, 94},		   {"Chicago", "USA", 151, 91},		 {"Moscow", "Russia", 356, 65},
	{"Hong Kong", "China", 483, 126},	   {"Sydney", "Australia", 545, 226}

};

void NearestGatewayScreenInterface::Remove()
{
	if (!IsVisible())
		return;

	EclRemoveButton("nearestgateway_maintitle");
	EclRemoveButton("nearestgateway_black");
	EclRemoveButton("nearestgateway_largemap");

	for (auto i = 0; i < 8; i++)
	{
		char buttonName[0x40];
		UplinkSnprintf(buttonName, 0x40, "nearestgateway_location %d", i);
		EclRemoveButton(buttonName);
	}

	EclRemoveButton("nearestgateway_helptext");
}

bool NearestGatewayScreenInterface::IsVisible()
{
	return EclGetButton("nearestgateway_largemap") != 0;
}

int NearestGatewayScreenInterface::ScreenID()
{
	return 31;
}

void NearestGatewayScreenInterface::Create(ComputerScreen* screen)
{
	_screen = screen;

	if (IsVisible())
		return;

	EclRegisterButton(40, 10, 350, 25, GetComputerScreen()->GetSubTitle(), "", "nearestgateway_maintitle");
	EclRegisterButtonCallbacks("nearestgateway_maintitle", DrawMainTitle, nullptr, nullptr, nullptr);

	EclRegisterButton(470, 280, 140, 140, " ", " ", "nearestgateway_black");
	EclRegisterButtonCallbacks("nearestgateway_black", DrawBlack, nullptr, nullptr, nullptr);

	const auto screenWidth = app->GetOptions()->GetOptionValue("graphics_screenwidth");

	const int height = (screenWidth - 46) / 595.0 * 316.0;

	EclRegisterButton(23, 50, (screenWidth - 46), height, "", "", "nearestgateway_largemap");
	if (game->GetWorldMapType() == 1)
	{
		button_assignbitmap("nearestgateway_largemap", "worldmaplarge_defcon.tif");
	}
	else
	{
		button_assignbitmap("nearestgateway_largemap", "worldmaplarge.tif");
	}
	EclRegisterButtonCallbacks("nearestgateway_largemap", DrawMainMap, 0, 0, nullptr);

	const auto locations = game->GetWorldMapType() == 1 ? PHYSICALGATEWAYLOCATIONS_DEFCON : PHYSICALGATEWAYLOCATIONS;

	for (auto i = 0; i < 8; i++)
	{
		const auto& location = locations[i];

		char name[0x40];
		UplinkSnprintf(name, 0x40, "nearestgateway_location %d", i);

		char tooltip[0x80];
		UplinkSnprintf(tooltip, 0x80, "Select %s as your local Gateway", location.City);

		EclRegisterButton(23 + (location.X / 594.0f) * (screenWidth - 46), 50 + (location.Y / 315.0f * height), 7, 7, location.City, tooltip, name);
		EclRegisterButtonCallbacks(name, DrawLocation, ClickLocation, button_click, button_highlight);
	}

	EclRegisterButton(50, height + 70, 400, 70, " ", " ", "nearestgateway_helptext");
	EclRegisterButtonCallbacks("nearestgateway_helptext", textbutton_draw, nullptr, nullptr, nullptr);
	EclRegisterCaptionChange("nearestgateway_helptext",
							 "Uplink Corporation has offices all around the world.\n"
							 "Please select a nearby location for your Gateway.\n"
							 "You will dial into this location when you use Uplink.",
							 2000, nullptr);
}

GenericScreen* NearestGatewayScreenInterface::GetComputerScreen()
{
	UplinkAssert(_screen != nullptr);
	return dynamic_cast<GenericScreen*>(_screen);
}

void NearestGatewayScreenInterface::DrawBlack(Button* button, bool highlighted, bool clicked)
{
	(void)highlighted;
	(void)clicked;
	UplinkAssert(button != nullptr);
	clear_draw(button->X, button->Y, button->Width, button->Height);
}

void NearestGatewayScreenInterface::DrawMainMap(Button* button, bool highlighted, bool clicked)
{
	(void)highlighted;
	(void)clicked;
	UplinkAssert(button != nullptr);
	imagebutton_drawtextured(button, highlighted, clicked);
	glColor3ub(81, 138, 215);
	border_draw(button);
}

void NearestGatewayScreenInterface::DrawLocation(Button* button, bool highlighted, bool clicked)
{
	(void)highlighted;
	(void)clicked;
	UplinkAssert(button != nullptr);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glVertex2i(button->X, button->Y);
	glVertex2i(button->X + 7, button->Y);
	glVertex2i(button->X + 7, button->Y + 7);
	glVertex2i(button->X, button->Y + 7);
	glEnd();

	char str[0x40];
	int index;
	sscanf(button->Name, "%s %d", str, &index);

	UplinkAssert(index < 8);

	const auto locations = game->GetWorldMapType() == 1 ? PHYSICALGATEWAYLOCATIONS_DEFCON : PHYSICALGATEWAYLOCATIONS;
	const auto& location = locations[index];
	GciDrawText(button->X - 10, button->Y + 20, location.City);
}

void NearestGatewayScreenInterface::ClickLocation(Button* button)
{
	UplinkAssert(button != nullptr);

	char str[0x40];
	int index;
	sscanf(button->Name, "%s %d", str, &index);

	UplinkAssert(index < 8);

	const auto locations = game->GetWorldMapType() == 1 ? PHYSICALGATEWAYLOCATIONS_DEFCON : PHYSICALGATEWAYLOCATIONS;
	const auto& location = locations[index];

	game->GetWorld().GetPlayer().GetLocalHost()->SetPLocation(location.X, location.Y);

	const auto remoteInterfaceScreen = dynamic_cast<NearestGatewayScreenInterface*>(game->GetInterface().GetRemoteInterface().GetInterfaceScreen());

	UplinkAssert(remoteInterfaceScreen != nullptr);

	const auto computerScreen = remoteInterfaceScreen->GetComputerScreen();

	UplinkAssert(computerScreen != nullptr);

	const auto computer = computerScreen->GetComputer();

	game->GetInterface().GetRemoteInterface().RunScreen(computerScreen->GetNextPage(), computer);
}
