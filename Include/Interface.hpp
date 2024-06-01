#pragma once

#include <Interface.hpp>
#include <Interface/LocalInterface.hpp>
#include <Interface/LocalScreens/LocalInterfaceScreen.hpp>
#include <Interface/RemoteInterface.hpp>
#include <LList.hpp>
#include <TaskManager.hpp>

class Interface : UplinkObject
{
	LocalInterface* localInterface = new LocalInterface();
	RemoteInterface* remoteInterface = new RemoteInterface();
	TaskManager* taskManager = new TaskManager();

public:
	~Interface() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
	void Create();
	LocalInterface* GetLocalInterface();
	RemoteInterface* GetRemoteInterface();
	TaskManager* GetTaskManager();
};
