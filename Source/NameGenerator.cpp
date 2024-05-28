#include <NameGenerator.hpp>

#include <Globals.hpp>
#include <LList.hpp>

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
