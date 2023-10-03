#pragma once

#include <time.h>
#include <UplinkObject.hpp>
#include <Interface.hpp>
#include <View.hpp>
#include <World.hpp>
#include <GameObituary.hpp>

class Game : UplinkObject
{
	Interface* interface = nullptr;
	View* view = nullptr;
	World* world = nullptr;
	int speed = 0;
	GameObituary* obituary = nullptr;
	char* loadedSaveVersion = nullptr;
	char* currentSaveVersion = nullptr;
	int _unknown1 = 0;
	char* winningCodeDesc = nullptr;
	char* _unknown2 = nullptr;
	unsigned int _unknown3 = 0;
	int _unknown4 = 1;

public:
	Game();
	virtual ~Game();
private:
	virtual bool Load(FILE* file);
	virtual void Save(FILE* file);
	virtual void Print();
	virtual void Update();
	virtual const char* GetID();
};
