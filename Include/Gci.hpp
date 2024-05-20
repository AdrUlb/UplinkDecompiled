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
const char* GciInitGraphics(const char* title, GciInitFlags flags, int width, int height, int depth);
int* GciGetClosestScreenMode(int width, int height);
int* GciGetCurrentScreenMode();
