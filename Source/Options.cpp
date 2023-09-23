#include "Options.hpp"

#include <cstring>
#include <cassert>
#include "Util.hpp"
#include "App.hpp"
#include "RedShirt.hpp"
#include "../UplinkDecompiledTempGlobals.hpp"
#include "../UplinkDecompiledTempDefs.hpp"

constexpr auto optionsFileVersionCurrent = "SAV62";
constexpr auto optionsFileVersionMin = "SAV56";
constexpr size_t optionsFileVersionLength = 6;

Options::Options()
{
	strncpy(themeName, "graphics", OPTIONS_THEMENAME_MAX);
}

Options::~Options()
{
	DeleteBTreeData<Option*>(&options);

	const auto colorOptions = this->colorOptions.ConvertToDArray();
	const auto colorOptionsSize = colorOptions->Size();
	for (auto i = 0; i < colorOptionsSize; i++)
	{
		if (colorOptions->ValidIndex(i) != 0)
		{
			const auto colorOption = colorOptions->GetData(i);
			if (colorOption)
				delete colorOption;
		}
	}

	delete colorOptions;
}

bool Options::Load(FILE* file)
{
	char optionsFilePath[APP_PATH_MAX];
	UplinkSnprintf(optionsFilePath, APP_PATH_MAX, "%soptions", gApp->UsersPath);
	printf("Loading uplink options from %s...", optionsFilePath);

	auto fileIsRedshirt = RsFileEncryptedNoVerify(optionsFilePath);

	FILE* optionsFile;
	if (fileIsRedshirt)
	{
		if (!RsFileEncrypted(optionsFilePath))
		{
			puts("failed");
			return false;
		}
		optionsFile = RsFileOpen(optionsFilePath, "rb");
	}
	else
		optionsFile = fopen(optionsFilePath, "rb");

	if (!optionsFile)
	{
		puts("failed");
		return false;
	}

	char optionsFileVersion[32];
	if (!FileReadDataInt("options/options.cpp", 430, optionsFileVersion, optionsFileVersionLength, 1, optionsFile) ||
		strcmp(optionsFileVersion, optionsFileVersionMin) < 0 || strcmp(optionsFileVersion, optionsFileVersionCurrent) > 0)
	{
		puts("\nERROR : Could not load options due to incompatible version format");

		if (fileIsRedshirt)
			RsFileClose(optionsFilePath, optionsFile);
		else
			fclose(optionsFile);

		return false;
	}

	puts("success");

	LoadID(optionsFile);
	if (!LoadBTree(&options, optionsFile))
	{
		DeleteBTreeData(&options);
		return false;
	}
	LoadID_END(optionsFile);

	size_t themeNameLength;
	char themeName[128];
	if (fgetc(optionsFile) == 't' && fread(&themeNameLength, 4, 1, optionsFile) == 1)
	{
		if (themeNameLength + 1 < OPTIONS_THEMENAME_MAX)
			if (fread(themeName, themeNameLength, 1, optionsFile) == 1)
				UplinkStrncpy(this->themeName, themeName, OPTIONS_THEMENAME_MAX);
	}

	if (fileIsRedshirt)
		RsFileClose(optionsFilePath, optionsFile);

	return true;

	fclose(optionsFile);
	return true;
}

void Options::Save(FILE* file)
{
	char optionsFilePath[APP_PATH_MAX];

	MakeDirectory(gApp->UsersPath);
	UplinkSnprintf(optionsFilePath, 0x100, "%soptions", gApp->UsersPath);
	printf("Saving uplink options to %s...", optionsFilePath, "%soptions", gApp->UsersPath);

	const auto optionsFile = fopen(optionsFilePath, "wb");

	if (!optionsFile)
	{
		puts("failed");
		return;
	}

	puts("success");
	fwrite(optionsFileVersionCurrent, optionsFileVersionLength, 1, optionsFile);

	SaveID(optionsFile);
	SaveBTree(&options, optionsFile);
	SaveID_END(optionsFile);

	fputc('t', optionsFile);
	const size_t themeNameLength = strlen(themeName);
	fwrite(&themeNameLength, sizeof(themeNameLength), 1, optionsFile);
	fwrite(themeName, themeNameLength, 1, optionsFile);
	fclose(optionsFile);
	RsEncryptFile(optionsFilePath);
}

void Options::Print()
{
	Options_Print(this);
}

void Options::Update()
{

}


const char* Options::GetID()
{
	return "OPTIONS";
}

void Options::CreateDefaultOptions()
{
	Options_CreateDefaultOptions(this);
}

Option* Options::GetOption(const char* name)
{
	const auto option = options.LookupTree(name);

	if (!option)
		return nullptr;

	return option->Data;
}

int Options::GetOptionValue(const char* name)
{
	const auto option = GetOption(name);
	if (!option)
	{
		char abortMessage[0x100];
		UplinkSnprintf(abortMessage, 0x100, "Option %s not found", name);
		UplinkAbort(abortMessage);
	}
	return option->Value;
}


bool Options::IsOptionEqualTo(const char* name, int value)
{
	bool ret;

	const auto option = GetOption(name);

	if (!option)
		return false;

	return option->Value == value;
}

void Options::SetOptionValue(const char* name, int value)
{
	const auto option = options.LookupTree(name);
	if (!option)
	{
		printf("Tried to set unrecognised option: %s\n");
		return;
	}

	UplinkAssert(option->Data);
	option->Data->SetValue(value);
}

const char* Options::GetThemeName()
{
	return themeName;
}

void Options::SetThemeName(const char* themeName)
{
	Options_SetThemeName(this, themeName);
}