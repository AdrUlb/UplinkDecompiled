#pragma once

#include <Interface/LocalScreens/LocalInterfaceScreen.hpp>
#include <LList.hpp>

typedef void WorldMapLayout;

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
	void Create(int id);
	static int IsVisibleWorldMapInterface();
	static void CloseWorldMapInterface_Large();
	static void CreateWorldMapInterface_Small();
	static void CreateWorldMapInterface_Large();
	static void CreateWorldMapInterface(int type);
	static void RemoveTempConnectionButton();
	static void RemoveWorldMapInterface();
};

class SWInterface : LocalInterfaceScreen
{
	void Update() override;
	void Create() override;
	void Remove() override;
	bool IsVisible() override;

public:
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

class HUDInterface : LocalInterfaceScreen
{
	WorldMapInterface worldMapInterface;
	int field_54;
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
	static void MoveSelecter(int screenCode, int screenIndex);
};
