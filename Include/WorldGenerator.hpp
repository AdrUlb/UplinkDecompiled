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

} // namespace WorldGenerator
