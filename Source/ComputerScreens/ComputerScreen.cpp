#include <ComputerScreens/ComputerScreen.hpp>

#include <Globals.hpp>
#include <Util.hpp>

bool ComputerScreen::Load(FILE* file)
{
	if (!LoadDynamicStringBuf(_mainTitle, 0x40, file))
		return false;

	if (!LoadDynamicStringBuf(_subTitle, 0x40, file))
		return false;

	if (!LoadDynamicStringBuf(_computer, 0x40, file))
		return false;

	return true;
}

void ComputerScreen::Save(FILE* file)
{
	SaveDynamicString(_mainTitle, 0x40, file);
	SaveDynamicString(_subTitle, 0x40, file);
	SaveDynamicString(_computer, 0x40, file);
}

void ComputerScreen::Print()
{
	printf("MainTitle : %s\n", _mainTitle);
	printf("SubTitle  : %s\n", _subTitle);
	printf("Computer  : %s\n", _computer);
}

const char* ComputerScreen::GetID()
{
	return "COMPSCR";
}

const char* ComputerScreen::GetMainTitle()
{
	return _mainTitle;
}
const char* ComputerScreen::GetSubTitle()
{
	return _subTitle;
}

Computer* ComputerScreen::GetComputer()
{
	return game->GetWorld().GetComputer(_computer);
}

void ComputerScreen::SetMainTitle(const char* mainTitle)
{
	UplinkStrncpy(_mainTitle, mainTitle, 0x40);
}

void ComputerScreen::SetSubTitle(const char* subTitle)
{
	UplinkStrncpy(_subTitle, subTitle, 0x40);
}

void ComputerScreen::SetComputer(const char* value)
{
	UplinkStrncpy(_computer, value, 0x40);
}
