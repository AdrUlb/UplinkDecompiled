#include <ComputerScreens/GenericScreen.hpp>

#include <Util.hpp>

bool GenericScreen::Load(FILE* file)
{
	if (!ComputerScreen::Load(file))
		return false;

	if (!FileReadData(&_nextPage, 4, 1, file))
		return false;

	if (!FileReadData(&_type, 4, 1, file))
		return false;

	return true;
}

void GenericScreen::Save(FILE* file)
{
	ComputerScreen::Save(file);
	fwrite(&_nextPage, 4, 1, file);
	fwrite(&_type, 4, 1, file);
}

void GenericScreen::Print()
{
	puts("GenericScreen :");
	ComputerScreen::Print();
	printf("TYPE=%d, nextpage=%d\n", _type, _nextPage);
}

const char* GenericScreen::GetID()
{
	return "SCR_GEN";
}

UplinkObjectId GenericScreen::GetOBJECTID()
{
	return UplinkObjectId::GenericScreen;
}

int GenericScreen::GetNextPage()
{
	return _nextPage;
}

int GenericScreen::GetType()
{
	return _type;
}

void GenericScreen::SetNextPage(int nextPage)
{
	_nextPage = nextPage;
}

void GenericScreen::SetScreenType(int type)
{
	_type = type;
}
