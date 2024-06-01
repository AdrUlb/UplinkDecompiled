#pragma once

#include <LanComputer.hpp>

namespace LanGenerator
{
	void GenerateLANCluster(LanComputer* lanComputer, int level);
	Computer* GenerateLAN(const char* companyName, int level);
} // namespace LanGenerator
