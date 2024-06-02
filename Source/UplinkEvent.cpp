#include <UplinkEvent.hpp>

#include <Util.hpp>

bool UplinkEvent::Load(FILE* file)
{
	return _runDate.Load(file);
}

void UplinkEvent::Save(FILE* file)
{
	_runDate.Save(file);
}

void UplinkEvent::Print()
{
	puts("UplinkEvent:");
	_runDate.Print();
}

const char* UplinkEvent::GetID()
{
	UplinkAbort("This MUST be overridden");
}

UplinkObjectId UplinkEvent::GetOBJECTID()
{
	UplinkAbort("This MUST be overridden");
}

Date& UplinkEvent::GetRunDate()
{
	return _runDate;
}

char* UplinkEvent::GetShortString()
{
	const auto str = new char[0x20];
	strncpy(str, "Uplink Event", 0x20);
	return str;
}

char* UplinkEvent::GetLongString()
{
	const auto str = new char[0x20];
	strncpy(str, "Uplink Event", 0x20);
	return str;
}

void UplinkEvent::SetRunDate(Date* date)
{
	UplinkAssert(date != nullptr);
	_runDate.SetDate(date);
}

void UplinkEvent::Run()
{
	UplinkAbort("This MUST be overridden");
}

void UplinkEvent::RunWarning() {}
