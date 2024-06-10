#include <LanGenerator.hpp>

#include <Globals.hpp>
#include <LanCluster.hpp>
#include <NameGenerator.hpp>
#include <NumberGenerator.hpp>
#include <WorldGenerator.hpp>

static void GenerateLAN_Level0Cluster(LanComputer* computer, LanCluster* cluster, LList<int>* radioFreqs, int x, int y, int width, int height)
{
	(void)computer;
	(void)cluster;
	(void)radioFreqs;
	(void)x;
	(void)y;
	(void)width;
	(void)height;
	puts("TODO: implement GenerateLAN_Level0Cluster()");
}

static void GenerateLAN_Level1Cluster(LanComputer* computer, LanCluster* cluster, LList<int>* radioFreqs, int x, int y, int width, int height)
{
	(void)computer;
	(void)cluster;
	(void)radioFreqs;
	(void)x;
	(void)y;
	(void)width;
	(void)height;
	puts("TODO: implement GenerateLAN_Level1Cluster()");
}

static void GenerateLAN_Level2Cluster(LanComputer* computer, LanCluster* cluster, LList<int>* radioFreqs)
{
	(void)computer;
	(void)cluster;
	(void)radioFreqs;
	puts("TODO: implement GenerateLAN_Level2Cluster()");
}

static void GenerateLAN_Level3Cluster(LanComputer* computer, LanCluster* cluster, LList<int>* radioFreqs)
{
	(void)computer;
	(void)cluster;
	(void)radioFreqs;
	puts("TODO: implement GenerateLAN_Level3Cluster()");
}

static void GenerateLAN_Level4Cluster(LanComputer* computer, LanCluster* cluster, LList<int>* radioFreqs)
{
	(void)computer;
	(void)cluster;
	(void)radioFreqs;
	puts("TODO: implement GenerateLAN_Level4Cluster()");
}

void LanGenerator::GenerateLANCluster(LanComputer* computer, int level)
{
	const auto cluster = new LanCluster();
	LList<int> radioFreqs;

	switch (level)
	{
		case 0:
			GenerateLAN_Level0Cluster(computer, cluster, &radioFreqs, 10, 100, 200, 200);
			break;
		case 1:
			GenerateLAN_Level1Cluster(computer, cluster, &radioFreqs, 10, 100, 275, 275);
			break;
		case 2:
			GenerateLAN_Level2Cluster(computer, cluster, &radioFreqs);
			break;
		case 3:
			GenerateLAN_Level3Cluster(computer, cluster, &radioFreqs);
			break;
		case 4:
			GenerateLAN_Level4Cluster(computer, cluster, &radioFreqs);
			break;
	}

	/*UplinkAssert(cluster->Input != 0);
	UplinkAssert(cluster->Output != 0);

	const auto inputSystem = cluster->Systems.GetData(cluster->Input);
	const auto outputSystem = cluster->Systems.GetData(cluster->Output);
	const auto router = GenerateRouter(computer, cluster, (inputSystem->X - 70), (inputSystem->Y - 100), 1);
	cluster->AddLanLink(router, cluster->Input, 1, 0.5f, 1.0f, 0.5f, 0.0f);

	if (level <= 1)
	{
		const auto mainServer = GenerateMainServer(computer, cluster, (outputSystem->X + 70), (outputSystem->Y + 120), 1);
		cluster->AddLanLink(cluster->Output, mainServer, 1, 0.5f, 1.0f, 0.5f, 0.0f);
	}

	cluster->Rotate(NumberGenerator::RandomNumber(4));
	cluster->Merge(computer);*/
	puts("TODO: implement LanGenerator::GenerateLANCluster()");

	delete cluster;
}

Computer* LanGenerator::GenerateLAN(const char* companyName, int level)
{
	NameGenerator::GenerateLANName(companyName);

	char value[0x80];
	UplinkStrncpy(value, tempname, 0x80);

	const auto vlocation = WorldGenerator::GenerateLocation();
	vlocation->SetListed(false);
	const auto computer = new LanComputer();
	computer->SetTYPE(0x40);
	computer->SetName(value);
	computer->SetCompanyName(companyName);
	computer->SetTraceSpeed(5);
	computer->SetTraceAction(9);
	computer->GetSecurity().AddSystem(1, 5, -1);
	computer->GetSecurity().AddSystem(4, 5, -1);
	computer->SetIP(vlocation->GetIp());
	game->GetWorld().CreateComputer(computer);
	GenerateLANCluster(computer, level);

	computer->GetDataBank().SetSize(NumberGenerator::RandomNormalNumber(100.0f, 40.0f));

	const auto fileCount = NumberGenerator::RandomNormalNumber(10.0f, 5.0f);

	for (auto i = 0; i < fileCount; i++)
	{
		const auto type = NumberGenerator::RandomNumber(2) + 1;
		const auto size = NumberGenerator::RandomNormalNumber(6.0f, 4.0f);

		NameGenerator::GenerateDataName(companyName, type);
		const auto data = new Data();
		data->SetTitle(tempname);

		auto compressed = 0;
		auto encrypted = 0;

		if (NumberGenerator::RandomNumber(2) != 0)
			compressed = NumberGenerator::RandomNumber(5);

		if (NumberGenerator::RandomNumber(2) != 0)
			encrypted = NumberGenerator::RandomNumber(5);

		data->SetDetails(type, size, encrypted, compressed, 1.0f, 0);
		computer->GetDataBank().PutData(data);
	}

	const auto logCount = NumberGenerator::RandomNumber(10);

	for (auto i = 0; i < logCount; i++)
	{
		const auto log = new AccessLog();
		const auto rax_10 = WorldGenerator::GetRandomLocation();
		log->SetProperties(game->GetWorld().GetCurrentDate(), rax_10->GetIp(), " ", 0, 1);
		log->SetData1("Accessed File");
		computer->GetLogBank().AddLog(log, -1);
	}

	const auto record = new Record();
	record->AddField("Name", "admin");
	NameGenerator::GenerateComplexPassword();
	record->AddField("Password", tempname);
	record->AddField("Security", "1");
	computer->GetRecordBank().AddRecord(*record);

	return computer;
}
