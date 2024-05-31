#include <ComputerScreens/MenuScreen.hpp>

#include <Util.hpp>

MenuScreen::~MenuScreen()
{
	DeleteLListData(reinterpret_cast<LList<UplinkObject*>*>(&options));
}

bool MenuScreen::Load(FILE* file)
{
	if (!ComputerScreen::Load(file))
		return false;

	if (!LoadLList(reinterpret_cast<LList<UplinkObject*>*>(&options), file))
		return false;

	return true;
}

void MenuScreen::Save(FILE* file)
{
	ComputerScreen::Save(file);
	SaveLList(reinterpret_cast<LList<UplinkObject*>*>(&options), file);
}

void MenuScreen::Print()
{
	puts("MenuScreen : ");
	ComputerScreen::Print();
	PrintLList(reinterpret_cast<LList<UplinkObject*>*>(&options));
}

const char* MenuScreen::GetID()
{
	return "SCR_MENU";
}

UplinkObjectId MenuScreen::GetOBJECTID()
{
	return UplinkObjectId::MenuScreen;
}

void MenuScreen::AddOption(const char* caption, const char* tooltip, int nextPage, int security, int index)
{
	const auto option = new MenuScreenOption();

	option->SetCaption(caption);
	option->SetTooltip(tooltip);
	option->SetNextPage(nextPage);
	option->SetSecurity(security);

	if (index == -1)
	{
		options.PutData(option);
		return;
	}

	options.PutDataAtIndex(option, index);
}
