#include <ComputerScreens/DialogScreen.hpp>

#include <Util.hpp>

DialogScreen::~DialogScreen()
{
	DeleteLListData(reinterpret_cast<LList<UplinkObject*>*>(&_widgets));

	if (_returnKeyButtonName != nullptr)
		delete[] _returnKeyButtonName;

	if (_escapeKeyButtonName != nullptr)
		delete[] _escapeKeyButtonName;
}

bool DialogScreen::Load(FILE* file)
{
	if (!ComputerScreen::Load(file))
		return false;

	if (!LoadLList(reinterpret_cast<LList<UplinkObject*>*>(&_widgets), file))
		return false;

	if (!LoadDynamicString(_returnKeyButtonName, file))
		return false;

	if (!LoadDynamicString(_escapeKeyButtonName, file))
		return false;

	return true;
}

void DialogScreen::Save(FILE* file)
{
	ComputerScreen::Save(file);
	SaveLList(reinterpret_cast<LList<UplinkObject*>*>(&_widgets), file);
	SaveDynamicString(_returnKeyButtonName, file);
	SaveDynamicString(_escapeKeyButtonName, file);
}

void DialogScreen::Print()
{
	puts("DialogScreen");
	ComputerScreen::Print();
	PrintLList(reinterpret_cast<LList<UplinkObject*>*>(&_widgets));
	printf("ReturnKeyButton = %s\n", _returnKeyButtonName);
	printf("EscapeKeyButton = %s\n", _escapeKeyButtonName);
}

const char* DialogScreen::GetID()
{
	return "SCR_DLG";
}

UplinkObjectId DialogScreen::GetOBJECTID()
{
	return UplinkObjectId::DialogScreen;
}

LList<DialogScreenWidget*>* DialogScreen::GetWidgets()
{
	return &_widgets;
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
	_widgets.PutData(widget);
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
	_widgets.PutData(widget);
}

void DialogScreen::SetReturnKeyButton(const char* button)
{
	if (_returnKeyButtonName != nullptr)
		delete[] _returnKeyButtonName;

	_returnKeyButtonName = new char[strlen(button) + 1];
	strcpy(_returnKeyButtonName, button);
}

void DialogScreen::SetEscapeKeyButton(const char* button)
{
	if (_escapeKeyButtonName != nullptr)
		delete[] _escapeKeyButtonName;

	_escapeKeyButtonName = new char[strlen(button) + 1];
	strcpy(_escapeKeyButtonName, button);
}
