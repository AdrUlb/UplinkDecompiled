#pragma once

#include <Interface/LocalScreens/LocalInterfaceScreen.hpp>
#include <Interface/LocalScreens/WorldMapInterface.hpp>
#include <LList.hpp>

enum class WorldMapInterfaceType
{
	None,
	Small,
	Large
};
class WorldMapInterfaceObject
{
	int _type;
	int _x;
	int _y;
	int _baseX;
	int _baseY;
	bool _field_1c;
	char* _ip;
	bool _playerHasLink;
	bool _colored;

public:
	WorldMapInterfaceObject();
	virtual ~WorldMapInterfaceObject();
};

class WorldMapInterfaceLabel
{
};

struct WorldMapRect
{
	int X;
	int Y;
	int Width;
	int Height;
};

class WorldMapObjectiveFunction
{
	int _cost;
	int _width;
	int _height;
	int* _array;
	WorldMapRect _rect;

public:
	WorldMapObjectiveFunction(const WorldMapRect& rect);
	~WorldMapObjectiveFunction();
	void Reset();
};

class WorldMapLayout
{
	// TODO: come up with proper field names
	int _field_0;
	int _field_4;
	int _field_8;
	float _field_c;
	float _field_10;
	bool _layoutComplete;
	bool _field_15;
	WorldMapRect _rect;
	LList<WorldMapInterfaceObject*> _objects;
	LList<WorldMapInterfaceLabel*> _labels;
	WorldMapObjectiveFunction _objectiveFunction;

public:
	WorldMapLayout(const WorldMapRect& rect);
	~WorldMapLayout();
	void Reset();
	void ResetLayoutParameters();
	void AddLocation(int x, int y, const char* name, const char* ip, bool arg6); // TODO: figure out what arg6 is
	void DeleteLocations();
};

class WorldMapInterface : LocalInterfaceScreen
{
	WorldMapLayout* _layout = nullptr;
	LList<char*> _savedConnection;
	int _field_38 = 0;
	int _field_3c = 0;
	int _field_40 = 0;
	static float _zoom;
	static float _scrollX;
	static float _scrollY;

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
	void Create(WorldMapInterfaceType type);
	void ProgramLayoutEngine();
	static int GetLargeMapWidth();
	static int GetLargeMapHeight();
	static WorldMapRect GetLargeMapRect();
	static WorldMapInterfaceType IsVisibleWorldMapInterface();
	static void CloseWorldMapInterface_Large();
	static void CreateWorldMapInterface_Small();
	static void CreateWorldMapInterface_Large();
	static void CreateWorldMapInterface(WorldMapInterfaceType type);
	static void RemoveTempConnectionButton();
	static void RemoveWorldMapInterface();
	static void UpdateAccessLevel();
	static int GetScaledX(int x, WorldMapInterfaceType worldmapInterfaceType);
	static int GetScaledY(int y, WorldMapInterfaceType worldmapInterfaceType);
	static void DrawLocation(Button* button, bool highlighted, bool clicked);
	static void LocationClick(Button* button);
};

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
	char _upgrades[8]{ 0 };

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
