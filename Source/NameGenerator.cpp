#include <NameGenerator.hpp>

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
