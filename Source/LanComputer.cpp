#include <LanComputer.hpp>

LanComputer::~LanComputer()
{
	DeleteDArrayDataD(reinterpret_cast<DArray<UplinkObject*>*>(&_systems));
	DeleteDArrayDataD(reinterpret_cast<DArray<UplinkObject*>*>(&_links));
}

bool LanComputer::Load(FILE* file)
{
	if (!Computer::Load(file))
		return false;

	if (!LoadDArray(reinterpret_cast<DArray<UplinkObject*>*>(&_systems), file))
		return false;

	if (!LoadDArray(reinterpret_cast<DArray<UplinkObject*>*>(&_links), file))
		return false;

	return true;
}

void LanComputer::Save(FILE* file)
{
	Computer::Save(file);
	SaveDArray(reinterpret_cast<DArray<UplinkObject*>*>(&_systems), file);
	SaveDArray(reinterpret_cast<DArray<UplinkObject*>*>(&_links), file);
}

void LanComputer::Print()
{
	puts("LAN Computer");
	PrintDArray(reinterpret_cast<DArray<UplinkObject*>*>(&_systems));
	PrintDArray(reinterpret_cast<DArray<UplinkObject*>*>(&_links));
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

DArray<LanComputerSystem*>& LanComputer::GetSystems()
{
	return _systems;
}
