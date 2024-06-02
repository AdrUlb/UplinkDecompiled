#include <Record.hpp>

Record::~Record()
{
	DeleteBTreeData(&_fields);
}

bool Record::Load(FILE* file)
{
	return LoadBTree(reinterpret_cast<BTree<UplinkObject*>*>(&_fields), file);
}

void Record::Save(FILE* file)
{
	SaveBTree(reinterpret_cast<BTree<UplinkObject*>*>(&_fields), file);
}

void Record::Print()
{
	puts("Record :");

	const auto names = _fields.ConvertIndexToDArray();
	const auto values = _fields.ConvertToDArray();

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
	_fields.PutData(name, buf);
}

RecordBank::~RecordBank()
{
	DeleteLListData(reinterpret_cast<LList<UplinkObject*>*>(&this->_records));
}

bool RecordBank::Load(FILE* file)
{
	return LoadLList(reinterpret_cast<LList<UplinkObject*>*>(&this->_records), file);
}

void RecordBank::Save(FILE* file)
{
	SaveLList(reinterpret_cast<LList<UplinkObject*>*>(&this->_records), file);
}

void RecordBank::Print()
{
	puts("RecordBank");
	PrintLList(reinterpret_cast<LList<UplinkObject*>*>(&this->_records));
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
	_records.PutData(&record);
}

char* RecordBank::MakeSafeField(const char* name)
{
	const auto len = strlen(name);
	char* str = new char[len + 1];
	strcpy(str, name);

	for (size_t i = 0; i < len; i++)
	{
		if (str[i] == ';')
			str[i] = '.';
	}

	return str;
}

/*Record* RecordBank::GetRecordFromName(const char* name)
{
	char* rax = MakeSafeField(name);
	char s[0x100];
	UplinkSnprintf(s, 0x100, "%s = %s", "Name", rax);
	if (rax != nullptr)
		delete[] rax;

	return GetRecord(s);
}*/
