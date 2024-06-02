#include <ComputerScreens/HighSecurityScreen.hpp>

#include <Util.hpp>

HighSecurityScreen::~HighSecurityScreen()
{
	DeleteLListData(reinterpret_cast<LList<UplinkObject*>*>(&_options));
}

bool HighSecurityScreen::Load(FILE* file)
{
	if (!ComputerScreen::Load(file))
		return false;

	if (!LoadLList(reinterpret_cast<LList<UplinkObject*>*>(&_options), file))
		return false;

	if (!FileReadData(&_nextPage, 4, 1, file))
		return false;

	return true;
}

void HighSecurityScreen::Save(FILE* file)
{
	ComputerScreen::Save(file);
	SaveLList(reinterpret_cast<LList<UplinkObject*>*>(&_options), file);
	fwrite(&_nextPage, 4, 1, file);
}

void HighSecurityScreen::Print()
{
	puts("HighSecurityScreen : ");
	ComputerScreen::Print();
	PrintLList(reinterpret_cast<LList<UplinkObject*>*>(&_options));
	printf("nextpage = %d\n", _nextPage);
}

const char* HighSecurityScreen::GetID()
{
	return "SCR_HI";
}

UplinkObjectId HighSecurityScreen::GetOBJECTID()
{
	return UplinkObjectId::HighSecurityScreen;
}

void HighSecurityScreen::SetNextPage(int nextPage)
{
	this->_nextPage = nextPage;
}

void HighSecurityScreen::AddSystem(const char* caption, int nextPage)
{
	const auto option = new MenuScreenOption();
	option->SetCaption(caption);
	option->SetNextPage(nextPage);
	option->SetSecurity(10);
	_options.PutData(option);
}
