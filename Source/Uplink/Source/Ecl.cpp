#include <Ecl.hpp>

#include <sys/time.h>
#include "../TempGlobals.hpp"

static void EclDirtyClear()
{
	auto index = gButtons.Size() - 1;

	while (index >= 0)
	{
		if (gButtons.ValidIndex(index))
			gButtons[index]->Dirty = false;

		index--;
	}
}

void EclReset(int width, int height)
{
	(void)width;
	(void)height;

	if (gCurrentHighlight)
		delete[] gCurrentHighlight;

	if (gCurrentClick)
		delete[] gCurrentClick;

	gCurrentHighlight = nullptr;
	gCurrentClick = nullptr;

	while (gButtons.ValidIndex(0))
		EclRemoveButton(gButtons.GetData(0)->Name);

	while (const auto b = gEditableButtons.GetData(0))
	{
		if (!b)
			break;

		gEditableButtons.RemoveData(0);

		if (b)
			delete[] b;
	}
	gButtons.Empty();
	gEditableButtons.Empty();
	gSuperhighlightBorderWidth = 0;
	EclDirtyClear();
	return;
}

void EclRegisterClearDrawFunction(ClearDrawFunc func)
{
	gClearDrawFunc = func;
}

void EclRegisterDefaultButtonCallbacks(ButtonDrawFunc drawFunc, ButtonMouseUpFunc mouseUpFunc,
	ButtonMouseDownFunc mouseDownFunc, ButtonMouseMoveFunc mouseMoveFunc)
{
	gDefaultDrawFunc = drawFunc;
	gDefaultMouseUpFunc = mouseUpFunc;
	gDefaultMouseDownFunc = mouseDownFunc;
	gDefaultMouseMoveFunc = mouseMoveFunc;
}

void EclRegisterSuperHighlightFunction(int borderWidth, ButtonDrawFunc func)
{
	gSuperhighlightBorderWidth = borderWidth;
	gSuperhighlightDrawFunc = func;
}

void EclDisableAnimations()
{
	gAnimsEnabled = false;
}

void EclEnableFasterAnimations(double speed)
{
	gAnimsFasterSpeed = speed;
	gAnimsFasterEnabled = true;
}

long double EclGetAccurateTime()
{
	static bool initted = false;
	static timeval startTime;
	timeval thisTime;

	if (!initted)
	{
		initted = true;
		gettimeofday(&startTime, nullptr);
		startTime.tv_usec = 0;
		return 0.0L;
	}

	gettimeofday(&thisTime, nullptr);
	return (long double)(thisTime.tv_usec - startTime.tv_usec) / 1000.0L +
		(long double)(thisTime.tv_sec - startTime.tv_sec) * 1000.0L;
}
