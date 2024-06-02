#pragma once

#include <UplinkObject.hpp>

class TaskManager : UplinkObject
{
	int _targetProgramPid = -1;
	int _unknown = 0;
	int _lastTargetProgramPid = -1;
	UplinkObject* _programTarget = nullptr;
	int _lastUpdateTime = 0;

public:
	TaskManager();
	void Print() override;
	void Update() override;
	const char* GetID() override;
	void SetTargetProgram(int pid);
};
