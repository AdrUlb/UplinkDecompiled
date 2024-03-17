#include <UplinkObject.hpp>
#include <Util.hpp>

UplinkObject::~UplinkObject() {}

bool UplinkObject::Load(FILE* file)
{
	(void)file;
	return true;
}

void UplinkObject::Save(FILE* file)
{
	(void)file;
}

void UplinkObject::Print() {}

void UplinkObject::Update() {}

const char* UplinkObject::GetID()
{
	return "UOBJ";
}

UplinkObjectId UplinkObject::GetOBJECTID()
{
	return UplinkObjectId::Unknown;
}

char* UplinkObject::GetID_END()
{
	constexpr size_t bufferSize = 0x20;
	const auto s = new char[bufferSize];
	UplinkSnprintf(s, bufferSize, "%s_END", GetID());
	return s;
}

void UplinkObject::LoadID(FILE* file)
{
	(void)file;
}

void UplinkObject::LoadID_END(FILE* file)
{
	(void)file;
}

void UplinkObject::SaveID(FILE* file)
{
	(void)file;
}

void UplinkObject::SaveID_END(FILE* file)
{
	(void)file;
}
