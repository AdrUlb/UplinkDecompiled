#include <Security.hpp>

#include <Util.hpp>

bool SecuritySystem::Load(FILE* file)
{
	if (!FileReadData(&_type, 4, 1, file))
		return false;

	if (!FileReadData(&_enabled, 1, 1, file))
		return false;

	if (!FileReadData(&_bypassed, 1, 1, file))
		return false;

	if (!FileReadData(&_level, 4, 1, file))
		return false;

	return true;
}

void SecuritySystem::Save(FILE* file)
{
	fwrite(&_type, 4, 1, file);
	fwrite(&_enabled, 1, 1, file);
	fwrite(&_bypassed, 1, 1, file);
	fwrite(&_level, 4, 1, file);
}

void SecuritySystem::Print()
{
	printf("SecuritySystem : TYPE=%d, enabled=%d, bypassed=%d, level=%d\n", _type, _enabled, _bypassed, _level);
}

const char* SecuritySystem::GetID()
{
	return "SECSYST";
}

UplinkObjectId SecuritySystem::GetOBJECTID()
{
	return UplinkObjectId::SecuritySystem;
}

bool SecuritySystem::GetEnabled()
{
	return _enabled;
}

void SecuritySystem::SetTYPE(int type)
{
	_type = type;
}

void SecuritySystem::SetLevel(int level)
{
	_level = level;
}

void SecuritySystem::Enable()
{
	_enabled = true;
}

Security::~Security()
{
	DeleteDArrayDataD(reinterpret_cast<DArray<UplinkObject*>*>(&_systems));
}

bool Security::Load(FILE* file)
{
	return LoadDArray(reinterpret_cast<DArray<UplinkObject*>*>(&_systems), file);
}

void Security::Save(FILE* file)
{
	SaveDArray(reinterpret_cast<DArray<UplinkObject*>*>(&_systems), file);
}

void Security::Print()
{
	puts("Security : ");
	PrintDArray(reinterpret_cast<DArray<UplinkObject*>*>(&_systems));
}

const char* Security::GetID()
{
	return "SECUR";
}

void Security::AddSystem(SecuritySystem* system, int index)
{
	if (index == -1)
	{
		_systems.PutData(system);
		return;
	}

	_systems.PutData(system, index);
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
	for (auto i = 0; i < _systems.Size(); i++)
		if (_systems.ValidIndex(i) != 0 && _systems.GetData(i) != nullptr && !_systems.GetData(i)->GetEnabled())
			return true;

	return false;
}
