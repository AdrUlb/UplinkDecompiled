#include <ComputerScreens/UserIDScreen.hpp>

#include <Util.hpp>

bool UserIDScreen::Load(FILE* file)
{
	if (!ComputerScreen::Load(file))
		return false;

	if (!FileReadData(&nextPage, 4, 1, file))
		return false;

	if (!FileReadData(&difficulty, 4, 1, file))
		return false;

	return true;
}

void UserIDScreen::Save(FILE* file)
{
	ComputerScreen::Save(file);
	fwrite(&nextPage, 4, 1, file);
	fwrite(&difficulty, 4, 1, file);
}

void UserIDScreen::Print()
{
	puts("UserIDScreen : ");
	ComputerScreen::Print();
	printf("\tNextPage:%d, Difficulty:%d\n", nextPage, difficulty);
}

const char* UserIDScreen::GetID()
{
	return "SCR_UID";
}

UplinkObjectId UserIDScreen::GetOBJECTID()
{
	return UplinkObjectId::UserIDScreen;
}

void UserIDScreen::SetDifficulty(int value)
{
	difficulty = value;
}

void UserIDScreen::SetNextPage(int value)
{
	nextPage = value;
}