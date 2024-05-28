#pragma once

namespace NameGenerator
{
	void Shutdown();
	void GeneratePublicAccessServerName(char const* companyName);
	void GenerateInternalServicesServerName(const char* companyName);
	void GenerateCentralMainframeName(const char* companyName);
	void GenerateLANName(const char* companyName);
} // namespace NameGenerator
