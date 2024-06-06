#include <ComputerScreens/LogScreen.hpp>

#include <Util.hpp>

bool LogScreen::Load(FILE* file)
{
	if (!ComputerScreen::Load(file))
		return false;

	if (!FileReadData(&_nextPage, 4, 1, file))
		return false;

	if (!FileReadData(&_target, 4, 1, file))
		return false;

	return true;
}

void LogScreen::Save(FILE* file)
{
	fwrite(&_nextPage, 4, 1, file);
	fwrite(&_target, 4, 1, file);
}

void LogScreen::Print()
{
	puts("LogScreen : ");
	ComputerScreen::Print();
	printf("\tNextPage = %d, TARGET = %d\n", _nextPage, _target);
}

const char* LogScreen::GetID()
{
	return "SCR_LOGS";
}

void LogScreen::SetNextPage(int nextPage)
{
	_nextPage = nextPage;
}

UplinkObjectId LogScreen::GetOBJECTID()
{
	return UplinkObjectId::LogScreen;
}

void LogScreen::SetTARGET(int type)
{
	_target = type;
}
