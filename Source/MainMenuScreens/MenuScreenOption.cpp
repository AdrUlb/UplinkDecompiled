#include <ComputerScreens/MenuScreenOption.hpp>

#include <Util.hpp>

bool MenuScreenOption::Load(FILE* file)
{
	if (!LoadDynamicStringBuf(caption, 0x40, file))
		return false;

	if (!LoadDynamicStringBuf(tooltip, 0x80, file))
		return false;

	if (!FileReadData(&nextPage, 4, 1, file))
		return false;

	if (!FileReadData(&security, 4, 1, file))
		return false;

	return true;
}

void MenuScreenOption::Save(FILE* file)
{
	SaveDynamicString(caption, 0x40, file);
	SaveDynamicString(tooltip, 0x80, file);
	fwrite(&nextPage, 4, 1, file);
	fwrite(&security, 4, 1, file);
}

void MenuScreenOption::Print()
{
	puts("MenuScreenOption : ");
	printf("\tNextPage = %d, Caption = %s\n", nextPage, caption);
	printf("\tTooltip = %s, security = %d\n", tooltip, security);
}

const char* MenuScreenOption::GetID()
{
	return "MNUOPT";
}

UplinkObjectId MenuScreenOption::GetOBJECTID()
{
	return UplinkObjectId::MenuScreenOption;
}

void MenuScreenOption::SetCaption(const char* value)
{
	UplinkStrncpy(caption, value, 0x40);
}

void MenuScreenOption::SetTooltip(const char* value)
{
	UplinkStrncpy(tooltip, value, 0x80);
}

void MenuScreenOption::SetNextPage(int value)
{
	nextPage = value;
}

void MenuScreenOption::SetSecurity(int value)
{
	security = value;
}