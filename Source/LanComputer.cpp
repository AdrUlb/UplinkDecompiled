#include <LanComputer.hpp>

LanComputer::~LanComputer()
{
	DeleteDArrayDataD(reinterpret_cast<DArray<UplinkObject*>*>(&systems));
	DeleteDArrayDataD(reinterpret_cast<DArray<UplinkObject*>*>(&links));
}

bool LanComputer::Load(FILE* file)
{
	if (!Computer::Load(file))
		return false;

	if (!LoadDArray(reinterpret_cast<DArray<UplinkObject*>*>(&systems), file))
		return false;

	if (!LoadDArray(reinterpret_cast<DArray<UplinkObject*>*>(&links), file))
		return false;

	return true;
}

void LanComputer::Save(FILE* file)
{
	Computer::Save(file);
	SaveDArray(reinterpret_cast<DArray<UplinkObject*>*>(&systems), file);
	SaveDArray(reinterpret_cast<DArray<UplinkObject*>*>(&links), file);
}

void LanComputer::Print()
{
	puts("LAN Computer");
	PrintDArray(reinterpret_cast<DArray<UplinkObject*>*>(&systems));
	PrintDArray(reinterpret_cast<DArray<UplinkObject*>*>(&links));
	Computer::Print();
}

const char* LanComputer::GetID()
{
	return "LANCOMP";
}

UplinkObjectId LanComputer::GetOBJECTID()
{
	return UplinkObjectId::LanComputer;
}