#pragma once

#include <DArray.hpp>
#include <UplinkObject.hpp>

class SecuritySystem : UplinkObject
{
public:
	int type = 0;
	int level = 0;
	bool enabled = false;
	bool bypassed = false;

	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	void SetTYPE(int value);
	void SetLevel(int value);
	void Enable();
};

class Security : UplinkObject
{
	DArray<SecuritySystem*> systems;

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
