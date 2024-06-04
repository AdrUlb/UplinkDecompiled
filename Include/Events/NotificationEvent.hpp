#pragma once

#include <Events/UplinkEvent.hpp>

enum class NotificationType
{
	Unknown,
	GrowCompanies,
	GenerateNewMissions,
	CheckForSecurityBreaches,
	CheckMissionDueDates,
	CheckRecentHackCount,
	GiveMissionToNpc,
	PayUplinkMonthlyFee,
	ExpireOldStuff,
	AddInterestToLoans,
	DemoGameOver,
	DemoGenerateMission,
	BuyAgentList,
	AgentsOnListDie,
	WarezGameOver
};

class NotificationEvent : public UplinkEvent
{
	NotificationType _type = NotificationType::Unknown;

public:
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	void Run() override;
	char* GetShortString() override;
	char* GetLongString() override;
	void SetTYPE(NotificationType type);
	static void ScheduleStartingEvents();
	void ApplyMonthlyGrowth();
	void GenerateNewMissions();
	void CheckForSecurityBreaches();
	void CheckMissionDueDates();
	void CheckRecentHackCount();
	void GiveMissionToNpc();
	void PayUplinkMonthlyFee();
	void ExpireOldStuff();
	void AddInterestToLoans();
	void DemoGameOver();
	void DemoGenerateNewMission();
	void BuyAgentList();
	void AgentsOnListDie();
	void WarezGameOver();
};