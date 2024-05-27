#pragma once

namespace WorldGenerator
{
	void Initialise();
	void Shutdown();
	void ReplaceInvalidCompanyAdmins();
	void UpdateSoftwareUpgrades();
	void LoadDynamicsGatewayDefs();
	void GenerateAll();
	void GenerateSimpleStartingMissionA();
	void GenerateSimpleStartingMissionB();
} // namespace WorldGenerator
