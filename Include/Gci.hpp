#pragma once

#include <cstdint>

struct GciInitFlags
{
	bool UnknownFlag0 : 1{false};
	bool UnknownFlag1 : 1{false};
	bool Fullscreen : 1{false};
	bool Debug : 1{false};
};

void GciRestoreScreenSize();
const char* GciInitGraphicsLibrary(GciInitFlags flags);
const char* GciInitGraphics(const char* title, GciInitFlags flags, int32_t width, int32_t height, int32_t depth);
int32_t* GciGetClosestScreenMode(int32_t width, int32_t height);
int32_t* GciGetCurrentScreenMode();
