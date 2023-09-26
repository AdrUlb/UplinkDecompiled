#include <UplinkObject.hpp>

#include <Util.hpp>

UplinkObject::UplinkObject()
{

}

UplinkObject::~UplinkObject()
{

}

bool UplinkObject::Load(FILE* file)
{
	(void)file;
	return true;
}

void UplinkObject::Save(FILE* file)
{
	(void)file;
}

void UplinkObject::Print()
{

}

void UplinkObject::Update()
{

}

const char* UplinkObject::GetID()
{
	return "UOBJ";
}

int UplinkObject::GetOBJECTID()
{
	return 0;
}

char* UplinkObject::GetID_END()
{
	const auto buffer = new char[13];
	UplinkSnprintf(buffer, 13, "%s_END", GetID());
	return buffer;
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
