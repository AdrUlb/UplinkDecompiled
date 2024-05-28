#include <Rating.hpp>

#include <Util.hpp>

bool Rating::Load(FILE* file)
{
	(void)file;
	puts("TODO: implement Rating::Load()");
	return false;
}

void Rating::Save(FILE* file)
{
	(void)file;
	puts("TODO: implement Rating::Save()");
}

void Rating::Print()
{
	puts("TODO: implement Rating::Print()");
}

const char* Rating::GetID()
{
	return "RATING";
}

void Rating::SetOwner(const char* value)
{
	UplinkStrncpy(owner, value, 0x80);
}
