#pragma once

#include <LList.hpp>
#include <UplinkObject.hpp>

class DemoPlotGenerator : public UplinkObject
{
	int _scene = 0;

public:
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
};

class PlotGenerator : UplinkObject
{
	int _act;
	int _scene;
	char _act1Scene3Agent[0x80];
	char _act1Scene4Agent[0x80];
	char _act2Scene1Agent[0x80];
	bool _playerVisitsPlotSites;
	bool _playerCancelsMail;
	int _playerLoyalty;
	bool _completedTracer;
	bool _completedTakeMeToYourLeader;
	bool _completedArcInfiltration;
	bool _completedDarwin;
	bool _completedSaveItForTheJury;
	bool _completedShinyHammer;
	char _saveItForTheJuryGuyToBeFramed[0x80];
	char _saveItForTheJuryTargetBankIp[0x18];
	char _tracerLastKnownIp[0x18];
	int _numUsesRevelation;
	float _versionRevelation;
	float _versionFaith;
	LList<char*> _revelationInfected;
	bool _revelationReleaseUncontrolled;
	bool _revelationReleaseFailed;
	bool _revelationArcBusted;
	int _specialMissionsCompleted;

public:
	PlotGenerator();
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	void Initialise();
};
