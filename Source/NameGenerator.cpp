#include <NameGenerator.hpp>

#include <Globals.hpp>
#include <LList.hpp>
#include <NumberGenerator.hpp>

static LList<char*> fornames;
static LList<char*> surnames;
static LList<char*> agentaliases;
static LList<char*> companynamesA;
static LList<char*> companynamesB;

void NameGenerator::Shutdown()
{
	DeleteLListData(&fornames);
	DeleteLListData(&surnames);
	DeleteLListData(&agentaliases);
	DeleteLListData(&companynamesA);
	DeleteLListData(&companynamesB);
	fornames.Empty();
	surnames.Empty();
	agentaliases.Empty();
	companynamesA.Empty();
	companynamesB.Empty();
}

void NameGenerator::GeneratePublicAccessServerName(char const* companyName)
{
	UplinkSnprintf(tempname, 0x80, "%s Public Access Server", companyName);
}

void NameGenerator::GenerateInternalServicesServerName(char const* companyName)
{
	UplinkSnprintf(tempname, 0x80, "%s Internal Services Machine", companyName);
}

void NameGenerator::GenerateCentralMainframeName(const char* companyName)
{
	UplinkSnprintf(tempname, 0x80, "%s Central Mainframe", companyName);
}

void NameGenerator::GenerateLANName(const char* companyName)
{
	UplinkSnprintf(tempname, 0x80, "%s Local Area Network", companyName);
}

void NameGenerator::GenerateDataName(const char* companyName, int type)
{
	UplinkAssert(companyName != nullptr);

	const auto num = NumberGenerator::RandomNumber(99999);

	char s[0x40];
	const char* typeString;

	switch (type)
	{
		case 0:
			typeString = "file";
			break;
		case 1:
			typeString = "data";
			break;
		case 2:
			typeString = "prog";
			break;
		default:
			typeString = "unknown";
			break;
	}

	UplinkSnprintf(s, 0x40, "%c%c%c-%s-%d", companyName[0], companyName[1], companyName[2], typeString, num);
	strncpy(tempname, s, 0x80);
}

const char* NameGenerator::GeneratePassword()
{
	const auto& passwords = game->GetWorld()->passwords;

	while (true)
	{
		const auto index = NumberGenerator::RandomNumber(passwords.Size());

		if (passwords.ValidIndex(index))
			return passwords.GetData(index);
	}
}

void NameGenerator::GenerateComplexPassword()
{
	char pass[0x9];
	strncpy(pass, NameGenerator::GeneratePassword(), 9);

	const auto count = NumberGenerator::RandomNumber(5);
	for (auto i = 0; i < count; i++)
	{
		const auto changeIndex = NumberGenerator::RandomNumber(8);
		pass[changeIndex] = 'a' + NumberGenerator::RandomNumber(26);
	}

	strncpy(tempname, pass, 0x80);
}
