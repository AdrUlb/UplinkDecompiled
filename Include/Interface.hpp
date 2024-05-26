#pragma once

#include <TaskManager.hpp>
#include <UplinkObject.hpp>

typedef void LocalInterface;
typedef void RemoteInterface;

struct Interface : UplinkObject
{
	LocalInterface* localInterface;
	RemoteInterface* remoteInterface;
	TaskManager* taskManager;

	Interface();
	~Interface() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
};
