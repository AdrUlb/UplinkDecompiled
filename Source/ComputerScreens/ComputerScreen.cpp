#include <ComputerScreens/ComputerScreen.hpp>

#include <Globals.hpp>
#include <Util.hpp>

bool ComputerScreen::Load(FILE* file)
{
	if (!LoadDynamicStringBuf(mainTitle, 0x40, file))
		return false;

	if (!LoadDynamicStringBuf(subTitle, 0x40, file))
		return false;

	if (!LoadDynamicStringBuf(computer, 0x40, file))
		return false;

	return true;
}

void ComputerScreen::Save(FILE* file)
{
	SaveDynamicString(mainTitle, 0x40, file);
	SaveDynamicString(subTitle, 0x40, file);
	SaveDynamicString(computer, 0x40, file);
}

void ComputerScreen::Print()
{
	printf("MainTitle : %s\n", mainTitle);
	printf("SubTitle  : %s\n", subTitle);
	printf("Computer  : %s\n", computer);
}

const char* ComputerScreen::GetID()
{
	return "COMPSCR";
}

Computer* ComputerScreen::GetComputer()
{
	return game->GetWorld()->GetComputer(computer);
}

void ComputerScreen::SetComputer(const char* value)
{
	UplinkStrncpy(computer, value, 0x40);
}

void ComputerScreen::SetMainTitle(const char* value)
{
	UplinkStrncpy(mainTitle, value, 0x40);
}

void ComputerScreen::SetSubTitle(const char* value)
{
	UplinkStrncpy(subTitle, value, 0x40);
}
