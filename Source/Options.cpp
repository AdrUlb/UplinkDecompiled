#include <Globals.hpp>
#include <Options.hpp>

static ColourOption getColourDefault = {.red = 0, .green = 0, .blue = 0};

Option::Option() : name{0}, tooltip{0}, yesOrNo(false), visible(true), value(0) {}

bool Option::Load(FILE* file)
{
	(void)file;
	puts("TODO: implement Option::Load(FILE*)");
	abort();
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
	puts("TODO: implement Options::Load(FILE*)");
	abort();
}

void Options::Save(FILE* file)
{
	(void)file;
	puts("TODO: implement Options::Save(FILE*)");
	abort();
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
	puts("TODO: implement Options::CreateDefaultOptions()");
	abort();
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
	struct Option* option = GetOption(name);
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
	puts("TODO: implement Options::SetThemeName()");
	abort();
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
