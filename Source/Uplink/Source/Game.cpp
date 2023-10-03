#include <Game.hpp>

#include "../TempDefs.hpp"

Game::Game()
{
	Game_Game(this);
}

Game::~Game()
{
	if (interface)
		delete interface;

	if (view)
		delete view;

	if (world)
		delete world;

	if (obituary)
		delete obituary;

	if (loadedSaveVersion)
		delete[] loadedSaveVersion;

	if (currentSaveVersion)
		delete[] currentSaveVersion;

	if (winningCodeDesc)
		delete[] winningCodeDesc;

	if (_unknown2)
		delete[] _unknown2;
}

bool Game::Load(FILE* file)
{
	return Game_Load(this, file);
}

void Game::Save(FILE* file)
{
	Game_Save(this, file);
}

void Game::Print()
{
	Game_Print(this);
}

void Game::Update()
{
	Game_Update(this);
}

const char* Game::GetID()
{
	return "GAME";
}
