#pragma once

#include <DArray.hpp>
#include <UplinkObject.hpp>

class InterfaceScreen : public UplinkObject
{
	DArray<char*>* buttonNames;

public:
	InterfaceScreen();
	~InterfaceScreen() override;
	virtual void RegisterButton(int x, int y, int width, int height, const char* caption, const char* name);
	virtual void RegisterButton(int x, int y, int width, int height, const char* caption, const char* tooltip, const char* name);
	virtual void Create();
	virtual void Remove();
	virtual bool IsVisible();
	virtual int ScreenID();
};
