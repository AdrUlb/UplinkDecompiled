#include <Security.hpp>

#include <Util.hpp>

bool SecuritySystem::Load(FILE* file)
{
	if (!FileReadData(&type, 4, 1, file))
		return false;

	if (!FileReadData(&enabled, 1, 1, file))
		return false;

	if (!FileReadData(&bypassed, 1, 1, file))
		return false;

	if (!FileReadData(&level, 4, 1, file))
		return false;

	return true;
}

void SecuritySystem::Save(FILE* file)
{
	fwrite(&type, 4, 1, file);
	fwrite(&enabled, 1, 1, file);
	fwrite(&bypassed, 1, 1, file);
	fwrite(&level, 4, 1, file);
}

void SecuritySystem::Print()
{
	printf("SecuritySystem : TYPE=%d, enabled=%d, bypassed=%d, level=%d\n", type, enabled, bypassed, level);
}

const char* SecuritySystem::GetID()
{
	return "SECSYST";
}

UplinkObjectId SecuritySystem::GetOBJECTID()
{
	return UplinkObjectId::SecuritySystem;
}

void SecuritySystem::SetTYPE(int value)
{
	type = value;
}

void SecuritySystem::SetLevel(int value)
{
	level = value;
}

void SecuritySystem::Enable()
{
	enabled = true;
}

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

void Security::AddSystem(SecuritySystem* system, int index)
{
	if (index == -1)
	{
		systems.PutData(system);
		return;
	}

	systems.PutData(system, index);
}

void Security::AddSystem(int type, int level, int index)
{
	const auto system = new SecuritySystem();
	system->SetTYPE(type);
	system->SetLevel(level);
	system->Enable();
	AddSystem(system, index);
}

bool Security::IsAnythingDisabled()
{
	for (auto i = 0; i < systems.Size(); i++)
		if (systems.ValidIndex(i) != 0 && systems.GetData(i) != nullptr && !systems.GetData(i)->enabled)
			return true;

	return false;
}
