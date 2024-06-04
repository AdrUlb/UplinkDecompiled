#pragma once

#include <BTree.hpp>
#include <Company.hpp>
#include <Computer.hpp>
#include <DArray.hpp>
#include <Date.hpp>
#include <EventScheduler.hpp>
#include <GatewayDef.hpp>
#include <Person.hpp>
#include <PlotGenerator.hpp>
#include <UplinkObject.hpp>
#include <VLocation.hpp>

class World : UplinkObject
{
	Date _nextUpdateDate;
	Date _currentDate;
	EventScheduler _eventScheduler;
	PlotGenerator _plotGenerator;
	DemoPlotGenerator _demoPlotGenerator;
	char _unknown[80];
	BTree<VLocation*> _vlocations;
	BTree<Company*> _companies;
	BTree<Computer*> _computers;
	BTree<Person*> _people;
	DArray<char*> _passwords;
	DArray<GatewayDef*> _gatewayDefs;

public:
	World();
	~World() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
	void ForceNextUpdate();
	Date& GetCurrentDate();
	EventScheduler& GetEventScheduler();
	PlotGenerator& GetPlotGenerator();
	BTree<VLocation*>& GetVLocations();
	BTree<Company*>& GetCompanies();
	BTree<Computer*>& GetComputers();
	DArray<char*>& GetPasswords();
	DArray<GatewayDef*>& GetGatewayDefs();
	Player& GetPlayer();
	VLocation* GetVLocation(const char* ip);
	Computer* GetComputer(const char* name);
	Company* GetCompany(const char* name);
	Person* GetPerson(const char* name);
	Person* CreatePerson(const char* name, const char* host);
	void CreatePerson(Person* person);
	Company* CreateCompany(const char* name);
	void CreateCompany(Company* company);
	VLocation* CreateVLocation(const char* ip, int x, int y);
	void CreateVLocation(VLocation* vlocation);
	Computer* CreateComputer(const char* computerName, const char* companyName, const char* ip);
	void CreateComputer(Computer* computer);
	void CreatePassword(const char* password);
	void CreateGatewayDef(GatewayDef* def);
};
