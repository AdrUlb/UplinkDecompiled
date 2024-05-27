#include <Security.hpp>

#include <Util.hpp>

Security::~Security()
{
	DeleteDArrayDataD(&systems);
}

bool Security::Load(FILE* file)
{
	return LoadDArray(reinterpret_cast<DArray<UplinkObject*>*>(&systems), file);
}

void Security::Save(FILE* file)
{
	SaveDArray(reinterpret_cast<DArray<UplinkObject*>*>(&systems), file);
	SaveID_END(file);
}

void Security::Print()
{
	puts("Security : ");
	PrintDArray(reinterpret_cast<DArray<UplinkObject*>*>(&systems));
}

const char* Security::GetID()
{
	return "SECUR";
}
