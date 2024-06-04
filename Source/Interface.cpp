#include <Interface.hpp>

#include <Eclipse.hpp>
#include <Globals.hpp>
#include <Opengl.hpp>

Interface::~Interface()
{
	if (_localInterface != 0)
		delete _localInterface;
	if (_remoteInterface != 0)
		delete _remoteInterface;
	if (_taskManager != 0)
		delete _taskManager;
}

bool Interface::Load(FILE* file)
{
	(void)file;
	puts("TODO: implement Interface::Load");
	return false;
}

void Interface::Save(FILE* file)
{
	(void)file;
	puts("TODO: implement Interface::Save");
}

void Interface::Print()
{
	puts("TODO: implement Interface::Print");
}

void Interface::Update()
{
	GetLocalInterface().Update();
	GetRemoteInterface().Update();

	static auto called = false;
	if (!called)
	{
		puts("TODO: implement Interface::Update");
		called = true;
	}
}

const char* Interface::GetID()
{
	return "INTERFA";
}

void Interface::Create()
{
	GetRemoteInterface().Create();

	if (!app->GetOptions()->IsOptionEqualTo("game_firsttime", 1))
	{
		puts("TODO: implement Interface::Create");
	}
}

LocalInterface& Interface::GetLocalInterface()
{
	UplinkAssert(_localInterface != nullptr);
	return *_localInterface;
}

RemoteInterface& Interface::GetRemoteInterface()
{
	UplinkAssert(_remoteInterface != nullptr);
	return *_remoteInterface;
}

TaskManager& Interface::GetTaskManager()
{
	UplinkAssert(_taskManager != nullptr);
	return *_taskManager;
}