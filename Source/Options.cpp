#include <Globals.hpp>
#include <Options.hpp>
#include <RedShirt.hpp>
#include <cmath>

static const char* minSaveVersion = "SAV56";

static ColourOption getColourDefault = {.red = 0, .green = 0, .blue = 0};

Option::Option() : name{0}, tooltip{0}, yesOrNo(false), visible(true), value(0) {}

bool Option::Load(FILE* file)
{
	(void)file;
	UplinkAbort("TODO: implement Option::Load(FILE*)");
}

void Option::Save(FILE* file)
{
	fwrite(name, NAME_MAX, 1, file);
	fwrite(tooltip, TOOLTIP_MAX, 1, file);
	fwrite(&yesOrNo, 1, 1, file);
	fwrite(&visible, 1, 1, file);
	fwrite(&value, 4, 1, file);
	SaveID_END(file);
}

void Option::Print()
{
	printf("Option : name=%s, value=%d\n"
		   "\tYesOrNo=%d, Visible=%d\n",
		   name, value, yesOrNo, visible);
}

const char* Option::GetID()
{
	return "OPTION";
}

UplinkObjectId Option::GetOBJECTID()
{
	return UplinkObjectId::Option;
}

Options::Options()
{
	strncpy(themeName, "graphics", THEMENAME_MAX);
}

Options::~Options()
{
	DeleteBTreeData(&options);
	auto array = colourOptions.ConvertToDArray();

	for (int i = 0; i < array->Size(); i++)
	{
		if ((array->ValidIndex(i) != 0 && array->GetData(i) != 0))
			delete array->GetData(i);
	}

	delete array;
}

bool Options::Load(FILE* file)
{
	(void)file;
	FILE* optionsFile;
	char optionsFilePath[0x100];
	char themeName[sizeof(this->themeName) + 4];
	char saveVersion[0x20];

	UplinkSnprintf(optionsFilePath, sizeof(optionsFilePath), "%soptions", app->usersPath);

	printf("Loading uplink options from %s...", optionsFilePath);

	const auto fileEncrypted = RsFileEncryptedNoVerify(optionsFilePath);

	if (fileEncrypted)
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

	if (!FileReadData(saveVersion, 6, 1, optionsFile) || saveVersion[0] == 0 || strcmp(saveVersion, minSaveVersion) < 0 ||
		strcmp(saveVersion, saveVersion) > 0)
	{
		puts("\nERROR : Could not load options due to incompatible version format");
		if (fileEncrypted)
		{
			RsFileClose(optionsFilePath, optionsFile);
			return false;
		}
		fclose(optionsFile);
		return false;
	}

	puts("success");

	LoadID(optionsFile);
	if (!LoadBTree((BTree<UplinkObject*>*)&options, optionsFile))
	{
		DeleteBTreeData((BTree<UplinkObject*>*)&options);
		return false;
	}
	LoadID_END(optionsFile);

	// The size of themeNameLength in the file depends on the native integer size
	// This code will work with either both 32-bit and 64-bit integers in this field
	uint32_t themeNameLength = 0;
	if ((fgetc(optionsFile) == 't' && fread(&themeNameLength, sizeof(themeNameLength), 1, optionsFile) == 1) &&
		themeNameLength + 1 < sizeof(themeName))
	{
		if (fread(themeName, themeNameLength, 1, optionsFile) == 1)
		{
			auto fixedThemeName = themeName;

			if (themeNameLength > 0 && themeName[0] == 0)
			{
				fread(fixedThemeName + themeNameLength, 4, 1, optionsFile);
				fixedThemeName += 4;
			}

			fixedThemeName[themeNameLength] = 0;
			UplinkStrncpy(this->themeName, fixedThemeName, sizeof(this->themeName));
		}
	}

	if (fileEncrypted)
	{
		RsFileClose(optionsFilePath, optionsFile);
	}
	else
		fclose(optionsFile);

	return true;
}

void Options::Save(FILE* file)
{
	(void)file;
	UplinkAbort("TODO: implement Options::Save(FILE*)");
}

