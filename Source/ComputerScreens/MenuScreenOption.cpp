#include <ComputerScreens/MenuScreenOption.hpp>

#include <Util.hpp>

bool MenuScreenOption::Load(FILE* file)
{
	if (!LoadDynamicStringBuf(_caption, 0x40, file))
		return false;

	if (!LoadDynamicStringBuf(_tooltip, 0x80, file))
		return false;

	if (!FileReadData(&_nextPage, 4, 1, file))
		return false;

	if (!FileReadData(&_security, 4, 1, file))
		return false;

	return true;
}

void MenuScreenOption::Save(FILE* file)
{
	SaveDynamicString(_caption, 0x40, file);
	SaveDynamicString(_tooltip, 0x80, file);
	fwrite(&_nextPage, 4, 1, file);
	fwrite(&_security, 4, 1, file);
}

void MenuScreenOption::Print()
{
	puts("MenuScreenOption : ");
	printf("\tNextPage = %d, Caption = %s\n", _nextPage, _caption);
	printf("\tTooltip = %s, security = %d\n", _tooltip, _security);
}

const char* MenuScreenOption::GetID()
{
	return "MNUOPT";
}

UplinkObjectId MenuScreenOption::GetOBJECTID()
{
	return UplinkObjectId::MenuScreenOption;
}

const char* MenuScreenOption::GetCaption()
{
	return _caption;
}

const char* MenuScreenOption::GetTooltip()
{
	return _tooltip;
}

int MenuScreenOption::GetNextPage()
{
	return _nextPage;
}

int MenuScreenOption::GetSecurity()
{
	return _security;
}

void MenuScreenOption::SetCaption(const char* caption)
{
	UplinkStrncpy(_caption, caption, 0x40);
}

void MenuScreenOption::SetTooltip(const char* tooltip)
{
	UplinkStrncpy(_tooltip, tooltip, 0x80);
}

void MenuScreenOption::SetNextPage(int nextPage)
{
	_nextPage = nextPage;
}

void MenuScreenOption::SetSecurity(int security)
{
	_security = security;
}