#pragma once

#include <UplinkObject.hpp>

class DemoPlotGenerator : public UplinkObject
{
	int scene;

public:
	DemoPlotGenerator();
	~DemoPlotGenerator() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
};
