#include <ComputerScreens/CypherScreen.hpp>

bool CypherScreen::Load(FILE* file)
{
	if (!ComputerScreen::Load(file))
		return false;

	if (!FileReadData(&_nextPage, 4, 1, file))
		return false;

	if (!FileReadData(&_difficulty, 4, 1, file))
		return false;

	return true;
}

void CypherScreen::Save(FILE* file)
{
	ComputerScreen::Save(file);
	fwrite(&_nextPage, 4, 1, file);
	fwrite(&_difficulty, 4, 1, file);
}

void CypherScreen::Print()
{
	puts("CypherScreen : ");
	ComputerScreen::Print();
	printf("\tNextPage:%d, Difficulty:%d\n", _nextPage, _difficulty);
}

const char* CypherScreen::GetID()
{
	return "SCR_CYPH";
}

UplinkObjectId CypherScreen::GetOBJECTID()
{
	return UplinkObjectId::CypherScreen;
}

void CypherScreen::SetNextPage(int nextPage)
{
	_nextPage = nextPage;
}

void CypherScreen::SetDifficulty(int difficulty)
{
	_difficulty = difficulty;
}
