#include <Options.hpp>
#include <Util.hpp>

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

void Option::SetName(const char* value)
{
    UplinkStrncpy(name, value, NAME_MAX);
}

void Option::SetTooltip(const char* value)
{
    UplinkStrncpy(tooltip, value, TOOLTIP_MAX);
}

void Option::SetValue(int value)
{
    this->value = value;
}

void Option::SetVisible(bool visible)
{
    this->visible = visible;
}

void Option::SetYesOrNo(bool yesOrNo)
{
    this->yesOrNo = yesOrNo;
}

Options::Options()
{
	strncpy(themeName, "graphics", THEMENAME_MAX);
}

Options::~Options()
{
	DeleteBTreeData(options);
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
	PrintBTree(options);
	puts("============================================================");
}

const char* Options::GetID()
{
	return "OPTIONS";
}

void Options::ApplyShutdownChanges()
{
	puts("TODO: implement Options::ApplyShutdownChanges()");
	abort();
}

void Options::CreateDefaultOptions()
{
	puts("TODO: implement Options::CreateDefaultOptions()");
	abort();
}