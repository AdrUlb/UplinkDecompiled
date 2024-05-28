#pragma once

#include <Company.hpp>
#include <Computer.hpp>
#include <VLocation.hpp>

namespace WorldGenerator
{
	void Initialise();
	void Shutdown();
	void ReplaceInvalidCompanyAdmins();
	void UpdateSoftwareUpgrades();
	void LoadDynamicsGatewayDefs();
	void GenerateAll();
	void GenerateSpecifics();
	void GeneratePlayer(const char* handle);
	void GenerateRandomWorld();
	void LoadDynamics();
	void GenerateSimpleStartingMissionA();
	void GenerateSimpleStartingMissionB();
	void GenerateLocalMachine();
	void GenerateCompanyGovernment();
	void GenerateGlobalCriminalDatabase();
	void GenerateInternationalSocialSecurityDatabase();
	void GenerateInternationalAcademicDatabase();
	void GenerateInterNIC();
	void GenerateStockMarket();
	void GenerateProtoVision();
	void GenerateOCP();
	void GenerateSJGames();
	void GenerateIntroversion();
	void GenerateCompanyUplink();
	VLocation* GenerateLocation();
	Computer* GenerateComputer(const char* name);
	Company* GenerateCompany(const char* name, int size, int type, int growth, int alignment);
	Computer* GeneratePublicAccessServer(const char* name);
	Computer* GenerateInternalServicesMachine(const char* name);
	Computer* GenerateCentralMainframe(const char* name);
	Computer* GenerateLAN(const char* name);
	void GenerateValidMapPos(int& outX, int& outY);
} // namespace WorldGenerator
