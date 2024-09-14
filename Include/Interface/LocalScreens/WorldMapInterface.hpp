#pragma once

#include <Interface/LocalScreens/LocalInterfaceScreen.hpp>
#include <LList.hpp>

struct WorldMapRect
{
	int X;
	int Y;
	int Width;
	int Height;

	WorldMapRect(int x, int y, int width, int height);
	explicit WorldMapRect(const WorldMapRect* rect);
};

class WorldMapObjectiveFunction
{
	int _cost;
	int _width;
	int _height;
	int* _array;
	WorldMapRect _rect;

public:
	explicit WorldMapObjectiveFunction(const WorldMapRect* rect);
	~WorldMapObjectiveFunction();
	void Reset();
};

class WorldMapInterfaceObject
{
	unsigned int _type = 0;
	int x = 0;
	int y = 0;
	int baseX = 0;
	int baseY = 0;
	char* _ip = nullptr;
	bool _isGreen = false;
	bool _isOrange = false;

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
	int _field_0 = 0;
	int _field_4 = 0;
	int _field_8 = 0;
	float _field_c = 0.0f;
	float _field_10 = 0.0f;
	bool _layoutComplete = false;
	bool _layoutStarted = false;
	WorldMapRect _rect;
	LList<WorldMapInterfaceObject*> _objects;
	LList<WorldMapInterfaceLabel*> _labels;
	WorldMapObjectiveFunction _objectiveFunction;

public:
	explicit WorldMapLayout(const WorldMapRect* rect);
	void Reset();
	void ResetLayoutParameters();
	void DeleteLocations();
	void AddLocation(int x, int y, const char* computerName, const char* ip, bool isUnknown);
};

class WorldMapInterface : LocalInterfaceScreen
{
	WorldMapLayout* _layout = nullptr;
	LList<char*> _savedConnection;
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
	void ProgramLayoutEngine();
	static int IsVisibleWorldMapInterface();
	static void CloseWorldMapInterface_Large();
	static void CreateWorldMapInterface_Small();
	static void CreateWorldMapInterface_Large();
	static void CreateWorldMapInterface(int type);
	static void RemoveTempConnectionButton();
	static void RemoveWorldMapInterface();
};
