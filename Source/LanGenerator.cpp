#include <LanGenerator.hpp>

#include <Globals.hpp>
#include <LanComputer.hpp>
#include <NameGenerator.hpp>
#include <WorldGenerator.hpp>

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

	puts("TODO: implement LanGenerator::GenerateLAN()");

	return computer;
}
