#pragma once

class FrameRate
{
	int lastTime;
	int count;
	float fps;

	FrameRate();

	long double FPS();
	void Start();
	void Tick();
};
