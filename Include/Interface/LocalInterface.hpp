#pragma once

#include <Interface/LocalScreens/HUDInterface.hpp>
#include <Interface/LocalScreens/LocalInterfaceScreen.hpp>
#include <UplinkObject.hpp>

class LocalInterface : UplinkObject
{
	LocalInterfaceScreen* _screen = nullptr;
	HUDInterface* _hud = new HUDInterface();
	int _screenCode = 0;
	int _screenIndex = 0;

public:
	~LocalInterface() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
	HUDInterface* GetHUD();
	bool VerifyScreen(int screenCode, int screenIndex);
	void RunScreen(int code, int index);
	int InScreen();
	LocalInterfaceScreen* GetInterfaceScreen();
	bool IsVisible();
	void Create();
	void Remove();
	void Reset();
};
