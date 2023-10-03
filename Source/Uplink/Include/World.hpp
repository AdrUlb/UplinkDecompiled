#pragma once

#include <UplinkObject.hpp>
#include <Date.hpp>
#include <BTree.hpp>
#include <EventScheduler.hpp>
#include <PlotGenerator.hpp>
#include <VLocation.hpp>
#include <Company.hpp>
#include <Computer.hpp>
#include <Person.hpp>
#include <GatewayDef.hpp>

class World : UplinkObject
{
	Date unknown;
	Date ingameDateTime;
	EventScheduler eventScheduler;
	PlotGenerator plotGenerator;
	DemoPlotGenerator demoPlotGenerator;
	BTree<VLocation*> vLocations;
	BTree<Company*> companies;
	BTree<Computer*> computers;
	BTree<Person*> persons;
	DArray<char*> passwords;
	DArray<GatewayDef*> gatewayDefs;

public:
	World();
	virtual ~World();
};
