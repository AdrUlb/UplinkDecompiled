#include <PlotGenerator.hpp>

#include <Globals.hpp>
#include <Util.hpp>
#include <cstring>

PlotGenerator::PlotGenerator()
{
	act = 0;
	scene = 0;
	strncpy(act1Scene3Agent, " ", 0x80);
	strncpy(act1Scene4Agent, " ", 0x80);
	strncpy(act2Scene1Agent, " ", 0x80);
	playerLoyalty = 0;
	completedTracer = false;
	completedDarwin = false;
	completedSaveItForTheJury = false;
	completedShinyHammer = false;
	completedArcInfiltration = false;
	completedTakeMeToYourLeader = false;
	strncpy(saveItForTheJuryGuyToBeFramed, "None", 0x80);
	strcpy(saveItForTheJuryTargetBankIp, "None");
	strcpy(tracerLastKnownIp, "None ");
	numUsesRevelation = 0;
	versionRevelation = 0.0f;
	versionFaith = 0.0f;
	revelationReleaseUncontrolled = false;
	revelationReleaseFailed = false;
	revelationArcBusted = false;
	specialMissionsCompleted = 0;
}

bool PlotGenerator::Load(FILE* file)
{
	if (!FileReadData(&act, 4, 1, file))
		return false;

	if (!FileReadData(&scene, 4, 1, file))
		return false;

	if (strcmp(game->GetLoadedSavefileVer(), "SAV59") < 0)
	{
		auto success = FileReadData(act1Scene3Agent, 0x80, 1, file);
		act1Scene3Agent[0x7F] = 0;
		if (!success)
			return false;

		success = FileReadData(act1Scene4Agent, 0x80, 1, file);
		act1Scene4Agent[0x7F] = 0;
		if (!success)
			return false;

		success = FileReadData(act2Scene1Agent, 0x80, 1, file);
		act2Scene1Agent[0x7F] = 0;
		if (!success)
			return false;
	}
	else
	{
		if (!LoadString(act1Scene3Agent, 0x80, file))
			return false;

		if (!LoadString(act1Scene4Agent, 0x80, file))
			return false;

		if (!LoadString(act2Scene1Agent, 0x80, file))
			return false;
	}

	if (!FileReadData(&playerVisitsPlotSites, 1, 1, file))
		return false;

	if (!FileReadData(&playerCancelsMail, 1, 1, file))
		return false;

	if (!FileReadData(&playerLoyalty, 4, 1, file))
		return false;

	if (!FileReadData(&completedTracer, 1, 1, file))
		return false;

	if (!FileReadData(&completedTakeMeToYourLeader, 1, 1, file))
		return false;

	if (!FileReadData(&completedArcInfiltration, 1, 1, file))
		return false;

	if (!FileReadData(&completedDarwin, 1, 1, file))
		return false;

	if (!FileReadData(&completedSaveItForTheJury, 1, 1, file))
		return false;

	if (!FileReadData(&completedShinyHammer, 1, 1, file))
		return false;

	if (!FileReadData(&specialMissionsCompleted, 4, 1, file))
		return false;

	if (strcmp(game->GetLoadedSavefileVer(), "SAV59") < 0)
	{
		auto success = FileReadData(saveItForTheJuryGuyToBeFramed, 0x80, 1, file);
		saveItForTheJuryGuyToBeFramed[0x7f] = 0;
		if (!success)
			return false;

		success = FileReadData(saveItForTheJuryTargetBankIp, 0x18, 1, file);
		this->saveItForTheJuryTargetBankIp[0x17] = 0;
		if (!success)
			return false;

		success = FileReadData(tracerLastKnownIp, 0x18, 1, file);
		this->tracerLastKnownIp[0x17] = 0;
		if (!success)
			return false;
	}
	else
	{
		if (!LoadString(saveItForTheJuryGuyToBeFramed, 0x80, file))
			return false;

		if (!LoadString(saveItForTheJuryTargetBankIp, 0x18, file))
			return false;

		if (!LoadString(tracerLastKnownIp, 0x18, file))
			return false;
	}

	if (!FileReadData(&numUsesRevelation, 4, 1, file))
		return false;

	if (!FileReadData(&revelationReleaseUncontrolled, 1, 1, file))
		return false;

	if (!FileReadData(&revelationReleaseFailed, 1, 1, file))
		return false;

	if (!FileReadData(&revelationArcBusted, 1, 1, file))
		return false;

	if (!FileReadData(&versionRevelation, 4, 1, file))
		return false;

	if (!FileReadData(&versionFaith, 4, 1, file))
		return false;

	if (!LoadLList(&revelationInfected, file))
		return false;

	return true;
}

