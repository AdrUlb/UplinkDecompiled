#pragma once

#include <DArray.hpp>
#include <UplinkObject.hpp>

typedef void SecuritySystem;

class Security : UplinkObject
{
	DArray<SecuritySystem*> systems;

public:
	~Security() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
};
