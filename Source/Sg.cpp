#include <Sg.hpp>

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

static bool SgInitialised = false;

static char requestedplaylist[0x40];
static char currentplaylist[0x40];
static char currentsong[0x100];
static int requestedtime = 0;
static int songindex = 0;
static int playerVolume = 64;

static struct LList<SgPlaylist*> playlists;

SgPlaylist::~SgPlaylist()
{
	for (int i = 0; i < songs.Size(); i++)
	{
		const auto song = songs.GetData(i);

		if (song == nullptr || song[0] == 0)
			continue;

		delete[] song;
	}
}

void SgPlaylist::SetName(const char* const value)
{
	strcpy(name, value);
}

void SgPlaylist::AddSong(const char* const name)
{
	const auto str = new char[strlen(name) + 1];
	sprintf(str, name);
	songs.PutData(str);
}

static int musicVol()
{
	return playerVolume / 20.0 * 85.333333333333329;
}

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

void SgPlaylist_Initialise()
{
	strcpy(currentplaylist, "None");
	strcpy(currentsong, "None");
	strcpy(requestedplaylist, "None");
	requestedtime = -1;
	songindex = -1;
}

void SgSetModVolume(int value)
{
	if (!SgInitialised)
		return;

	playerVolume = value;
	Mix_VolumeMusic(((uint64_t)musicVol()));
}

void SgPlaylist_Create(const char* name)
{
	const auto playlist = new SgPlaylist();
	playlist->SetName(name);
	playlists.PutData(playlist);
}

SgPlaylist* SgPlaylist_GetPlaylist(const char* name)
{

	for (int32_t rbx = 0; rbx < playlists.Size(); rbx++)
	{
		const auto playlist = playlists.GetData(rbx);
		if (playlist != nullptr && strcmp(playlist->GetName(), name) == 0)
			return playlist;
	}

	return nullptr;
}

void SgPlaylist_AddSong(char const* playlistName, char const* songName)
{
	const auto rax = SgPlaylist_GetPlaylist(playlistName);
	if (rax == nullptr)
	{
		printf("SgPlaylist_AddSong : Failed because playlist %s does not exist\n", playlistName);
		return;
	}

	rax->AddSong(songName);
}
