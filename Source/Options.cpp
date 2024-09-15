#include <Globals.hpp>
#include <Options.hpp>
#include <RedShirt.hpp>
#include <cmath>
#include <fstream>
#include <sstream>

static ColourOption getColourDefault = {.Red = 0, .Green = 0, .Blue = 0};

Option::Option() : _name{0}, _tooltip{0}, _yesOrNo(false), _visible(true), _value(0) {}

bool Option::Load(FILE* file)
{
	if (FileReadData(_name, OPTION_NAME_MAX, 1, file) == 0)
	{
		_name[0] = 0;
		return false;
	}
	_name[OPTION_NAME_MAX - 1] = 0;

	if (FileReadData(_tooltip, OPTION_TOOLTIP_MAX, 1, file) == 0)
	{
		_tooltip[0] = 0;
		return false;
	}
	_tooltip[OPTION_TOOLTIP_MAX - 1] = 0;

	if (FileReadData(&_yesOrNo, 1, 1, file) == 0)
		return false;

	if (FileReadData(&_visible, 1, 1, file) == 0)
		return false;

	if (FileReadData(&_value, 4, 1, file) == 0)
		return false;

	return true;
}

void Option::Save(FILE* file)
{
	fwrite(_name, OPTION_NAME_MAX, 1, file);
	fwrite(_tooltip, OPTION_TOOLTIP_MAX, 1, file);
	fwrite(&_yesOrNo, 1, 1, file);
	fwrite(&_visible, 1, 1, file);
	fwrite(&_value, 4, 1, file);
}

void Option::Print()
{
	printf("Option : name=%s, value=%d\n"
		   "\tYesOrNo=%d, Visible=%d\n",
		   _name, _value, _yesOrNo, _visible);
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
	strncpy(_themeName, "graphics", OPTIONS_THEMENAME_MAX);
}

