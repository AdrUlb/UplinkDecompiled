#include <Interface.hpp>

#include <Eclipse.hpp>
#include <Globals.hpp>
#include <Opengl.hpp>

Interface::~Interface()
{
	if (localInterface != 0)
		delete localInterface;
	if (remoteInterface != 0)
		delete remoteInterface;
	if (taskManager != 0)
		delete taskManager;
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
	GetLocalInterface()->Update();
	GetRemoteInterface()->Update();

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
	Interface::GetRemoteInterface()->Create();

	if (!app->GetOptions()->IsOptionEqualTo("game_firsttime", 1))
	{
		puts("TODO: implement Interface::Create");
	}
}

LocalInterface* Interface::GetLocalInterface()
{
	UplinkAssert(localInterface != nullptr);
	return localInterface;
}

RemoteInterface* Interface::GetRemoteInterface()
{
	UplinkAssert(remoteInterface != nullptr);
	return remoteInterface;
}

TaskManager* Interface::GetTaskManager()
{
	UplinkAssert(taskManager != nullptr);
	return taskManager;
}