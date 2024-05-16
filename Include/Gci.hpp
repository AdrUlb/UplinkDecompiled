#pragma once

#include <cstdint>

struct GciInitFlags
{
	bool UnknownFlag0 : 1{false};
	bool UnknownFlag1 : 1{false};
	bool UnknownFlag2 : 1{false};
	bool UnknownFlag3 : 1{false};
};

void GciRestoreScreenSize();
const char* GciInitGraphicsLibrary(GciInitFlags flags);
