#pragma once

#include <GameObituary.hpp>
#include <Interface.hpp>
#include <UplinkObject.hpp>
#include <View.hpp>
#include <World.hpp>
#include <ctime>

class Game : UplinkObject
{
	Interface* _interface;
	View* _view;
	World* _world;
	int _speed;
	GameObituary* _obituary;
	time_t _lastAutosaveTime = 0;
	char* _loadedSaveFileVer;
	char* _createdSaveFileVer;
	int _field_48;
	char* _winCodeDesc;
	char* _field_58;
	int _field_60;
	int _worldMapType;

public:
	Game();
	~Game() override;

	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
	Interface& GetInterface();
	View& GetView();
	World& GetWorld();
	int GetGameSpeed();
	int GetWorldMapType();
	const char* GetLoadedSavefileVer();
	void SetGameSpeed(int speed);
	bool IsRunning();
	void NewGame();
	bool LoadGame(FILE* file);
	void ExitGame();
};
