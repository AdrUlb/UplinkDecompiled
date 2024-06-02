#pragma once

#include <Button.hpp>

typedef void (*AnimationFinishedCallback)();

struct Animation
{
	char* ButtonName = nullptr;
	::Button* Button = nullptr;
	int FromX;
	int FromY;
	int ToX;
	int ToY;
	float StepX;
	float StepY;
	int FromWidth;
	int FromHeight;
	int ToWidth;
	int ToHeight;
	float StepWidth;
	float StepHeight;
	char* TargetCaption = nullptr;
	float StepCaption;
	int Time;
	int StartTime;
	int FinishTime;
	int MoveType;
	AnimationFinishedCallback FinishedCallback;

	~Animation();
};
