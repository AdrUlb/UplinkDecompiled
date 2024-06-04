#include <Svb.hpp>
#include <TaskManager.hpp>

TaskManager::TaskManager()
{
	SvbReset();
}

void TaskManager::Print()
{
	puts("Task Manager ");
}

void TaskManager::Update()
{
	static auto called = false;
	if (!called)
	{
		puts("TODO: TaskManager::Update()");
		called = true;
	}
}

const char* TaskManager::GetID()
{
	return "TASKMAN";
}

void TaskManager::SetTargetProgram(int pid)
{
	if (pid != -1)
		_unknown = 0;
	_lastTargetProgramPid = _targetProgramPid;
	_targetProgramPid = pid;
}
