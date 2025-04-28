#pragma once

#include <Interface/LocalScreens/LocalInterfaceScreen.hpp>
#include <LList.hpp>

enum class WorldMapInterfaceType
{
	None,
	Small,
	Large
};

struct WorldMapRect
{
	int X;
	int Y;
	int Width;
	int Height;

	WorldMapRect(int x, int y, int width, int height);
	explicit WorldMapRect(const WorldMapRect& rect);
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

class WorldMapInterfaceObject
{
	int _type = 0;
	int _x = 0;
	int _y = 0;
	int _baseX = 0;
	int _baseY = 0;
	char* _ip = nullptr;
	bool _playerHasLinkMAYBE = false; // TODO: figure out what this ACTUALLY is
	bool _isColored = false;

public:
	virtual ~WorldMapInterfaceObject();
	virtual WorldMapRect GetExtent();
};

class WorldMapInterfaceLabel : WorldMapInterfaceObject
{
	char* _caption = nullptr;
	WorldMapInterfaceObject* _object;
	int _position;
	int _width = 0;
	int _positionCount;
	int _positions[9];

public:
	~WorldMapInterfaceLabel() override;
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
	bool _layoutStarted;
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
	static void DrawWorldMapSmall(Button* button, bool highlighted, bool clicked);
	static void DrawLocation(Button* button, bool highlighted, bool clicked);
	static void LocationClick(Button* button);
};
