#pragma once

#include <Date.hpp>
#include <LList.hpp>
#include <UplinkEvent.hpp>
#include <UplinkObject.hpp>

class EventScheduler : UplinkObject
{
	LList<UplinkEvent*> events;

public:
	~EventScheduler() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
	Date* GetDateOfNextEvent();
};
