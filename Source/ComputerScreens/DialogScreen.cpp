#include <ComputerScreens/DialogScreen.hpp>

#include <Util.hpp>

DialogScreen::~DialogScreen()
{
	DeleteLListData(reinterpret_cast<LList<UplinkObject*>*>(&this->widgets));

	if (returnKeyButton != nullptr)
		delete[] returnKeyButton;

	if (escapeKeyButton != nullptr)
		delete[] escapeKeyButton;
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
	const auto widget = new DialogScreenWidget();
	widget->SetName(name);
	widget->SetTYPE(type);
	widget->SetPosition(x, y);
	widget->SetSize(width, height);
	widget->SetCaption(caption);
	widget->SetTooltip(tooltip);
	widgets.PutData(widget);
}

void DialogScreen::AddWidget(const char* name, int type, int x, int y, int width, int height, const char* caption, const char* tooltip, int data1,
							 int data2, const char* stringData1, const char* stringData2)
{
	const auto widget = new DialogScreenWidget();
	widget->SetName(name);
	widget->SetTYPE(type);
	widget->SetPosition(x, y);
	widget->SetSize(width, height);
	widget->SetCaption(caption);
	widget->SetTooltip(tooltip);
	widget->SetData(data1, data2);
	widget->SetStringData(stringData1, stringData2);
	widgets.PutData(widget);
}