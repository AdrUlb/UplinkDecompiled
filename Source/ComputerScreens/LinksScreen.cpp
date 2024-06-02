#include <ComputerScreens/LinksScreen.hpp>

#include <Util.hpp>

bool LinksScreen::Load(FILE* file)
{
	if (!ComputerScreen::Load(file))
		return false;

	if (!FileReadData(&_nextPage, 4, 1, file))
		return false;

	if (!FileReadData(&_type, 4, 1, file))
		return false;

	return true;
}

void LinksScreen::Save(FILE* file)
{
	ComputerScreen::Save(file);
	fwrite(&this->_nextPage, 4, 1, file);
	fwrite(&this->_type, 4, 1, file);
}

void LinksScreen::Print()
{
	puts("LinksScreen :");
	ComputerScreen::Print();
	printf("TYPE=%d, nextpage=%d\n", _type, _nextPage);
}

const char* LinksScreen::GetID()
{
	return "SCR_LINK";
}

UplinkObjectId LinksScreen::GetOBJECTID()
{
	return UplinkObjectId::LinksScreen;
}

int LinksScreen::GetNextPage()
{
	return _nextPage;
}

int LinksScreen::GetScreenType()
{
	return _type;
}

void LinksScreen::SetNextPage(int nextPage)
{
	_nextPage = nextPage;
}

void LinksScreen::SetScreenType(int type)
{
	_type = type;
}
