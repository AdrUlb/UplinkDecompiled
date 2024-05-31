#pragma once

#include <UplinkObject.hpp>

class TaskManager : UplinkObject
{
	int targetProgramPid = -1;
	int unknown = 0;
	int lastTargetProgramPid = -1;
	UplinkObject* programTarget = nullptr;
	int lastUpdateTime = 0;

public:
	TaskManager();
	void Print() override;
	void Update() override;
	const char* GetID() override;
	void SetTargetProgram(int pid);
};
