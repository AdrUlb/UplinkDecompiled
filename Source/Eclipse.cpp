#include <Eclipse.hpp>

#include <Button.hpp>
#include <LList.hpp>
#include <sys/time.h>

static const char* currenthighlight;
static const char* currentclick;

static LList<Button*> buttons;
static LList<const char*> editablebuttons;

static uint32_t superhighlight_borderwidth = 0;

static void* dirtyrectangles = nullptr;
static void* data_7b3788 = nullptr;
static void* data_7b3790 = nullptr;

static void EclDirtyClear()
{
	data_7b3788 = dirtyrectangles;

	for (auto i = buttons.Size() - 1; i >= 0; i--)
		if (buttons.ValidIndex(i) != 0)
			buttons[i]->SetDirty(false);
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
		buttons.GetData(0)->GetName();

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
