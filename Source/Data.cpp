#include <Data.hpp>

#include <Util.hpp>

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
