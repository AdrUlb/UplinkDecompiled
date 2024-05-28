#include <LinksScreen.hpp>

#include <Util.hpp>

bool LinksScreen::Load(FILE* file)
{
	if (!ComputerScreen::Load(file))
		return false;

	if (!FileReadData(&nextPage, 4, 1, file))
		return false;

	if (!FileReadData(&type, 4, 1, file))
		return false;

	return true;
}

void LinksScreen::Save(FILE* file)
{
	ComputerScreen::Save(file);
	fwrite(&this->nextPage, 4, 1, file);
	fwrite(&this->type, 4, 1, file);
	SaveID_END(file);
}

void LinksScreen::Print()
{
	puts("LinksScreen :");
	ComputerScreen::Print();
	printf("TYPE=%d, nextpage=%d\n", type, nextPage);
}

const char* LinksScreen::GetID()
{
	return "SCR_LINK";
}

UplinkObjectId LinksScreen::GetOBJECTID()
{
	return UplinkObjectId::LinksScreen;
}

void LinksScreen::SetScreenType(int value)
{
	type = value;
}
