#include <Interface/InterfaceScreen.hpp>

#include <Eclipse.hpp>
#include <Util.hpp>

InterfaceScreen::InterfaceScreen()
{
	buttonNames = new DArray<char*>();
}

InterfaceScreen::~InterfaceScreen()
{
	DeleteDArrayDataD(buttonNames);
	if (buttonNames != nullptr)
	{
		delete buttonNames;
		buttonNames = nullptr;
	}
}

void InterfaceScreen::RegisterButton(int x, int y, int width, int height, const char* caption, const char* name)
{
	EclRegisterButton(x, y, width, height, caption, name);

	const auto buttonName = new char[strlen(name) + 1];
	strcpy(buttonName, name);

	buttonNames->PutData(buttonName);
}

void InterfaceScreen::RegisterButton(int x, int y, int width, int height, const char* caption, const char* tooltip, const char* name)
{
	EclRegisterButton(x, y, width, height, caption, tooltip, name);

	const auto buttonName = new char[strlen(name) + 1];
	strcpy(buttonName, name);

	buttonNames->PutData(buttonName);
}

void InterfaceScreen::Create() {}

void InterfaceScreen::Remove()
{

	for (auto i = 0; i < buttonNames->Size(); i++)
	{
		if (!buttonNames->ValidIndex(i))
			continue;

		const auto name = buttonNames->GetData(i);
		if (name != nullptr && name[0] != 0)
			EclRemoveButton(name);
	}
}

bool InterfaceScreen::IsVisible()
{
	return false;
}

int InterfaceScreen::ScreenID()
{
	return 0;
}
