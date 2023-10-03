#pragma once

#include <UplinkObject.hpp>
#include <TaskManager.hpp>

typedef void LocalInterface;
typedef void RemoteInterface;

class Interface : UplinkObject
{
	LocalInterface* local;
	RemoteInterface* remote;
	TaskManager* taskManager;
};
