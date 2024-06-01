#pragma once

#include <Interface/RemoteScreens/RemoteInterfaceScreen.hpp>
#include <UplinkObject.hpp>

class RemoteInterface : UplinkObject
{
public:
	RemoteInterfaceScreen* screen = nullptr;
	int previousScreenIndex = 0;
	int screenIndex = 0;
	char securityName[0x80] = " ";
	int securityLevel = 10;

	~RemoteInterface() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
	void Create();
	bool VerifyScreen(int index);
	bool IsVisible();
	void RunScreen(int screenIndex, Computer* computer);
	RemoteInterfaceScreen* GetInterfaceScreen();
};
