#include <Data.hpp>

#include <Util.hpp>

bool Data::Load(FILE* file)
{
	if (!LoadDynamicStringBuf(_title, 0x40, file))
		return false;

	if (!FileReadData(&_type, 4, 1, file))
		return false;

	if (!FileReadData(&_size, 4, 1, file))
		return false;

	if (!FileReadData(&_encrypted, 4, 1, file))
		return false;

	if (!FileReadData(&_compressed, 4, 1, file))
		return false;

	if (!FileReadData(&_softwareVersion, 4, 1, file))
		return false;

	if (!FileReadData(&_softwareType, 4, 1, file))
		return false;

	return true;
}

void Data::Save(FILE* file)
{
	SaveDynamicString(_title, 0x40, file);
	fwrite(&_type, 4, 1, file);
	fwrite(&_size, 4, 1, file);
	fwrite(&_encrypted, 4, 1, file);
	fwrite(&_compressed, 4, 1, file);
	fwrite(&_softwareVersion, 4, 1, file);
	fwrite(&_softwareType, 4, 1, file);
}

void Data::Print()
{
	printf("Data: %s\n", _title);
	printf("type = %d, size = %d, encrypted = %d, compressed = %d\nversion = %f, softwareTYPE = %d\n", _type, _size, _encrypted, _compressed,
		   _softwareVersion, _softwareType);
}

const char* Data::GetID()
{
	return "DATA";
}

UplinkObjectId Data::GetOBJECTID()
{
	return UplinkObjectId::Data;
}

int Data::GetSize()
{
	return _size;
}

void Data::SetTitle(const char* title)
{
	UplinkStrncpy(_title, title, 0x40);
}

void Data::SetDetails(int type, int size, int encrypted, int compressed, float softwareVersion, int softwareType)
{
	this->_type = type;
	this->_size = size;
	this->_softwareVersion = softwareVersion;
	this->_encrypted = encrypted;
	this->_compressed = compressed;
	this->_softwareType = softwareType;
}

DataBank::~DataBank()
{
	DeleteDArrayDataD(&_files);
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
	_placements.SetSize(value);
}

void DataBank::PutData(Data* value, int placement)
{
	UplinkAssert(value != 0);

	const auto index = _files.PutData(value);

	for (auto i = 0; i < value->GetSize(); i++)
		_placements.PutData(index, placement + i);
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

	for (auto i = 0; i < _placements.Size(); i++)
		if (IsValidPlacement(value, i) == 0)
			return i;

	return -1;
}

int DataBank::IsValidPlacement(Data* value, int placement)
{
	UplinkAssert(value != nullptr);

	for (auto i = placement; i < placement + value->GetSize(); i++)
	{
		// Outside of the range of valid placements
		if (i < 0 || i >= _placements.Size())
			return 2;

		// This placement is already in use
		if (_placements.ValidIndex(i))
			return 1;
	}

	return 0;
}
