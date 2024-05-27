#pragma once

#include <UplinkObject.hpp>

class FrameRate
{
	int lastTime;
	int ticks = 30;
	float fps = 0.0f;

public:
	void Start();
};

class View : UplinkObject
{
	FrameRate frameRate;

public:
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	void Initialise();
};
