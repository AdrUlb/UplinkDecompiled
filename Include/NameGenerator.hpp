#pragma once

namespace NameGenerator
{
	void Shutdown();
	void GeneratePublicAccessServerName(char const* companyName);
	void GenerateInternalServicesServerName(const char* companyName);
	void GenerateCentralMainframeName(const char* companyName);
	const char* GenerateInternationalBankName(const char* companyName);
	void GenerateLANName(const char* companyName);
	void GenerateDataName(const char* companyName, int type);
	const char* GeneratePassword();
	void GenerateComplexPassword();
} // namespace NameGenerator
