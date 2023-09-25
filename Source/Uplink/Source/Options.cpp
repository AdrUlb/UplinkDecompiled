#include <Options.hpp>

#include <cstring>
#include <cassert>
#include <Util.hpp>
#include <App.hpp>
#include <RedShirt.hpp>
#include "../UplinkDecompiledTempGlobals.hpp"
#include "../UplinkDecompiledTempDefs.hpp"

constexpr auto optionsFileVersionCurrent = "SAV62";
constexpr auto optionsFileVersionMin = "SAV56";
constexpr size_t optionsFileVersionLength = 6;

Option::Option()
{
	memset(name, 0, OPTION_NAME_MAX);
	name[OPTION_NAME_MAX - 1] = 0;
	strncpy(tooltip, "", OPTION_TOOLTIP_MAX);
	tooltip[OPTION_TOOLTIP_MAX - 1] = 0;
	yesNo = false;
	visible = true;
	Value = 0;
}

bool Option::Load(FILE* file)
{
	LoadID(file);
	if (FileReadDataInt(name, OPTION_NAME_MAX, 1, file) == 0)
	{
		name[0] = 0;
		return false;
	}
	name[OPTION_NAME_MAX - 1] = 0;

	if (FileReadDataInt(tooltip, OPTION_TOOLTIP_MAX, 1, file) == 0)
	{
		tooltip[0] = 0;
		return false;
	}
	tooltip[OPTION_TOOLTIP_MAX - 1] = 0;

	if (FileReadDataInt(&yesNo, 1, 1, file) == 0)
		return false;

	if (FileReadDataInt(&visible, 1, 1, file) == 0)
		return false;

	if (FileReadDataInt(&Value, 4, 1, file) == 0)
		return false;

	LoadID_END(file);
	return true;
}

void Option::Save(FILE* file)
{
	SaveID(file);
	fwrite(name, OPTION_NAME_MAX, 1, file);
	fwrite(tooltip, OPTION_TOOLTIP_MAX, 1, file);
	fwrite(&yesNo, sizeof(yesNo), 1, file);
	fwrite(&visible, sizeof(visible), 1, file);
	fwrite(&Value, sizeof(Value), 1, file);
	SaveID_END(file);
}

void Option::Print()
{
	printf("Option : name=%s, value=%d\n", name, Value);
	printf("\tYesOrNo=%d, Visible=%d\n", yesNo, visible);
}

void Option::Update()
{

}

const char* Option::GetID()
{
	return "OPTION";
}

int Option::GetOBJECTID()
{
	return 8;
}

void Option::SetName(const char* name)
{
	UplinkAssert(strlen(name) < OPTION_NAME_MAX);
	strncpy(this->name, name, OPTION_NAME_MAX);
}

void Option::SetTooltip(const char* tooltip)
{
	UplinkAssert(strlen(tooltip) < OPTION_TOOLTIP_MAX);
	strncpy(this->tooltip, tooltip, OPTION_TOOLTIP_MAX);
}

void Option::SetValue(int value)
{
	Value = value;
}

void Option::SetVisible(bool visible)
{
	this->visible = visible;
}

void Option::SetYesOrNo(bool value)
{
	yesNo = value;
}

Option::~Option()
{

}

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
	(void)file;

	char optionsFilePath[APP_PATH_MAX];
	UplinkSnprintf(optionsFilePath, APP_PATH_MAX, "%soptions", gApp->UsersPath);

	printf("Loading uplink options from %s...", optionsFilePath);

	FILE* optionsFile;
	bool optionsFileIsRedshirt;
	if (RsFileEncryptedNoVerify(optionsFilePath))
	{
		if (!RsFileEncrypted(optionsFilePath))
		{
			puts("failed");
			return false;
		}

		optionsFile = RsFileOpen(optionsFilePath, "rb");
		optionsFileIsRedshirt = true;
	}
	else
	{
		optionsFile = fopen(optionsFilePath, "rb");
		optionsFileIsRedshirt = false;
	}

	if (!optionsFile)
	{
		puts("failed");
		return false;
	}

	char optionsFileVersion[optionsFileVersionLength];
	if (!FileReadDataInt(optionsFileVersion, optionsFileVersionLength, 1, optionsFile) ||
		optionsFileVersion[0] == 0 ||
		strncmp(optionsFileVersion, optionsFileVersionMin, optionsFileVersionLength) < 0 ||
		strncmp(optionsFileVersion, optionsFileVersionCurrent, optionsFileVersionLength) > 0)
	{
		puts("\nERROR : Could not load options due to incompatible version format");

		if (optionsFileIsRedshirt)
			RsFileClose(optionsFilePath, optionsFile);
		else
			fclose(optionsFile);

		puts("failed");
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
	if (fgetc(optionsFile) == 't' && fread(&themeNameLength, 4, 1, optionsFile) == 1 && themeNameLength + 1 < OPTIONS_THEMENAME_MAX)
	{
		char themeName[OPTIONS_THEMENAME_MAX];
		if (fread(themeName, themeNameLength, 1, optionsFile) == 1)
		{
			themeName[themeNameLength] = 0;
			UplinkStrncpy(themeName, themeName, OPTIONS_THEMENAME_MAX);
		}
	}

	if (optionsFileIsRedshirt)
		RsFileClose(optionsFilePath, optionsFile);
	else
		fclose(optionsFile);
		
	return true;
}

void Options::Save(FILE* file)
{
	(void)file;
	char optionsFilePath[APP_PATH_MAX];

	MakeDirectory(gApp->UsersPath);
	UplinkSnprintf(optionsFilePath, APP_PATH_MAX, "%soptions", gApp->UsersPath);
	printf("Saving uplink options to %s...", optionsFilePath);

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
		printf("Tried to set unrecognised option: %s\n", name);
		return;
	}

	UplinkAssert(option->Data);
	option->Data->SetValue(value);
}

void Options::SetOptionValue(const char* name, int value, const char* tooltip, bool yesNo, bool visible)
{
	const auto optionNode = options.LookupTree(name);
	if (!optionNode)
	{
		auto option = new Option();
		option->SetName(name);
		option->SetValue(value);
		option->SetTooltip(tooltip);
		option->SetYesOrNo(yesNo);
		option->SetVisible(visible);
		options.PutData(name, &option);
		return;
	}

	UplinkAssert(optionNode->Data);
	auto option = optionNode->Data;
	option->SetValue(value);
	option->SetTooltip(tooltip);
	option->SetYesOrNo(yesNo);
	option->SetVisible(visible);
}

const char* Options::GetThemeName()
{
	return themeName;
}

void Options::SetThemeName(const char* themeName)
{
	Options_SetThemeName(this, themeName);
}
