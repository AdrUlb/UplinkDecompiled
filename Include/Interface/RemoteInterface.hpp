#pragma once

#include <Interface/RemoteScreens/RemoteInterfaceScreen.hpp>
#include <UplinkObject.hpp>

class RemoteInterface : UplinkObject
{
	RemoteInterfaceScreen* _screen = nullptr;
	int _previousScreenIndex = 0;
	int _screenIndex = 0;
	char _securityName[0x80] = " ";
	int _securityLevel = 10;

public:
	~RemoteInterface() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
	void Create();
	bool VerifyScreen(int index);
	bool IsVisible();
	ComputerScreen* GetComputerScreen();
	int GetSecurityLevel();
	RemoteInterfaceScreen* GetInterfaceScreen();
	void RunScreen(int screenIndex, Computer* computer);
	void RunNewLocation();
};
