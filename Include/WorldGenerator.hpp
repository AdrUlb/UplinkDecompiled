#pragma once

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
} // namespace WorldGenerator
