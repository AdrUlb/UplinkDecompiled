#pragma once

#include <UplinkEvent.hpp>

class NotificationEvent : UplinkEvent
{
	int type = 0;

public:
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	void Run() override;
	char* GetShortString() override;
	char* GetLongString() override;
	static void ScheduleStartingEvents();
};