void Options::Print()
{
	puts("============== O P T I O N S ===============================");
	PrintBTree(reinterpret_cast<BTree<UplinkObject*>*>(&options));
	puts("============================================================");
}

const char* Options::GetID()
{
	return "OPTIONS";
}

void Options::ApplyShutdownChanges()
{
	while (true)
	{
		const auto change = optionChanges.GetData(0);
		if (change == nullptr)
			break;

		optionChanges.RemoveData(0);
		SetOptionValue(change->name, change->value);
		delete change;
	}
}

void Options::CreateDefaultOptions()
{
	if (GetOption("game_debugstart") == nullptr)
	{
		SetOptionValue("game_debugstart", 1, "z", true, false);
	}

	if (GetOption("game_firsttime") == nullptr)
	{
		const auto existingGames = App::ListExistingGames();
		int32_t rax_25 = existingGames->Size();
		if (rax_25 > 0)
		{
			SetOptionValue("game_firsttime", 0, "z", true, false);
			for (int index = 0; index < rax_25; index++)
			{
				if (existingGames->ValidIndex(index))
				{
					const auto game = existingGames->GetData(index);
					if (game != nullptr)
					{
						delete game;
					}
				}
			}
		}
		else
			SetOptionValue("game_firsttime", 1, "z", true, false);

		delete existingGames;
	}

	float versionNumber;
	sscanf("1.55", "%f", &versionNumber);
	versionNumber = 100.0f * versionNumber;

	if (GetOption("game_version") == nullptr)
		Options::SetOptionValue("game_version", ((int32_t)(truncf(versionNumber))), "z", false, false);

	if (GetOption("graphics_screenwidth") == nullptr)
		Options::SetOptionValue("graphics_screenwidth", 0x400, "Sets the width of the screen", false, false);

	if (GetOption("graphics_screenheight") == nullptr)
		Options::SetOptionValue("graphics_screenheight", 0x300, "Sets the height of the screen", false, false);

	if (GetOption("graphics_screendepth") == nullptr)
		Options::SetOptionValue("graphics_screendepth", -1, "Sets the colour depth. -1 Means use desktop colour depth.", false, false);

	if (GetOption("graphics_screenrefresh") == nullptr)
		Options::SetOptionValue("graphics_screenrefresh", -1, "Sets the refresh rate. -1 Means use desktop refresh.", false, false);

	if (GetOption("graphics_fullscreen") == nullptr)
		Options::SetOptionValue("graphics_fullscreen", 1, "Sets the game to run fullscreen or in a window", true, true);

	if (GetOption("graphics_buttonanimations") == nullptr)
		Options::SetOptionValue("graphics_buttonanimations", 1, "Enables or disables button animations", true, true);

	if (GetOption("graphics_safemode") == nullptr)
		Options::SetOptionValue("graphics_safemode", 0, "Enables graphical safemode for troubleshooting", true, true);

	if (GetOption("graphics_softwaremouse") == nullptr)
		Options::SetOptionValue("graphics_softwaremouse", 0, "Render a software mouse.  Use to correct mouse problems.", true, true);

	if (GetOption("graphics_fasterbuttonanimations") == nullptr)
		Options::SetOptionValue("graphics_fasterbuttonanimations", 0, "Increase the speed of button animations.", true, true);

	if (GetOption("graphics_defaultworldmap") == nullptr)
		Options::SetOptionValue("graphics_defaultworldmap", 0, "Create agents with the default world map.", true, true);

	struct Option* rax_14 = GetOption("graphics_softwarerendering");
	if (rax_14 == nullptr)
	{
		SetOptionValue("graphics_softwarerendering", 0, "Enable software rendering.", true, false);
	}
	else
	{
		rax_14->SetVisible(false);
	}
	if (GetOption("sound_musicenabled") == nullptr)
	{
		SetOptionValue("sound_musicenabled", 1, "Enables or disables music", true, true);
	}

	// TODO: look at THIS??
	app->GetOptions()->GetOption("graphics_screenwidth")->SetVisible(false);
	app->GetOptions()->GetOption("graphics_screenheight")->SetVisible(false);
	app->GetOptions()->GetOption("graphics_screendepth")->SetVisible(false);
	app->GetOptions()->GetOption("graphics_screenrefresh")->SetVisible(false);
}

