#include <MissionGenerator.hpp>

#include <Probability.hpp>
#include <Util.hpp>

static int PROB_MISSION_SPECIAL[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static int PROB_MISSION_STEALFILE[] = {30, 40, 30, 15, 5, 30, 15, 10, 0, 0, 0, 5, 5, 5, 5, 5, 5};
static int PROB_MISSION_DESTROYFILE[] = {30, 40, 30, 15, 40, 10, 15, 5, 0, 0, 0, 5, 5, 5, 5, 5, 5};
static int PROB_MISSION_FINDDATA[] = {15, 5, 15, 10, 15, 30, 15, 15, 5, 0, 0, 5, 5, 5, 5, 5, 5};
static int PROB_MISSION_CHANGEDATA[] = {25, 15, 25, 60, 40, 30, 20, 15, 15, 5, 0, 5, 5, 5, 5, 5, 5};
static int PROB_MISSION_FRAMEUSER[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 30, 20, 20, 20, 20, 20, 20};
static int PROB_MISSION_TRACEUSER[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static int PROB_MISSION_CHANGEACCOUNT[] = {0, 0, 0, 0, 0, 0, 0, 30, 20, 25, 30, 20, 20, 20, 20, 20, 20};
static int PROB_MISSION_REMOVECOMPUTER[] = {0, 0, 0, 0, 0, 0, 35, 25, 20, 5, 10, 20, 20, 20, 20, 20, 20};
static int PROB_MISSION_REMOVECOMPANY[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static int PROB_MISSION_REMOVEUSER[] = {0, 0, 0, 0, 0, 0, 0, 0, 40, 25, 30, 20, 20, 20, 20, 20, 20};

static Probability* prob_missiontype[11];

void MissionGenerator::Initialise()
{
	for (auto i = 0; i < 17; i++)
	{
		prob_missiontype[i] = new Probability();

		prob_missiontype[i]->InputProbability(0, PROB_MISSION_SPECIAL[i]);
		prob_missiontype[i]->InputProbability(1, PROB_MISSION_STEALFILE[i]);
		prob_missiontype[i]->InputProbability(2, PROB_MISSION_DESTROYFILE[i]);
		prob_missiontype[i]->InputProbability(3, PROB_MISSION_FINDDATA[i]);
		prob_missiontype[i]->InputProbability(4, PROB_MISSION_CHANGEDATA[i]);
		prob_missiontype[i]->InputProbability(5, PROB_MISSION_FRAMEUSER[i]);
		prob_missiontype[i]->InputProbability(6, PROB_MISSION_TRACEUSER[i]);
		prob_missiontype[i]->InputProbability(7, PROB_MISSION_CHANGEACCOUNT[i]);
		prob_missiontype[i]->InputProbability(8, PROB_MISSION_REMOVECOMPUTER[i]);
		prob_missiontype[i]->InputProbability(9, PROB_MISSION_REMOVECOMPANY[i]);
		prob_missiontype[i]->InputProbability(10, PROB_MISSION_REMOVEUSER[i]);

		UplinkAssert(prob_missiontype[i]->Validate());
	}
}

void MissionGenerator::Shutdown()
{
	UplinkAbort("TODO: implement MissionGenerator::Shutdown()");
}
