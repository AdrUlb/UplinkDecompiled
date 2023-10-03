#pragma once

#include <UplinkObject.hpp>
#include <LList.hpp>

// TODO: replace hardcoded sizes

class PlotGenerator : UplinkObject
{
	int act;
	int scene;
	char act1scene3agent[0x80];
	char act1scene4agent[0x80];
	char act2scene1agent[0x80];
	bool playerVisitsPlotSites;
	bool playerCancelsMail;
	int playerLoyalty;
	bool completedTracer;
	bool completedTakeMeToYourLeader;
	bool completedArcInfiltration;
	bool completedDarwin;
	bool completedSaveItForTheJury;
	bool completedShinyHammer;
	char saveItForTheJury_guyToBeFramed[0x80];
	char saveItForTheJury_targetBankIp[0x18];
	char tracerLastKnownIp[0x18];
	int revelationUseCount;
	float revelationVersion;
	float faithVersion;
	LList<char*> infectedIpAddresses;
	bool revelation_releaseUncontrolled;
	bool revelation_releaseFailed;
	bool revelation_arcBusted;
	int specialMissionsCompleted;
};

class DemoPlotGenerator : UplinkObject
{
	int scene;
	char deadAgentName[0x80];
};
