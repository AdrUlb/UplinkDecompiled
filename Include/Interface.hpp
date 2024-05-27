#pragma once

#include <TaskManager.hpp>
#include <UplinkObject.hpp>

typedef void LocalInterface;
typedef void RemoteInterface;

class Interface : UplinkObject
{
	LocalInterface* localInterface;
	RemoteInterface* remoteInterface;
	TaskManager* taskManager;

public:
	Interface();
	~Interface() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
	void Create();
};
