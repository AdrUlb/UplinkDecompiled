#include <GameObituary.hpp>

#include <Globals.hpp>
#include <Util.hpp>

GameObituary::~GameObituary()
{
	if (_gameOverReason != nullptr)
		delete[] _gameOverReason;
}

bool GameObituary::Load(FILE* file)
{
	if (strcmp(game->GetLoadedSavefileVer(), "SAV59") >= 0)
	{
		if (!LoadDynamicStringBuf(_name, 0x80, file))
			return false;
	}
	else
	{
		const auto success = FileReadData(&_name, 0x80, 1, file);
		_name[0x7F] = 0;
		if (!success)
			return false;
	}

	if (!FileReadData(&_money, 4, 1, file))
		return false;

	if (!FileReadData(&_uplinkRating, 4, 1, file))
	{
		return false;
	}
	if (!FileReadData(&_neuromancerRating, 4, 1, file))
	{
		return false;
	}
	if (!FileReadData(&_specialMissionsCompleted, 4, 1, file))
	{
		return false;
	}
	if (!FileReadData(&_livesRuined, 4, 1, file))
	{
		return false;
	}
	if (!FileReadData(&_systemsDestroyed, 4, 1, file))
	{
		return false;
	}
	if (!FileReadData(&_highSecurityHacks, 4, 1, file))
	{
		return false;
	}
	if (!FileReadData(&_score, 4, 1, file))
	{
		return false;
	}
	if (!FileReadData(&_demoGameOver, 1, 1, file))
	{
		return false;
	}
	if (!FileReadData(&_warezGameOver, 1, 1, file))
		return false;

	if (!LoadDynamicString(_gameOverReason, file))
		return false;

	return true;
}

void GameObituary::Save(FILE* file)
{
	SaveDynamicString(_name, 0x80, file);
	fwrite(&_money, 4, 1, file);
	fwrite(&_uplinkRating, 4, 1, file);
	fwrite(&_neuromancerRating, 4, 1, file);
	fwrite(&_specialMissionsCompleted, 4, 1, file);
	fwrite(&_livesRuined, 4, 1, file);
	fwrite(&_systemsDestroyed, 4, 1, file);
	fwrite(&_highSecurityHacks, 4, 1, file);
	fwrite(&_score, 4, 1, file);
	fwrite(&_demoGameOver, 1, 1, file);
	fwrite(&_warezGameOver, 1, 1, file);
	SaveDynamicString(_gameOverReason, file);
}

void GameObituary::Print()
{
	puts("Game obituary: ");
	printf("Name : %s\n", _name);
	printf("Money %d, Uplink %d, Neuromancer %d\n", _money, _uplinkRating, _neuromancerRating);
	printf("SpecialMissionsCompleted : %d\n", _specialMissionsCompleted);
	printf("Score : People's lives ruined : %d\n", _livesRuined);
	printf("Score : Systems destroyed : %d\n", _systemsDestroyed);
	printf("Score : High Security Hacks : %d\n", _highSecurityHacks);
	printf("Score %d\n", _score);
	printf("DemoGameOver = %d\n", _demoGameOver);
	printf("Game over reason = %s\n", _gameOverReason);
}

const char* GameObituary::GetID()
{
	return "GOBIT";
}
