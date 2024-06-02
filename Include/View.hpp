#pragma once

#include <UplinkObject.hpp>

class FrameRate
{
	int _lastTime;
	int _ticks = 30;
	float _fps = 0.0f;

public:
	void Start();
};

class View : public UplinkObject
{
	FrameRate _frameRate;

public:
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	void Initialise();
};
