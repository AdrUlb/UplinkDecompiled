#pragma once

#include <Date.hpp>
#include <Events/UplinkEvent.hpp>
#include <LList.hpp>
#include <UplinkObject.hpp>

class EventScheduler : UplinkObject
{
	LList<UplinkEvent*> _events;

public:
	~EventScheduler() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
	Date* GetDateOfNextEvent();
	bool UpdateProcessEvents();
	void ScheduleEvent(UplinkEvent* event);
};
