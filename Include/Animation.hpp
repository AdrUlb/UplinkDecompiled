#pragma once

typedef void (*AnimationFinishedCallback)();

struct Animation
{
	char* buttonName;
	struct Button* button;
	int fromX;
	int fromY;
	int toX;
	int toY;
	float stepX;
	float stepY;
	int fromWidth;
	int fromHeight;
	int toWidth;
	int toHeight;
	float stepWidth;
	float stepHeight;
	char* targetCaption;
	float stepCaption;
	int time;
	int starttime;
	int finishTime;
	int moveType;
	AnimationFinishedCallback finishedCallback;

	Animation();
	~Animation();
};
