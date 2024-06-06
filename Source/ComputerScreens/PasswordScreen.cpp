#include <ComputerScreens/PasswordScreen.hpp>

#include <Globals.hpp>

bool PasswordScreen::Load(FILE* file)
{
	if (!ComputerScreen::Load(file))
		return false;

	if (!FileReadData(&_nextPage, 4, 1, file))
		return false;

	if (strcmp(game->GetLoadedSavefileVer(), "SAV59") >= 0)
	{
		if (!LoadDynamicStringBuf(_password, 0x40, file))
			return false;

		if (!FileReadData(&_difficulty, 4, 1, file))
			return false;
	}
	else
	{
		const auto success = FileReadData(&_password, 0x40, 1, file);
		_password[0x3F] = 0;
		if (success == 0)
			return false;

		if (!FileReadData(&_difficulty, 4, 1, file))
			return false;
	}

	return true;
}

void PasswordScreen::Save(FILE* file)
{
	ComputerScreen::Save(file);
	fwrite(&_nextPage, 4, 1, file);
	SaveDynamicString(_password, 0x40, file);
	fwrite(&_difficulty, 4, 1, file);
}

void PasswordScreen::Print()
{
	puts("PasswordScreen : ");
	ComputerScreen::Print();
	printf("Nextpage = %d, Password = %s, Difficulty = %d\n", _nextPage, _password, _difficulty);
}

const char* PasswordScreen::GetID()
{
	return "SCR_PASS";
}

UplinkObjectId PasswordScreen::GetOBJECTID()
{
	return UplinkObjectId::PasswordScreen;
}

int PasswordScreen::GetNextPage()
{
	return _nextPage;
}

const char* PasswordScreen::GetPassword()
{
	return _password;
}

void PasswordScreen::SetNextPage(int nextPage)
{
	_nextPage = nextPage;
}

void PasswordScreen::SetDifficulty(int difficulty)
{
	_difficulty = difficulty;
}

void PasswordScreen::SetPassword(const char* password)
{
	UplinkStrncpy(_password, password, 0x40);
}
