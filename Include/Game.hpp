#pragma once

#include <GameObituary.hpp>
#include <Interface.hpp>
#include <UplinkObject.hpp>
#include <View.hpp>
#include <World.hpp>
#include <ctime>

class Game : UplinkObject
{
	Interface* interface;
	View* view;
	World* world;
	int speed;
	GameObituary* obituary;
	time_t lastAutosaveTime;
	const char* loadedSaveFileVer;
	char* createdSaveFileVer;
	int field_48;
	const char* winCodeDesc;
	const char* field_58;
	int field_60;
	int worldMapType;

public:
	Game();
	~Game() override;

	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
	int GameSpeed();
	World* GetWorld();
	View* GetView();
	Interface* GetInterface();
	bool IsRunning();
	void NewGame();
	const char* GetLoadedSavefileVer();
	int GetWorldMapType();
};
