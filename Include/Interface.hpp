#pragma once

#include <Computer.hpp>
#include <ComputerScreens/ComputerScreen.hpp>
#include <Image.hpp>
#include <InterfaceScreen.hpp>
#include <LList.hpp>
#include <TaskManager.hpp>
#include <UplinkObject.hpp>

typedef void WorldMapLayout;

struct RemoteInterfaceScreen : InterfaceScreen
{
	ComputerScreen* computerScreen = nullptr;

	void Update() override;
	void Create() override;
	void Remove() override;
	bool IsVisible() override;
	int ScreenID() override;
	virtual void Create(ComputerScreen* screen);
	virtual bool ReturnKeyPressed();
	virtual bool EscapeKeyPressed();
};

class RemoteInterface : UplinkObject
{
	RemoteInterfaceScreen* screen = nullptr;
	int previousScreenIndex = 0;
	int screenIndex = 0;
	char securityName[0x80] = " ";
	int securityLevel = 10;

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
	void RunScreen(int screenIndex, Computer* computer);
	RemoteInterfaceScreen* GetInterfaceScreen();
};

class GatewayInterface : LocalInterfaceScreen
{
	void Update() override;
	void Create() override;
	void Remove() override;
	bool IsVisible() override;
	int ScreenID() override;
};

class SWInterface : LocalInterfaceScreen
{
	void Update() override;
	void Create() override;
	void Remove() override;
	bool IsVisible() override;
};

class WorldMapInterface : LocalInterfaceScreen
{
	WorldMapLayout* layout = nullptr;
	LList<char*> savedConnection;
	int field_38 = 0;
	int field_3c = 0;
	int field_40 = 0;

public:
	~WorldMapInterface() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
	void Create() override;
	void Remove() override;
	bool IsVisible() override;
	int ScreenID() override;
};

class HUDInterface : LocalInterfaceScreen
{
	WorldMapInterface worldMapInterface;
	char _54[4];
	SWInterface softwareInterface;
	GatewayInterface gatewayInterface;
	char* highlightedToolbarButton = nullptr;
	Image* image = nullptr;
	char upgrades[8]{0};

public:
	~HUDInterface() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Update() override;
	const char* GetID() override;
	void Create() override;
	bool IsVisible() override;
	int ScreenID() override;
};

class LocalInterface : UplinkObject
{
	LocalInterfaceScreen* screen = nullptr;
	HUDInterface* hud = new HUDInterface();
	int screenCode = 0;
	int screenIndex = 0;

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
};

class Interface : UplinkObject
{
	LocalInterface* localInterface = new LocalInterface();
	RemoteInterface* remoteInterface = new RemoteInterface();
	TaskManager* taskManager;

public:
	Interface();
	~Interface() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
	void Create();
	LocalInterface* GetLocalInterface();
	RemoteInterface* GetRemoteInterface();
};
