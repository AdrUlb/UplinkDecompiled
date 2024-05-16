#include <Gci.hpp>
#include <cstdio>
#include <cstdlib>
#include <Util.hpp>
#include <SDL/SDL.h>

bool _GciIsInitGraphicsLibrary = false;

void GciRestoreScreenSize()
{
	UplinkAbort("TODO: implement GciRestoreScreenSize()");
}

const char* GciInitGraphicsLibrary(GciInitFlags flags)
{
	const auto debug = flags.UnknownFlag3;

	if (debug)
		printf("Initialising SDL...");

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		const auto format = "Could not initialize SDL: %s.";
		const auto sdlError = SDL_GetError();

		const auto byteCount = strlen(format) + strlen(sdlError) + 1;

		const auto errorBuffer = new char[byteCount];

		snprintf(errorBuffer, byteCount, format, sdlError);
		
		return errorBuffer;
	}

	if (debug)
		printf("done\n ");

	_GciIsInitGraphicsLibrary = 1;
	return nullptr;
}