LList<Option*>* Options::GetAllOptions(const char* search, bool getInvisible)
{
	LList<Option*>* list = new LList<Option*>();

	DArray<Option*>* array = options.ConvertToDArray();
	for (int i = 0; i < array->Size(); i++)
	{
		if (!array->ValidIndex(i))
			continue;

		const auto option = array->GetData(i);
		UplinkAssert(option);

		if (option->GetVisible() || getInvisible != 0)
		{
			const char* match = nullptr;

			if (search != nullptr)
				match = strstr(option->GetName(), search);

			if (search == nullptr || match != nullptr)
				list->PutData(option);
		}
	}
	delete array;

	return list;
}

ColourOption* Options::GetColour(const char* name)
{
	const auto ret = colourOptions.GetData(name);

	if (ret == nullptr)
	{
		printf("Options::GetColour WARNING : Couldn't find colour %s\n", name);
		return &getColourDefault;
	}

	return ret;
}

Option* Options::GetOption(const char* name)
{
	return options.GetData(name);
}

int Options::GetOptionValue(const char* name)
{
	struct Option* option = GetOption(name);
	if (option == nullptr)
	{
		char s[0x100];
		UplinkSnprintf(s, sizeof(s), "Option %s not found", name);
		UplinkAbort(s);
	}
	return option->GetValue();
}

int Options::GetOptionValueOrDefault(const char* name, int defaultValue)
{
	const auto option = GetOption(name);
	if (option == nullptr)
		return defaultValue;
	return option->GetValue();
}

const char* Options::GetThemeDescription()
{
	return themeDescription;
}

const char* Options::GetThemeName()
{
	return themeName;
}

const char* Options::GetThemeTitle()
{
	return themeTitle;
}

bool Options::IsOptionEqualTo(const char* name, int value)
{
	const auto option = GetOption(name);
	if (option == 0)
		return false;

	return option->GetValue() == value;
}

void Options::RequestShutdownChange(const char* name, int value)
{
	auto change = new OptionChange();
	UplinkStrncpy(change->name, name, OptionChange::NAME_MAX);
	change->value = value;
	optionChanges.PutData(change);
}

void Options::SetOptionValue(char const* name, int value)
{
	auto tree = options.LookupTree(name);

	if (tree == nullptr)
		printf("Tried to set unrecognised option: %s\n", name);

	UplinkAssert(tree->data != nullptr);
	tree->data->SetValue(value);
}

void Options::SetOptionValue(const char* name, int value, const char* tooltip, bool yesOrNo, bool visible)
{
	const auto tree = options.LookupTree(name);

	if (tree == nullptr)
	{
		auto option = new Option();
		option->SetName(name);
		option->SetValue(value);
		option->SetTooltip(tooltip);
		option->SetYesOrNo(yesOrNo);
		option->SetVisible(visible);
		options.PutData(name, option);
	}
	else
	{
		UplinkAssert(tree->data != nullptr);
		const auto option = tree->data;
		option->SetValue(value);
		option->SetTooltip(tooltip);
		option->SetYesOrNo(yesOrNo);
		option->SetVisible(visible);
	}
}

void Options::SetThemeName(const char* value)
{
	(void)value;
	UplinkAbort("TODO: implement Options::SetThemeName()");
}

char* Options::ThemeFilename(const char* name)
{
	char* filename = new char[0x100];

	if (strcmp(themeName, "graphics") == 0)
	{
		UplinkSnprintf(filename, 0x100, "graphics/%s", name);
		return filename;
	}

	char tempPath[0x100];
	UplinkSnprintf(tempPath, sizeof(tempPath), "%s%s/%s", app->path, themeName, name);

	if (!DoesFileExist(tempPath))
	{
		UplinkSnprintf(filename, 0x100, "graphics/%s", name);
		return filename;
	}

	UplinkSnprintf(filename, 0x100, "%s/%s", themeName, name);
	return filename;
}
