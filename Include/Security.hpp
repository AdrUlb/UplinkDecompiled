#pragma once

#include <DArray.hpp>
#include <UplinkObject.hpp>

class SecuritySystem : public UplinkObject
{
	int _type = 0;
	int _level = 0;
	bool _enabled = false;
	bool _bypassed = false;

public:
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	bool GetEnabled();
	void SetTYPE(int type);
	void SetLevel(int level);
	void Enable();
};

class Security : UplinkObject
{
	DArray<SecuritySystem*> _systems;

public:
	~Security() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	void AddSystem(SecuritySystem* system, int index);
	void AddSystem(int type, int level, int index);
	bool IsAnythingDisabled();
};
