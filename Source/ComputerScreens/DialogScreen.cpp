#include <ComputerScreens/DialogScreen.hpp>

#include <Util.hpp>

DialogScreen::~DialogScreen()
{
	puts("DialogScreen");
	ComputerScreen::Print();
	PrintLList(reinterpret_cast<LList<UplinkObject*>*>(&widgets));
	printf("ReturnKeyButton = %s\n", returnKeyButton);
	printf("EscapeKeyButton = %s\n", escapeKeyButton);
}

bool DialogScreen::Load(FILE* file)
{
	if (!ComputerScreen::Load(file))
		return false;

	if (!LoadLList(reinterpret_cast<LList<UplinkObject*>*>(&widgets), file))
		return false;

	if (!LoadDynamicString(returnKeyButton, file))
		return false;

	if (!LoadDynamicString(escapeKeyButton, file))
		return false;

	return true;
}

void DialogScreen::Save(FILE* file)
{
	ComputerScreen::Save(file);
	SaveLList(reinterpret_cast<LList<UplinkObject*>*>(&widgets), file);
	SaveDynamicString(returnKeyButton, file);
	SaveDynamicString(escapeKeyButton, file);
	SaveID_END(file);
}

void DialogScreen::Print()
{
	puts("DialogScreen");
	ComputerScreen::Print();
	PrintLList(reinterpret_cast<LList<UplinkObject*>*>(&widgets));
	printf("ReturnKeyButton = %s\n", returnKeyButton);
	printf("EscapeKeyButton = %s\n", escapeKeyButton);
}

const char* DialogScreen::GetID()
{
	return "SCR_DLG";
}

UplinkObjectId DialogScreen::GetOBJECTID()
{
	return UplinkObjectId::DialogScreen;
}

void DialogScreen::AddWidget(const char* name, int type, int x, int y, int width, int height, const char* caption, const char* tooltip)
{
	puts("TODO: implement DialogScreen::AddWidget()");
}

void DialogScreen::AddWidget(const char* name, int type, int x, int y, int width, int height, const char* caption, const char* tooltip, int data1,
							 int data2, const char* stringData1, const char* stringData2)
{
	puts("TODO: implement DialogScreen::AddWidget()");
}