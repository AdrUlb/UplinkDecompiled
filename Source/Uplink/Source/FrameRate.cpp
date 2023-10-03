#include <FrameRate.hpp>
#include <Ecl.hpp>

FrameRate::FrameRate() : count(30), fps(0.0f) {}

long double FrameRate::FPS()
{
	return fps;
}

void FrameRate::Start()
{
	lastTime = EclGetAccurateTime();
	count = 30;
	fps = 0.0f;
}

void FrameRate::Tick()
{
	if (count > 0)
	{
		count--;
		return;
	}

	count = 30;
	fps = 30000.0f / (float)((int)EclGetAccurateTime() - lastTime);
	lastTime = EclGetAccurateTime();
}
