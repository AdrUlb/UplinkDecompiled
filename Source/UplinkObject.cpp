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
