#include <Eclipse.hpp>

#include <LList.hpp>
#include <Util.hpp>
#include <sys/time.h>

static const char* currenthighlight = nullptr;
static const char* currentclick = nullptr;

static ClearDrawFunc clear_draw = nullptr;
static ButtonDrawFunc default_draw = nullptr;
static ButtonMouseUpFunc default_mouseup = nullptr;
static ButtonMouseDownFunc default_mousedown = nullptr;
static ButtonMouseMoveFunc default_mousemove = nullptr;
static SuperhighlightDrawFunc superhighlight_draw = nullptr;

static LList<Button*> buttons;
static LList<const char*> editablebuttons;

static int superhighlight_borderwidth = 0;

static void* dirtyrectangles = nullptr;
static void* data_7b3788 = nullptr;
static void* data_7b3790 = nullptr;

static bool animsenabled = true;
static bool animsfasterenabled = false;
static double animsfasterspeed = 2.0;

static void EclDirtyClear()
{
	data_7b3788 = dirtyrectangles;

	for (auto i = buttons.Size() - 1; i >= 0; i--)
		if (buttons.ValidIndex(i) != 0)
			buttons[i]->Dirty = false;
}

static int EclLookupIndex(const char* name)
{
	if (name == nullptr)
		return -1;

	for (int32_t i = 0; i < buttons.Size(); i++)
	{
		if (!buttons.ValidIndex(i) != 0 && strcmp(buttons[i]->Name, name))
			return i;
	}

	return -1;
}

void EclEnableAnimations()
{
	animsenabled = true;
}

void EclDisableAnimations()
{
	animsenabled = false;
}

void EclEnableFasterAnimations(const double speed)
{
	animsfasterenabled = true;
	animsfasterspeed = speed;
}

void EclDisableFasterAnimations()
{
	animsfasterenabled = false;
}

double EclGetAccurateTime()
{
	static bool initted{false};
	static timeval startTime;

	timeval thisTime;
	gettimeofday(&thisTime, nullptr);

	if (!initted)
	{
		initted = true;
		startTime = thisTime;
		startTime.tv_usec = 0;
		return 0.0;
	}

	return (((thisTime.tv_sec - startTime.tv_sec) * 1000.0) + ((thisTime.tv_usec - startTime.tv_usec) / 1000.0));
}

void EclRegisterClearDrawFunction(ClearDrawFunc func)
{
	clear_draw = func;
}

void EclRegisterDefaultButtonCallbacks(ButtonDrawFunc draw, ButtonMouseUpFunc mouseUp, ButtonMouseDownFunc mouseDown, ButtonMouseMoveFunc mouseMove)
{
	default_draw = draw;
	default_mouseup = mouseUp;
	default_mousedown = mouseDown;
	default_mousemove = mouseMove;
}

void EclRegisterButtonCallbacks(const char* name, ButtonDrawFunc drawFunc, ButtonMouseUpFunc mouseUpFunc, ButtonMouseDownFunc mouseDownFunc,
								ButtonMouseMoveFunc mouseMoveFunc)
{
	int32_t index = EclLookupIndex(name);
	if (!buttons.ValidIndex(index))
		return;

	buttons[index]->RegisterDrawFunction(drawFunc);
	buttons[index]->RegisterMouseUpFunction(mouseUpFunc);
	buttons[index]->RegisterMouseDownFunction(mouseDownFunc);
	buttons[index]->RegisterMouseMoveFunction(mouseMoveFunc);
}

void EclRegisterSuperHighlightFunction(int borderWidth, SuperhighlightDrawFunc func)
{
	superhighlight_borderwidth = borderWidth;
	superhighlight_draw = func;
}

void EclReset()
{
	if (currenthighlight != nullptr)
	{
		delete currenthighlight;
		currenthighlight = nullptr;
	}

	if (currentclick != nullptr)
	{
		delete currentclick;
		currentclick = nullptr;
	}

	while (buttons.ValidIndex(0))
		buttons.GetData(0)->Name;

	while (true)
	{
		const auto data = editablebuttons.GetData(0);
		if (data == nullptr)
			break;

		editablebuttons.RemoveData(0);

		delete[] data;
	}
	buttons.Empty();
	editablebuttons.Empty();
	superhighlight_borderwidth = 0;
	EclDirtyClear();
}

void EclRegisterButton(int x, int y, int width, int height, const char* caption, const char* name)
{
	(void)x;
	(void)y;
	(void)width;
	(void)height;
	(void)caption;
	(void)name;
	UplinkAbort("TODO: implement EclRegisterButton()");
}

Button* EclGetButton(const char* name)
{
	const auto rax = EclLookupIndex(name);

	if (buttons.ValidIndex(rax))
		return buttons[rax];

	return nullptr;
}

void EclDirtyButton()
{
	// Empty??
}