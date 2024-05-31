#include <AccessLog.hpp>

#include <Util.hpp>

AccessLog::~AccessLog()
{
	if (data1 != nullptr)
		delete[] data1;

	if (data2 != nullptr)
		delete[] data2;

	if (data3 != nullptr)
		delete[] data3;
}

bool AccessLog::Load(FILE* file)
{
	if (!date.Load(file))
		return false;

	if (!LoadDynamicStringBuf(fromIp, 0x18, file))
		return false;

	if (!LoadDynamicStringBuf(fromName, 0x80, file))
		return false;

	if (!FileReadData(&type, 4, 1, file))
		return false;

	if (!FileReadData(&suspicious, 4, 1, file))
		return false;

	if (!LoadDynamicString(data1, file))
		return false;

	if (!LoadDynamicString(data2, file))
		return false;

	if (!LoadDynamicString(data3, file))
		return false;

	return true;
}

void AccessLog::Save(FILE* file)
{
	date.Save(file);
	SaveDynamicString(fromIp, 0x18, file);
	SaveDynamicString(fromName, 0x80, file);
	fwrite(&type, 4, 1, file);
	fwrite(&suspicious, 4, 1, file);
	SaveDynamicString(data1, file);
	SaveDynamicString(data2, file);
	SaveDynamicString(data3, file);
}

void AccessLog::Print()
{
	printf("AccessLog : TYPE=%d\n", type);

	date.Print();

	printf("fromIP:%s, fromNAME:%s\n", fromIp, fromName);

	switch (suspicious)
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

	if (data1 != nullptr)
		printf("Data1 : %s\n", data1);

	if (data2 != nullptr)
		printf("Data2 : %s\n", data2);

	if (data3 != nullptr)
		printf("Data3 : %s\n", data3);
}

const char* AccessLog::GetID()
{
	return "ACC_LOG";
}

UplinkObjectId AccessLog::GetOBJECTID()
{
	return UplinkObjectId::AccessLog;
}

void AccessLog::SetTYPE(int value)
{
	type = value;
}

void AccessLog::SetSuspicious(int value)
{
	suspicious = value;
}

void AccessLog::SetFromIP(const char* value)
{
	UplinkStrncpy(fromIp, value, 0x18);
}

void AccessLog::SetData1(const char* value)
{
	if (data1 != nullptr)
	{
		delete[] data1;
		data1 = nullptr;
	}

	if (value == nullptr)
		return;

	data1 = new char[strlen(value) + 1];
	strcpy(data1, value);
}

void AccessLog::SetData2(const char* value)
{
	if (data2 != nullptr)
	{
		delete[] data2;
		data2 = nullptr;
	}

	if (value == nullptr)
		return;

	data2 = new char[strlen(value) + 1];
	strcpy(data2, value);
}

void AccessLog::SetData3(const char* value)
{
	if (data3 != nullptr)
	{
		delete[] data3;
		data3 = nullptr;
	}

	if (value == nullptr)
		return;

	data3 = new char[strlen(value) + 1];
	strcpy(data3, value);
}

void AccessLog::SetProperties(Date& date, const char* ip, const char* fromName, int suspicious, int type)
{
	this->date.SetDate(&date);
	SetTYPE(type);
	SetFromIP(ip);
	UplinkStrncpy(this->fromName, fromName, 0x80);
	SetSuspicious(suspicious);
}

void AccessLog::SetProperties(AccessLog* other)
{
	UplinkAssert(other != nullptr);
	SetProperties(other->date, other->fromIp, other->fromName, other->suspicious, other->type);
	SetData1(other->data1);
	SetData2(other->data2);
	return SetData3(other->data3);
}
