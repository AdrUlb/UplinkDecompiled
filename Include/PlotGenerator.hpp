#pragma once

#include <LList.hpp>
#include <UplinkObject.hpp>

class DemoPlotGenerator : public UplinkObject
{
	int scene = 0;

public:
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
};

class PlotGenerator : UplinkObject
{
	int act;
	int scene;
	char act1Scene3Agent[0x80];
	char act1Scene4Agent[0x80];
	char act2Scene1Agent[0x80];
	bool playerVisitsPlotSites;
	bool playerCancelsMail;
	int playerLoyalty;
	bool completedTracer;
	bool completedTakeMeToYourLeader;
	bool completedArcInfiltration;
	bool completedDarwin;
	bool completedSaveItForTheJury;
	bool completedShinyHammer;
	char saveItForTheJuryGuyToBeFramed[0x80];
	char saveItForTheJuryTargetBankIp[0x18];
	char tracerLastKnownIp[0x18];
	int numUsesRevelation;
	float versionRevelation;
	float versionFaith;
	LList<char*> revelationInfected;
	bool revelationReleaseUncontrolled;
	bool revelationReleaseFailed;
	bool revelationArcBusted;
	int specialMissionsCompleted;

public:
	PlotGenerator();
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	void Initialise();
};