Options::~Options()
{
	DeleteBTreeData(reinterpret_cast<BTree<UplinkObject*>*>(&_options));
	const auto array = _colourOptions.ConvertToDArray();

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
	char themeName[sizeof(this->_themeName) + 4];
	char saveVersion[0x20];

	UplinkSnprintf(optionsFilePath, sizeof(optionsFilePath), "%soptions", app->UsersPath);

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
		strcmp(saveVersion, latestSaveVersion) > 0)
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

	if (!LoadBTree(reinterpret_cast<BTree<UplinkObject*>*>(&_options), optionsFile))
	{
		DeleteBTreeData(reinterpret_cast<BTree<UplinkObject*>*>(&_options));
		return false;
	}

	// The size of themeNameLength in the file depends on the native integer size
	// This code will work with either both 32-bit and 64-bit integers in this field
	uint32_t themeNameLength = 0;
	if ((fgetc(optionsFile) == 't' && fread(&themeNameLength, 4, 1, optionsFile) == 1) && themeNameLength + 1 < sizeof(themeName))
	{
		if (fread(themeName, themeNameLength, 1, optionsFile) == 1)
		{
			auto fixedThemeName = themeName;

			// If the first byte of the theme name is, assume it is one of the four high bytes of the 64-bit length
			if (themeNameLength > 0 && themeName[0] == 0)
			{
				// Read the remaining 4 theme name bytes
				fread(fixedThemeName + themeNameLength, 4, 1, optionsFile);

				// Adjust the theme name beginning
				fixedThemeName += 4;
			}

			fixedThemeName[themeNameLength] = 0;
			UplinkStrncpy(this->_themeName, fixedThemeName, sizeof(this->_themeName));
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
	MakeDirectory(app->UsersPath);

	char optionsFilePath[0x100];
	UplinkSnprintf(optionsFilePath, sizeof(optionsFilePath), "%soptions", app->UsersPath);

	printf("Saving uplink options to %s...", optionsFilePath);

	const auto fp = fopen(optionsFilePath, "wb");

	if (fp == nullptr)
	{
		puts("failed");
		return;
	}

	puts("success");

	fwrite(latestSaveVersion, strlen(latestSaveVersion) + 1, 1, fp);

	SaveBTree(reinterpret_cast<BTree<UplinkObject*>*>(&_options), fp);
	fputc('t', fp);
	const auto themeNameLength = strlen(_themeName);

	// The original game would save this with the native
	fwrite(&themeNameLength, 4, 1, fp);
	fwrite(_themeName, themeNameLength, 1, fp);

	fclose(fp);
	RsEncryptFile(optionsFilePath);
}

void Options::Print()
{
	puts("============== O P T I O N S ===============================");
	PrintBTree(reinterpret_cast<BTree<UplinkObject*>*>(&_options));
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
		const auto change = _optionChanges.GetData(0);
		if (change == nullptr)
			break;

		_optionChanges.RemoveData(0);
		SetOptionValue(change->Name, change->Value);
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
		auto rax_25 = existingGames->Size();
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
	sscanf(versionNumberString, "%f", &versionNumber);
	versionNumber = 100.0f * versionNumber;

	if (GetOption("game_version") == nullptr)
		Options::SetOptionValue("game_version", versionNumber, "z", false, false);

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

	if (const auto softwareRenderingOption = GetOption("graphics_softwarerendering"))
	{
		softwareRenderingOption->SetVisible(false);
	}
	else
		SetOptionValue("graphics_softwarerendering", 0, "Enable software rendering.", true, false);

	if (GetOption("sound_musicenabled") == nullptr)
		SetOptionValue("sound_musicenabled", 1, "Enables or disables music", true, true);

	// TODO: look at THIS??
	app->GetOptions().GetOption("graphics_screenwidth")->SetVisible(false);
	app->GetOptions().GetOption("graphics_screenheight")->SetVisible(false);
	app->GetOptions().GetOption("graphics_screendepth")->SetVisible(false);
	app->GetOptions().GetOption("graphics_screenrefresh")->SetVisible(false);
}

LList<Option*>* Options::GetAllOptions(const char* search, bool getInvisible)
{
	LList<Option*>* list = new LList<Option*>();

	DArray<Option*>* array = _options.ConvertToDArray();
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
	const auto ret = _colourOptions.GetData(name);

	if (ret == nullptr)
	{
		printf("Options::GetColour WARNING : Couldn't find colour %s\n", name);
		return &getColourDefault;
	}

	return ret;
}

Option* Options::GetOption(const char* name)
{
	return _options.GetData(name);
}

int Options::GetOptionValue(const char* name)
{
	const auto option = GetOption(name);
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
	return _themeDescription;
}

const char* Options::GetThemeName()
{
	return _themeName;
}

const char* Options::GetThemeTitle()
{
	return _themeTitle;
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
	UplinkStrncpy(change->Name, name, OPTIONCHANGE_NAME_MAX);
	change->Value = value;
	_optionChanges.PutData(change);
}

void Options::SetOptionValue(char const* name, int value)
{
	auto tree = _options.LookupTree(name);

	if (tree == nullptr)
		printf("Tried to set unrecognised option: %s\n", name);

	UplinkAssert(tree->Data != nullptr);
	tree->Data->SetValue(value);
}

void Options::SetOptionValue(const char* name, int value, const char* tooltip, bool yesOrNo, bool visible)
{
	const auto tree = _options.LookupTree(name);

	if (tree == nullptr)
	{
		auto option = new Option();
		option->SetName(name);
		option->SetValue(value);
		option->SetTooltip(tooltip);
		option->SetYesOrNo(yesOrNo);
		option->SetVisible(visible);
		_options.PutData(name, option);
	}
	else
	{
		UplinkAssert(tree->Data != nullptr);
		const auto option = tree->Data;
		option->SetValue(value);
		option->SetTooltip(tooltip);
		option->SetYesOrNo(yesOrNo);
		option->SetVisible(visible);
	}
}

void Options::SetThemeName(const char* value)
{
	(void)value;
	UplinkStrncpy(_themeName, value, OPTIONS_THEMENAME_MAX);

	const auto filePath = ThemeFilename("theme.txt");
	const auto themeFile = RsArchiveFileOpen(filePath);

	if (themeFile == nullptr)
	{
		RsArchiveFileClose(filePath, nullptr);
		delete[] filePath;
		return;
	}

	std::ifstream themeFileStream;

	themeFileStream.open(themeFile);

	char temp[0x68];

	themeFileStream >> temp >> std::ws;
	themeFileStream.getline(_themeTitle, OPTIONS_THEMETITLE_MAX, '\r');
	if ((temp[0] = themeFileStream.get()) != '\n')
		themeFileStream.rdbuf()->sputbackc(temp[0]);

	themeFileStream >> temp >> std::ws;
	themeFileStream.getline(_themeAuthor, OPTIONS_THEMEAUTHOR_MAX, '\r');
	if ((temp[0] = themeFileStream.get()) != '\n')
		themeFileStream.rdbuf()->sputbackc(temp[0]);

	themeFileStream >> temp >> std::ws;
	themeFileStream.getline(_themeDescription, OPTIONS_THEMEDESCRIPTION_MAX, '\r');
	if ((temp[0] = themeFileStream.get()) != '\n')
		themeFileStream.rdbuf()->sputbackc(temp[0]);

	while (!themeFileStream.eof())
	{
		char buffer[0x100];
		themeFileStream.getline(buffer, sizeof(buffer), '\r');
		if ((temp[0] = themeFileStream.get()) != '\n')
			themeFileStream.rdbuf()->sputbackc(temp[0]);

		if (strlen(buffer) == 0)
			continue;

		auto stream = std::istringstream(buffer);
		stream >> std::ws;

		if ((temp[0] = stream.get()) == ';')
			continue;
		stream.rdbuf()->sputbackc(temp[0]);

		char colourName[0x40];
		const auto colourOption = new ColourOption();
		stream >> colourName >> std::ws >> colourOption->Red >> colourOption->Green >> colourOption->Blue;

		const auto option = _colourOptions.LookupTree(colourName);
		if (option != nullptr)
		{
			delete option->Data;
			option->Data = colourOption;
		}
		else
			_colourOptions.PutData(colourName, colourOption);
	}

	themeFileStream.close();

	RsArchiveFileClose(filePath, nullptr);
	delete[] filePath;
}

char* Options::ThemeFilename(const char* name)
{
	char* filename = new char[0x100];

	if (strcmp(_themeName, "graphics") == 0)
	{
		UplinkSnprintf(filename, 0x100, "graphics/%s", name);
		return filename;
	}

	char tempPath[0x100];
	UplinkSnprintf(tempPath, sizeof(tempPath), "%s%s/%s", app->Path, _themeName, name);

	if (!DoesFileExist(tempPath))
	{
		UplinkSnprintf(filename, 0x100, "graphics/%s", name);
		return filename;
	}

	UplinkSnprintf(filename, 0x100, "%s/%s", _themeName, name);
	return filename;
}
