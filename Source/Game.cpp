#include <Game.hpp>
#include <Util.hpp>

Game::Game()
	: interface(nullptr), view(nullptr), world(nullptr), speed(0), obituary(nullptr), loadedSaveFileVer(nullptr), field_40(nullptr),
	  field_48(0), winCodeDesc(nullptr), field_58(nullptr), field_60(0), worldMapType(1)
{
}

Game::~Game()
{
	UplinkAbort("TODO: implement Game::~Game()");
}

bool Game::Load(FILE* file)
{
	(void)file;
	UplinkAbort("TODO: implement Game::Load(FILE*)");
}

void Game::Save(FILE* file)
{
	(void)file;
	UplinkAbort("TODO: implement Game::Save(FILE*)");
}

void Game::Print()
{
	UplinkAbort("TODO: implement Game::Print()");
}

void Game::Update()
{
	UplinkAbort("TODO: implement Game::Update()");
}

const char* Game::GetID()
{
	return "GAME";
}
