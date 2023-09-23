#include "Sound.hpp"

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include "../UplinkDecompiledTempGlobals.hpp"

void SgInitialise()
{
	printf("Compiled with SDL_mixer version: %d.%d.%d\n", SDL_MIXER_MAJOR_VERSION, SDL_MIXER_MINOR_VERSION, SDL_MIXER_PATCHLEVEL);

	const auto linkedVersion = Mix_Linked_Version();

	printf("Running with SDL_mixer version: %d.%d.%d\n", linkedVersion->major, linkedVersion->minor, linkedVersion->patch);

	if (SDL_Init(SDL_INIT_AUDIO) == -1)
	{
		printf("SDL_Init error: %s\n", SDL_GetError());
		return;
	}

	if (Mix_OpenAudio(22050, AUDIO_S16LSB, 2, 512) == -1)
	{
		printf("Mix_OpenAudio error: %s\n", SDL_GetError());
		return;
	}

	Mix_AllocateChannels(16);
	Mix_ReserveChannels(1);

	if (!gSgInitialised)
	{
		gSgInitialised = true;
		return;
	}

	int frequency;
	Uint16 format;
	int channels;
	if (Mix_QuerySpec(&frequency, &format, &channels) == 0)
	{
		gSgInitialised = true;
		return;
	}

	const char* formatString;
	if (format == AUDIO_U8)
		formatString = "U8";
	else if (format == AUDIO_S8)
		formatString = "S8";
	else if (format == AUDIO_U16MSB)
		formatString = "U16MSB";
	else if (format == AUDIO_S16MSB)
		formatString = "S16MSB";
	else if (format == AUDIO_U16LSB)
		formatString = "U16LSB";
	else if (format == AUDIO_S16LSB)
		formatString = "S16LSB";
	else
		formatString = "Unknown";

	printf("SDL_Mixer audio configuration: %dHz (%s) %d channels\n", frequency, formatString, channels);
	gSgInitialised = true;
	return;
}
