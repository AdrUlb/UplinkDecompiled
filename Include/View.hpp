#pragma once

#include <UplinkObject.hpp>

class View : UplinkObject
{
public:
	View();
	~View() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
};
