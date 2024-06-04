#include <Interface/RemoteScreens/CodeCardScreenInterface.hpp>

#include <Eclipse.hpp>
#include <Globals.hpp>

void CodeCardScreenInterface::Remove()
{
	if (!IsVisible())
		return;

	EclRemoveButton("codecard_maintitle");
	EclRemoveButton("codecard_subtitle");
	EclRemoveButton("codecard_text");
	EclRemoveButton("codecard_rowcol");
	EclRemoveButton("codecard_answer");
	EclRemoveButton("codecard_proceed");
}

bool CodeCardScreenInterface::IsVisible()
{
	return EclGetButton("codecard_maintitle") != nullptr;
}

int CodeCardScreenInterface::ScreenID()
{
	return 33;
}

void CodeCardScreenInterface::Create(ComputerScreen* screen)
{
	UplinkAssert(screen != nullptr);

	_screen = screen;

	if (app->GetCheckCodeCard() || GetComputerScreen()->GetNextPage() == -1)
		UplinkAbort("Code card not implemented.");

	game->GetInterface().GetRemoteInterface().RunScreen(GetComputerScreen()->GetNextPage(), screen->GetComputer());
}

bool CodeCardScreenInterface::ReturnKeyPressed()
{
	ProceedClick(nullptr);
	return true;
}

GenericScreen* CodeCardScreenInterface::GetComputerScreen()
{
	UplinkAssert(_screen != nullptr);
	return dynamic_cast<GenericScreen*>(_screen);
}

void CodeCardScreenInterface::ProceedClick(Button* button)
{
	(void)button;
	puts("TODO: implement CodeCardScreenInterface::ProceedClick()");
}