#include <Data.hpp>

#include <Util.hpp>

bool Data::Load(FILE* file)
{
	if (!LoadDynamicStringBuf(title, 0x40, file))
		return false;

	if (!FileReadData(&type, 4, 1, file))
		return false;

	if (!FileReadData(&size, 4, 1, file))
		return false;

	if (!FileReadData(&encrypted, 4, 1, file))
		return false;

	if (!FileReadData(&compressed, 4, 1, file))
		return false;

	if (!FileReadData(&softwareVersion, 4, 1, file))
		return false;

	if (!FileReadData(&softwareType, 4, 1, file))
		return false;

	return true;
}

void Data::Save(FILE* file)
{
	SaveDynamicString(title, 0x40, file);
	fwrite(&type, 4, 1, file);
	fwrite(&size, 4, 1, file);
	fwrite(&encrypted, 4, 1, file);
	fwrite(&compressed, 4, 1, file);
	fwrite(&softwareVersion, 4, 1, file);
	fwrite(&softwareType, 4, 1, file);
}

void Data::Print()
{
	printf("Data: %s\n", title);
	printf("type = %d, size = %d, encrypted = %d, compressed = %d\nversion = %f, softwareTYPE = %d\n", type, size, encrypted, compressed,
		   softwareVersion, softwareType);
}

const char* Data::GetID()
{
	return "DATA";
}

UplinkObjectId Data::GetOBJECTID()
{
	return UplinkObjectId::Data;
}

void Data::SetTitle(const char* value)
{
	UplinkStrncpy(title, value, 0x40);
}

void Data::SetDetails(int type, int size, int encrypted, int compressed, float softwareVersion, int softwareType)
{
	this->type = type;
	this->size = size;
	this->softwareVersion = softwareVersion;
	this->encrypted = encrypted;
	this->compressed = compressed;
	this->softwareType = softwareType;
}

DataBank::~DataBank()
{
	DeleteDArrayDataD(&files);
}

bool DataBank::Load(FILE* file)
{
	(void)file;
	(void)puts("TODO: implement DataBank::Load()");
	return false;
}

void DataBank::Save(FILE* file)
{
	(void)file;
	puts("TODO: implement DataBank::Save()");
}

void DataBank::Print()
{
	puts("TODO: implement DataBank::Print()");
}

const char* DataBank::GetID()
{
	return "DATABANK";
}

UplinkObjectId DataBank::GetOBJECTID()
{
	return UplinkObjectId::DataBank;
}

void DataBank::SetSize(int value)
{
	placements.SetSize(value);
}

void DataBank::PutData(Data* value, int placement)
{
	UplinkAssert(value != 0);

	const auto index = files.PutData(value);

	for (auto i = 0; i < value->size; i++)
		placements.PutData(index, placement + i);
}

bool DataBank::PutData(Data* value)
{
	UplinkAssert(value != nullptr);

	const auto placement = FindValidPlacement(value);
	if (placement == -1)
		return false;

	PutData(value, placement);
	return true;
}

int DataBank::FindValidPlacement(Data* value)
{
	UplinkAssert(value != nullptr);

	for (auto i = 0; i < placements.Size(); i++)
		if (IsValidPlacement(value, i) == 0)
			return i;

	return -1;
}

int DataBank::IsValidPlacement(Data* value, int placement)
{
	UplinkAssert(value != nullptr);

	for (auto i = placement; i < placement + value->size; i++)
	{
		// Outside of the range of valid placements
		if (i < 0 || i >= placements.Size())
			return 2;

		// This placement is already in use
		if (placements.ValidIndex(i))
			return 1;
	}

	return 0;
}
