#include <ComputerScreens/GenericScreen.hpp>

#include <Util.hpp>

bool GenericScreen::Load(FILE* file)
{
	if (!ComputerScreen::Load(file))
		return false;

	if (!FileReadData(&nextPage, 4, 1, file))
		return false;

	if (!FileReadData(&type, 4, 1, file))
		return false;

	return true;
}

void GenericScreen::Save(FILE* file)
{
	ComputerScreen::Save(file);
	fwrite(&nextPage, 4, 1, file);
	fwrite(&type, 4, 1, file);
}

void GenericScreen::Print()
{
	puts("GenericScreen :");
	ComputerScreen::Print();
	printf("TYPE=%d, nextpage=%d\n", type, nextPage);
}

const char* GenericScreen::GetID()
{
	return "SCR_GEN";
}

void GenericScreen::SetNextPage(int value)
{
	nextPage = value;
}

void GenericScreen::SetScreenType(int value)
{
	type = value;
}
