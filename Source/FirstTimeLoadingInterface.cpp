#include <FirstTimeLoadingInterface.hpp>

#include <Eclipse.hpp>
#include <Globals.hpp>
#include <Opengl.hpp>
#include <Sg.hpp>

void FirstTimeLoadingInterface::Create()
{
	if (IsVisible() != 0)
		return;

	if (app->GetOptions()->IsOptionEqualTo("sound_musicenabled", 1))
		SgPlaylist_Play("main");

	EclRegisterButton(app->GetOptions()->GetOptionValue("graphics_screenwidth") - 370,
					  app->GetOptions()->GetOptionValue("graphics_screenheight") - 30, 370, 20, "", "", "firsttimeloading_text");

	EclRegisterButtonCallbacks("firsttimeloading_text", textbutton_draw, 0, 0, nullptr);
}

void FirstTimeLoadingInterface::Remove()
{
	if (!IsVisible())
		return;
	
	EclRemoveButton("firsttimeloading_text");
}

bool FirstTimeLoadingInterface::IsVisible()
{
	return EclGetButton("firsttimeloading_text") != nullptr;
}

MainMenuScreenCode FirstTimeLoadingInterface::ScreenID()
{
	return MainMenuScreenCode::FirstTimeLoading;
}

bool FirstTimeLoadingInterface::ReturnKeyPressed()
{
	UplinkAbort("TODO: implement FirstTimeLoadingInterface::ReturnKeyPressed()");
}
