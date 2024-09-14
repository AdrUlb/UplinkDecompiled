#pragma once

#include <Interface/LocalScreens/LocalInterfaceScreen.hpp>
#include <Interface/LocalScreens/WorldMapInterface.hpp>
#include <LList.hpp>

class SWInterface : LocalInterfaceScreen
{
public:
	void Update() override;
	void Create() override;
	void Remove() override;
	bool IsVisible() override;
	static bool IsVisibleSoftwareMenu();
	static void ToggleSoftwareMenu();
};

class GatewayInterface : LocalInterfaceScreen
{
	void Update() override;
	void Create() override;
	void Remove() override;
	bool IsVisible() override;
	int ScreenID() override;
};

struct HUDUpgrade
{
	int Id;
	const char* Name;
	const char* Description;
	const char* ButtonName;
	const char* FileName;
	const char* FileNameHighlight;
	const char* FileNameClicked;
	ButtonMouseUpFunc Callback;
};

class HUDInterface : LocalInterfaceScreen
{
	WorldMapInterface worldMapInterface;
	int field_54;
	SWInterface softwareInterface;
	GatewayInterface gatewayInterface;
	char* highlightedToolbarButton = nullptr;
	Image* image = nullptr;
	char _upgrades[8]{0};

public:
	~HUDInterface() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Update() override;
	const char* GetID() override;
	void Create() override;
	bool IsVisible() override;
	int ScreenID() override;
	static void MoveSelecter(int screenCode, int screenIndex);
	static void CloseGame();
	HUDUpgrade* GetUpgrade(char upgrade);
	bool IsUpgradeVisible(char upgrade);
	void AddUpgrade(char upgrade);
};
