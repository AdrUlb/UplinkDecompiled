#pragma once

#include <BTree.hpp>
#include <Company.hpp>
#include <Computer.hpp>
#include <DArray.hpp>
#include <Date.hpp>
#include <DemoPlotGenerator.hpp>
#include <EventScheduler.hpp>
#include <GatewayDef.hpp>
#include <Person.hpp>
#include <PlotGenerator.hpp>
#include <UplinkObject.hpp>
#include <VLocation.hpp>

struct World : UplinkObject
{
	Date nextUpdateDate;
	Date currentDate;
	EventScheduler eventScheduler;
	PlotGenerator plotGenerator;
	DemoPlotGenerator demoPlotGenerator;
	char unknown[80];
	BTree<VLocation*> vlocations;
	BTree<Company*> companies;
	BTree<Computer*> computers;
	BTree<Person*> people;
	DArray<char*> passwords;
	DArray<GatewayDef*> gatewayDefs;

	World();
	~World() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
	void ForceNextUpdate();
};
