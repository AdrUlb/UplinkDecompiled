#include <ComputerScreens/LogScreen.hpp>

#include <Util.hpp>

bool LogScreen::Load(FILE* file)
{
	if (!ComputerScreen::Load(file))
		return false;

	if (!FileReadData(&nextPage, 4, 1, file))
		return false;

	if (!FileReadData(&target, 4, 1, file))
		return false;

	return true;
}

void LogScreen::Save(FILE* file)
{
	fwrite(&nextPage, 4, 1, file);
	fwrite(&target, 4, 1, file);
}

void LogScreen::Print()
{
	puts("LogScreen : ");
	ComputerScreen::Print();
	printf("\tNextPage = %d, TARGET = %d\n", nextPage, target);
}

const char* LogScreen::GetID()
{
	return "SCR_LOGS";
}

void LogScreen::SetNextPage(int value)
{
	nextPage = value;
}

void LogScreen::SetTARGET(int value)
{
	target = value;
}
