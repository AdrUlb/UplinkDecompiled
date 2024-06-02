#include <View.hpp>

#include <Eclipse.hpp>

void FrameRate::Start()
{
	_ticks = 30;
	_fps = 0.0f;
	_lastTime = EclGetAccurateTime();
}

bool View::Load(FILE* file)
{
	(void)file;
	Initialise();
	return true;
}

void View::Save(FILE* file)
{
	(void)file;
}

void View::Print()
{
	puts("============== V I E W =====================================");
	puts("============== E N D  O F  V I E W =========================");
}

const char* View::GetID()
{
	return "VIEW";
}

void View::Initialise()
{
	_frameRate.Start();
}