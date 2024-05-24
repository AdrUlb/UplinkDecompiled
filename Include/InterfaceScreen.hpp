#pragma once

#include <DArray.hpp>
#include <UplinkObject.hpp>

class InterfaceScreen : UplinkObject
{
	DArray<char*>* buttonNames;

	InterfaceScreen();
	~InterfaceScreen() override;
	void Update() override;
	virtual void RegisterButton(int x, int y, int width, int height, const char* caption, const char* name);
	virtual void RegisterButton(int x, int y, int width, int height, const char* caption, const char* tooltip, const char* name);
	virtual void Create();
	virtual void Remove();
	virtual bool IsVisible();
	virtual int ScreenID();
};
