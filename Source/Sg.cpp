#include <Sg.hpp>

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

static bool SgInitialised = false;

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

	if (SgInitialised)
		return;

	int freq;
	uint16_t format;
	int channels;
	if (Mix_QuerySpec(&freq, &format, &channels) == 0)
	{
		SgInitialised = true;
		return;
	}

	const char* formatString;
	switch (format)
	{
		case AUDIO_U8:
			formatString = "U8";
			break;
		case AUDIO_S8:
			formatString = "S8";
			break;
		case AUDIO_U16MSB:
			formatString = "U16MSB";
			break;
		case AUDIO_U16LSB:
			formatString = "U16LSB";
			break;
		case AUDIO_S16MSB:
			formatString = "S16MSB";
			break;
		case AUDIO_S16LSB:
			formatString = "S16LSB";
			break;
		default:
			formatString = "Unknown";
			break;
	}

	printf("SDL_Mixer audio configuration: %dHz (%s) %d channels\n", freq, formatString, channels);

	SgInitialised = true;
}
