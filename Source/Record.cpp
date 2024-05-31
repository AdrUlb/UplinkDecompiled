#include <Record.hpp>

Record::~Record()
{
	DeleteBTreeData(&fields);
}

bool Record::Load(FILE* file)
{
	return LoadBTree(reinterpret_cast<BTree<UplinkObject*>*>(&fields), file);
}

void Record::Save(FILE* file)
{
	SaveBTree(reinterpret_cast<BTree<UplinkObject*>*>(&fields), file);
}

void Record::Print()
{
	puts("Record :");

	const auto names = fields.ConvertIndexToDArray();
	const auto values = fields.ConvertToDArray();

	for (auto i = 0; i < names->Size(); i++)
	{
		UplinkAssert(names->ValidIndex(i));
		UplinkAssert(values->ValidIndex(i));

		const auto name = names->GetData(i);
		const auto value = values->GetData(i);

		printf("%s : %s\n", name, value);
	}

	delete names;
	delete values;
}

const char* Record::GetID()
{
	return "RECORD";
}

UplinkObjectId Record::GetOBJECTID()
{
	return UplinkObjectId::Record;
}

void Record::AddField(const char* name, const char* value)
{
	char* buf = new char[strlen(value) + 1];
	strcpy(buf, value);
	fields.PutData(name, buf);
}

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

void RecordBank::AddRecord(Record& record)
{
	records.PutData(&record);
}
