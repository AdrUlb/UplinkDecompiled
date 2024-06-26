#pragma once

#include <DArray.hpp>

enum class MainMenuScreenCode
{
	Unknown = 0x0,
	Login = 0x1,
	Loading = 0x2,
	FirstTimeLoading = 0x3,
	Options = 0x4,
	Obituary = 0x5,
	ConnectionLost = 0x6,
	Disavowed = 0x7,
	TheTeam = 0x8,
	Closing = 0x9,
	DemoGameOver = 0xa,
	RevelationWon = 0xb,
	RevelationLost = 0xc,
	WarezGameOver = 0xd,
	GenericOptions = 0x14,
	NetworkOptions = 0x15,
	Theme = 0x16,
	GraphicOptions = 0x17
};

class MainMenuScreen
{
	DArray<char*>* buttons;

public:
	MainMenuScreen();
	virtual ~MainMenuScreen();
	virtual void Create();
	virtual void Remove();
	virtual void Update();
	virtual bool IsVisible();
	virtual MainMenuScreenCode ScreenID();
	virtual bool ReturnKeyPressed();
	void RegisterButton(int x, int y, int width, int height, const char* caption, const char* name);
};