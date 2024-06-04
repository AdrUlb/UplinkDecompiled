#include <PlotGenerator.hpp>

#include <Globals.hpp>
#include <Util.hpp>
#include <cstring>

bool DemoPlotGenerator::Load(FILE* file)
{
	return FileReadData(&_scene, 4, 1, file);
}

void DemoPlotGenerator::Save(FILE* file)
{
	fwrite(&_scene, 4, 1, file);
}

void DemoPlotGenerator::Print()
{
	puts("Demo Plot Generator");
	printf("Scene %d\n", _scene);
}

void DemoPlotGenerator::Update()
{
	UplinkAbort("TODO: implement DemoPlotGenerator::Update()");
}

const char* DemoPlotGenerator::GetID()
{
	return "DEMOPGEN";
}

PlotGenerator::PlotGenerator()
{
	_act = 0;
	_scene = 0;
	strncpy(_act1Scene3Agent, " ", 0x80);
	strncpy(_act1Scene4Agent, " ", 0x80);
	strncpy(_act2Scene1Agent, " ", 0x80);
	_playerLoyalty = 0;
	_completedTracer = false;
	_completedDarwin = false;
	_completedSaveItForTheJury = false;
	_completedShinyHammer = false;
	_completedArcInfiltration = false;
	_completedTakeMeToYourLeader = false;
	strncpy(_saveItForTheJuryGuyToBeFramed, "None", 0x80);
	strcpy(_saveItForTheJuryTargetBankIp, "None");
	strcpy(_tracerLastKnownIp, "None ");
	_numUsesRevelation = 0;
	_versionRevelation = 0.0f;
	_versionFaith = 0.0f;
	_revelationReleaseUncontrolled = false;
	_revelationReleaseFailed = false;
	_revelationArcBusted = false;
	_specialMissionsCompleted = 0;
}

bool PlotGenerator::Load(FILE* file)
{
	if (!FileReadData(&_act, 4, 1, file))
		return false;

	if (!FileReadData(&_scene, 4, 1, file))
		return false;

	if (strcmp(game->GetLoadedSavefileVer(), "SAV59") < 0)
	{
		auto success = FileReadData(_act1Scene3Agent, 0x80, 1, file);
		_act1Scene3Agent[0x7F] = 0;
		if (!success)
			return false;

		success = FileReadData(_act1Scene4Agent, 0x80, 1, file);
		_act1Scene4Agent[0x7F] = 0;
		if (!success)
			return false;

		success = FileReadData(_act2Scene1Agent, 0x80, 1, file);
		_act2Scene1Agent[0x7F] = 0;
		if (!success)
			return false;
	}
	else
	{
		if (!LoadDynamicStringBuf(_act1Scene3Agent, 0x80, file))
			return false;

		if (!LoadDynamicStringBuf(_act1Scene4Agent, 0x80, file))
			return false;

		if (!LoadDynamicStringBuf(_act2Scene1Agent, 0x80, file))
			return false;
	}

	if (!FileReadData(&_playerVisitsPlotSites, 1, 1, file))
		return false;

	if (!FileReadData(&_playerCancelsMail, 1, 1, file))
		return false;

	if (!FileReadData(&_playerLoyalty, 4, 1, file))
		return false;

	if (!FileReadData(&_completedTracer, 1, 1, file))
		return false;

	if (!FileReadData(&_completedTakeMeToYourLeader, 1, 1, file))
		return false;

	if (!FileReadData(&_completedArcInfiltration, 1, 1, file))
		return false;

	if (!FileReadData(&_completedDarwin, 1, 1, file))
		return false;

	if (!FileReadData(&_completedSaveItForTheJury, 1, 1, file))
		return false;

	if (!FileReadData(&_completedShinyHammer, 1, 1, file))
		return false;

	if (!FileReadData(&_specialMissionsCompleted, 4, 1, file))
		return false;

	if (strcmp(game->GetLoadedSavefileVer(), "SAV59") < 0)
	{
		auto success = FileReadData(_saveItForTheJuryGuyToBeFramed, 0x80, 1, file);
		_saveItForTheJuryGuyToBeFramed[0x7f] = 0;
		if (!success)
			return false;

		success = FileReadData(_saveItForTheJuryTargetBankIp, 0x18, 1, file);
		this->_saveItForTheJuryTargetBankIp[0x17] = 0;
		if (!success)
			return false;

		success = FileReadData(_tracerLastKnownIp, 0x18, 1, file);
		this->_tracerLastKnownIp[0x17] = 0;
		if (!success)
			return false;
	}
	else
	{
		if (!LoadDynamicStringBuf(_saveItForTheJuryGuyToBeFramed, 0x80, file))
			return false;

		if (!LoadDynamicStringBuf(_saveItForTheJuryTargetBankIp, 0x18, file))
			return false;

		if (!LoadDynamicStringBuf(_tracerLastKnownIp, 0x18, file))
			return false;
	}

	if (!FileReadData(&_numUsesRevelation, 4, 1, file))
		return false;

	if (!FileReadData(&_revelationReleaseUncontrolled, 1, 1, file))
		return false;

	if (!FileReadData(&_revelationReleaseFailed, 1, 1, file))
		return false;

	if (!FileReadData(&_revelationArcBusted, 1, 1, file))
		return false;

	if (!FileReadData(&_versionRevelation, 4, 1, file))
		return false;

	if (!FileReadData(&_versionFaith, 4, 1, file))
		return false;

	if (!LoadLList(&_revelationInfected, file))
		return false;

	return true;
}

