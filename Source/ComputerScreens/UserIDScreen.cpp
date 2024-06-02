#include <ComputerScreens/UserIDScreen.hpp>

#include <Util.hpp>

bool UserIDScreen::Load(FILE* file)
{
	if (!ComputerScreen::Load(file))
		return false;

	if (!FileReadData(&_nextPage, 4, 1, file))
		return false;

	if (!FileReadData(&_difficulty, 4, 1, file))
		return false;

	return true;
}

void UserIDScreen::Save(FILE* file)
{
	ComputerScreen::Save(file);
	fwrite(&_nextPage, 4, 1, file);
	fwrite(&_difficulty, 4, 1, file);
}

void UserIDScreen::Print()
{
	puts("UserIDScreen : ");
	ComputerScreen::Print();
	printf("\tNextPage:%d, Difficulty:%d\n", _nextPage, _difficulty);
}

const char* UserIDScreen::GetID()
{
	return "SCR_UID";
}

UplinkObjectId UserIDScreen::GetOBJECTID()
{
	return UplinkObjectId::UserIDScreen;
}

void UserIDScreen::SetDifficulty(int difficulty)
{
	_difficulty = difficulty;
}

void UserIDScreen::SetNextPage(int nextPage)
{
	_nextPage = nextPage;
}
