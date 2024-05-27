#include <UplinkEvent.hpp>

#include <Util.hpp>

bool UplinkEvent::Load(FILE* file)
{
	return runDate.Load(file);
}

void UplinkEvent::Save(FILE* file)
{
	runDate.Save(file);
}

void UplinkEvent::Print()
{
	puts("UplinkEvent:");
	runDate.Print();
}

const char* UplinkEvent::GetID()
{
	UplinkAbort("This MUST be overridden");
}

UplinkObjectId UplinkEvent::GetOBJECTID()
{
	UplinkAbort("This MUST be overridden");
}

void UplinkEvent::Run()
{
	UplinkAbort("This MUST be overridden");
}

void UplinkEvent::RunWarning() {}

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
	runDate.SetDate(date);
}