void PlotGenerator::Save(FILE* file)
{
	fwrite(&_act, 4, 1, file);
	fwrite(&_scene, 4, 1, file);
	SaveDynamicString(_act1Scene3Agent, 0x80, file);
	SaveDynamicString(_act1Scene4Agent, 0x80, file);
	SaveDynamicString(_act2Scene1Agent, 0x80, file);
	fwrite(&_playerVisitsPlotSites, 1, 1, file);
	fwrite(&_playerCancelsMail, 1, 1, file);
	fwrite(&_playerLoyalty, 4, 1, file);
	fwrite(&_completedTracer, 1, 1, file);
	fwrite(&_completedTakeMeToYourLeader, 1, 1, file);
	fwrite(&_completedArcInfiltration, 1, 1, file);
	fwrite(&_completedDarwin, 1, 1, file);
	fwrite(&_completedSaveItForTheJury, 1, 1, file);
	fwrite(&_completedShinyHammer, 1, 1, file);
	fwrite(&_specialMissionsCompleted, 4, 1, file);
	SaveDynamicString(_saveItForTheJuryGuyToBeFramed, 0x80, file);
	SaveDynamicString(_saveItForTheJuryTargetBankIp, 0x18, file);
	SaveDynamicString(_tracerLastKnownIp, 0x18, file);
	fwrite(&_numUsesRevelation, 4, 1, file);
	fwrite(&_revelationReleaseUncontrolled, 1, 1, file);
	fwrite(&_revelationReleaseFailed, 1, 1, file);
	fwrite(&_revelationArcBusted, 1, 1, file);
	fwrite(&_versionRevelation, 4, 1, file);
	fwrite(&_versionFaith, 4, 1, file);
	SaveLList(&_revelationInfected, file);
}

void PlotGenerator::Print()
{
	puts("Plot Generator");
	printf("Act=%d, Scene=%d\n", _act, _scene);
	printf("Act1Scene3Agent = %s\n", _act1Scene3Agent);
	printf("Act1Scene4Agent = %s\n", _act1Scene4Agent);
	printf("Act2Scene1Agent = %s\n", _act2Scene1Agent);
	printf("PlayerVisitsPlotSites = %d\n", _playerVisitsPlotSites);
	printf("PlayerCancelsMail = %d\n", _playerCancelsMail);
	printf("PlayerLoyalty = %d\n", _playerLoyalty);
	printf("NumUses_Revelation = %d\n", _numUsesRevelation);
	printf("Version Revelation = %f\n", _versionRevelation);
	printf("Version Faith = %f\n", _versionFaith);
	printf("Completed Tracer : %d\n", _completedTracer);
	printf("Completed TakeMeToYourLeader : %d\n", _completedTakeMeToYourLeader);
	printf("Completed ARC Infiltration : %d\n", _completedArcInfiltration);
	printf("Completed Darwin : %d\n", _completedDarwin);
	printf("Completed SaveItForTheJury : %d\n", _completedSaveItForTheJury);
	printf("Completed ShinyHammer : %d\n", _completedShinyHammer);
	printf("specialmissionscompleted : %d\n", _specialMissionsCompleted);
	printf("saveitforthejury_targetbankip : %s\n", _saveItForTheJuryTargetBankIp);
	printf("saveitforthejury_guytobeframed : %s\n", _saveItForTheJuryGuyToBeFramed);
	printf("tracer_lastknownip : %s\n", _tracerLastKnownIp);
	printf("revelation_releaseuncontrolled : %d\n", _revelationReleaseUncontrolled);
	printf("revelation_releasefailed : %d\n", _revelationReleaseFailed);
	printf("revelation_arcbusted : %d\n", _revelationArcBusted);
	puts("Infected IP addresses:");
	PrintLList(&_revelationInfected);
}

const char* PlotGenerator::GetID()
{
	return "PLOTGEN";
}

void PlotGenerator::Initialise()
{
	puts("TODO: implement PlotGenerator::Initialise()");
}

void PlotGenerator::PlayerVisitsPlotSites()
{
	_playerVisitsPlotSites = true;
}