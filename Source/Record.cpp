#include <Record.hpp>

RecordBank::~RecordBank()
{
	DeleteLListData(reinterpret_cast<LList<UplinkObject*>*>(&this->records));
}

bool RecordBank::Load(FILE* file)
{
	return LoadLList(reinterpret_cast<LList<UplinkObject*>*>(&this->records), file);
}

void RecordBank::Save(FILE* file)
{
	SaveLList(reinterpret_cast<LList<UplinkObject*>*>(&this->records), file);
	SaveID_END(file);
}

void RecordBank::Print()
{
	puts("RecordBank");
	PrintLList(reinterpret_cast<LList<UplinkObject*>*>(&this->records));
}

const char* RecordBank::GetID()
{
	return "RECBANK";
}

UplinkObjectId RecordBank::GetOBJECTID()
{
	return UplinkObjectId::RecordBank;
}
