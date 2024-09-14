#include <AccessLog.hpp>

#include <Util.hpp>

AccessLog::~AccessLog()
{
	delete[] _data1;
	delete[] _data2;
	delete[] _data3;
}

bool AccessLog::Load(FILE* file)
{
	if (!_date.Load(file))
		return false;

	if (!LoadDynamicStringBuf(_fromIp, 0x18, file))
		return false;

	if (!LoadDynamicStringBuf(_fromName, 0x80, file))
		return false;

	if (!FileReadData(&_type, 4, 1, file))
		return false;

	if (!FileReadData(&_suspicious, 4, 1, file))
		return false;

	if (!LoadDynamicString(_data1, file))
		return false;

	if (!LoadDynamicString(_data2, file))
		return false;

	if (!LoadDynamicString(_data3, file))
		return false;

	return true;
}

void AccessLog::Save(FILE* file)
{
	_date.Save(file);
	SaveDynamicString(_fromIp, 0x18, file);
	SaveDynamicString(_fromName, 0x80, file);
	fwrite(&_type, 4, 1, file);
	fwrite(&_suspicious, 4, 1, file);
	SaveDynamicString(_data1, file);
	SaveDynamicString(_data2, file);
	SaveDynamicString(_data3, file);
}

void AccessLog::Print()
{
	printf("AccessLog : TYPE=%d\n", _type);

	_date.Print();

	printf("fromIP:%s, fromNAME:%s\n", _fromIp, _fromName);

	switch (_suspicious)
	{
		case 1:
			puts("This was a suspicious action");
			break;
		case 2:
			puts("This was suspicious and was noticed");
			break;
		case 3:
			puts("This is under investigation");
			break;
		default:
			puts("This was not suspicious");
			break;
	}

	if (_data1 != nullptr)
		printf("Data1 : %s\n", _data1);

	if (_data2 != nullptr)
		printf("Data2 : %s\n", _data2);

	if (_data3 != nullptr)
		printf("Data3 : %s\n", _data3);
}

const char* AccessLog::GetID()
{
	return "ACC_LOG";
}

UplinkObjectId AccessLog::GetOBJECTID()
{
	return UplinkObjectId::AccessLog;
}

int AccessLog::GetTYPE()
{
	return _type;
}

Date& AccessLog::GetDate()
{
	return _date;
}

const char* AccessLog::GetFromIp()
{
	return _fromIp;
}

const char* AccessLog::GetFromName()
{
	return _fromName;
}

int AccessLog::GetSuspicious()
{
	return _suspicious;
}

void AccessLog::SetTYPE(int value)
{
	_type = value;
}

void AccessLog::SetSuspicious(int value)
{
	_suspicious = value;
}

void AccessLog::SetFromIP(const char* value)
{
	UplinkStrncpy(_fromIp, value, 0x18);
}

void AccessLog::SetData1(const char* value)
{
	delete[] _data1;
	_data1 = nullptr;

	if (value == nullptr)
		return;

	_data1 = new char[strlen(value) + 1];
	strcpy(_data1, value);
}

void AccessLog::SetData2(const char* value)
{
	if (_data2 != nullptr)
	{
		delete[] _data2;
		_data2 = nullptr;
	}

	if (value == nullptr)
		return;

	_data2 = new char[strlen(value) + 1];
	strcpy(_data2, value);
}

void AccessLog::SetData3(const char* value)
{
	delete[] _data3;
	_data3 = nullptr;

	if (value == nullptr)
		return;

	_data3 = new char[strlen(value) + 1];
	strcpy(_data3, value);
}

void AccessLog::SetProperties(Date& date, const char* ip, const char* fromName, int suspicious, int type)
{
	this->_date.SetDate(&date);
	SetTYPE(type);
	SetFromIP(ip);
	UplinkStrncpy(this->_fromName, fromName, 0x80);
	SetSuspicious(suspicious);
}

void AccessLog::SetProperties(AccessLog* other)
{
	UplinkAssert(other != nullptr);
	SetProperties(other->_date, other->_fromIp, other->_fromName, other->_suspicious, other->_type);
	SetData1(other->_data1);
	SetData2(other->_data2);
	return SetData3(other->_data3);
}