void PlotGenerator::Save(FILE* file)
{
	fwrite(&act, 4, 1, file);
	fwrite(&scene, 4, 1, file);
	SaveDynamicString(act1Scene3Agent, 0x80, file);
	SaveDynamicString(act1Scene4Agent, 0x80, file);
	SaveDynamicString(act2Scene1Agent, 0x80, file);
	fwrite(&playerVisitsPlotSites, 1, 1, file);
	fwrite(&playerCancelsMail, 1, 1, file);
	fwrite(&playerLoyalty, 4, 1, file);
	fwrite(&completedTracer, 1, 1, file);
	fwrite(&completedTakeMeToYourLeader, 1, 1, file);
	fwrite(&completedArcInfiltration, 1, 1, file);
	fwrite(&completedDarwin, 1, 1, file);
	fwrite(&completedSaveItForTheJury, 1, 1, file);
	fwrite(&completedShinyHammer, 1, 1, file);
	fwrite(&specialMissionsCompleted, 4, 1, file);
	SaveDynamicString(saveItForTheJuryGuyToBeFramed, 0x80, file);
	SaveDynamicString(saveItForTheJuryTargetBankIp, 0x18, file);
	SaveDynamicString(tracerLastKnownIp, 0x18, file);
	fwrite(&numUsesRevelation, 4, 1, file);
	fwrite(&revelationReleaseUncontrolled, 1, 1, file);
	fwrite(&revelationReleaseFailed, 1, 1, file);
	fwrite(&revelationArcBusted, 1, 1, file);
	fwrite(&versionRevelation, 4, 1, file);
	fwrite(&versionFaith, 4, 1, file);
	SaveLList(&revelationInfected, file);
	SaveID_END(file);
}

void PlotGenerator::Print()
{
	puts("Plot Generator");
	printf("Act=%d, Scene=%d\n", act, scene);
	printf("Act1Scene3Agent = %s\n", act1Scene3Agent);
	printf("Act1Scene4Agent = %s\n", act1Scene4Agent);
	printf("Act2Scene1Agent = %s\n", act2Scene1Agent);
	printf("PlayerVisitsPlotSites = %d\n", playerVisitsPlotSites);
	printf("PlayerCancelsMail = %d\n", playerCancelsMail);
	printf("PlayerLoyalty = %d\n", playerLoyalty);
	printf("NumUses_Revelation = %d\n", numUsesRevelation);
	printf("Version Revelation = %f\n", versionRevelation);
	printf("Version Faith = %f\n", versionFaith);
	printf("Completed Tracer : %d\n", completedTracer);
	printf("Completed TakeMeToYourLeader : %d\n", completedTakeMeToYourLeader);
	printf("Completed ARC Infiltration : %d\n", completedArcInfiltration);
	printf("Completed Darwin : %d\n", completedDarwin);
	printf("Completed SaveItForTheJury : %d\n", completedSaveItForTheJury);
	printf("Completed ShinyHammer : %d\n", completedShinyHammer);
	printf("specialmissionscompleted : %d\n", specialMissionsCompleted);
	printf("saveitforthejury_targetbankip : %s\n", saveItForTheJuryTargetBankIp);
	printf("saveitforthejury_guytobeframed : %s\n", saveItForTheJuryGuyToBeFramed);
	printf("tracer_lastknownip : %s\n", tracerLastKnownIp);
	printf("revelation_releaseuncontrolled : %d\n", revelationReleaseUncontrolled);
	printf("revelation_releasefailed : %d\n", revelationReleaseFailed);
	printf("revelation_arcbusted : %d\n", revelationArcBusted);
	puts("Infected IP addresses:");
	PrintLList(&revelationInfected);
}

const char* PlotGenerator::GetID()
{
	return "PLOTGEN";
}
