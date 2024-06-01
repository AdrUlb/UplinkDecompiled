#include <LanGenerator.hpp>

#include <Globals.hpp>
#include <NameGenerator.hpp>
#include <NumberGenerator.hpp>
#include <WorldGenerator.hpp>

void LanGenerator::GenerateLANCluster(LanComputer* lanComputer, int level)
{
	(void)lanComputer;
	(void)level;
	puts("TODO: implement LanGenerator::GenerateLANCluster()");
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
	computer->security.AddSystem(1, 5, -1);
	computer->security.AddSystem(4, 5, -1);
	computer->SetIP(vlocation->ip);
	game->GetWorld()->CreateComputer(computer);
	GenerateLANCluster(computer, level);

	computer->dataBank.SetSize(NumberGenerator::RandomNormalNumber(100.0f, 40.0f));

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
		computer->dataBank.PutData(data);
	}

	const auto logCount = NumberGenerator::RandomNumber(10);

	for (auto i = 0; i < logCount; i++)
	{
		const auto log = new AccessLog();
		const auto rax_10 = WorldGenerator::GetRandomLocation();
		log->SetProperties(game->GetWorld()->currentDate, rax_10->ip, " ", 0, 1);
		log->SetData1("Accessed File");
		computer->logBank.AddLog(log, -1);
	}

	const auto record = new Record();
	record->AddField("Name", "admin");
	NameGenerator::GenerateComplexPassword();
	record->AddField("Password", tempname);
	record->AddField("Security", "1");
	computer->recordBank.AddRecord(*record);

	return computer;
}
