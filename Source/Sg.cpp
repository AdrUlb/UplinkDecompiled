#include <Sg.hpp>

#include <BTree.hpp>
#include <Eclipse.hpp>
#include <LList.hpp>
#include <RedShirt.hpp>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

static bool SgInitialised = false;

static char requestedplaylist[0x40];
static char currentplaylist[0x40];
static char currentsong[0x100];
static int requestedtime = 0;
static int songindex = 0;
static int playerVolume = 64;

static BTree<Mix_Chunk*> cache;

static LList<SgPlaylist*> playlists;

static Mix_Music* currentmod;

SgPlaylist::~SgPlaylist()
{
	for (int i = 0; i < _songs.Size(); i++)
	{
		const auto song = _songs.GetData(i);

		if (song == nullptr || song[0] == 0)
			continue;

		delete[] song;
	}
}

const char* SgPlaylist::GetName()
{
	return _name;
}

LList<char*>& SgPlaylist::GetSongs()
{
	return _songs;
}

void SgPlaylist::SetName(const char* const value)
{
	strcpy(_name, value);
}

void SgPlaylist::AddSong(const char* const name)
{
	const auto str = new char[strlen(name) + 1];
	sprintf(str, name);
	_songs.PutData(str);
}

static int musicVol()
{
	return playerVolume / 20.0 * 85.333333333333329;
}

static void SgStopMod()
{
	if (!SgInitialised || currentmod == nullptr)
		return;

	Mix_HaltMusic();
	Mix_FreeMusic(currentmod);
	currentmod = nullptr;
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

void SgShutdown()
{
	if (!SgInitialised)
		return;
	SgStopMod();
	Mix_HaltChannel(-1);
	Mix_CloseAudio();
}

void SgPlayMod(const char* path)
{
	if (!SgInitialised)
		return;

	SgStopMod();
	currentmod = Mix_LoadMUS(path);
	if (currentmod != nullptr)
	{
		Mix_VolumeMusic(musicVol());
		Mix_PlayMusic(currentmod, 0);
	}
}

static bool SgModFinished()
{
	if (!SgInitialised)
		return false;

	if (currentmod == nullptr)
		return true;

	return !Mix_PlayingMusic();
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
	for (int i = 0; i < playlists.Size(); i++)
	{
		const auto playlist = playlists.GetData(i);
		if (playlist != nullptr && strcmp(playlist->GetName(), name) == 0)
			return playlist;
	}

	return nullptr;
}

void SgPlaylist_AddSong(char const* playlistName, char const* songName)
{
	const auto playlist = SgPlaylist_GetPlaylist(playlistName);
	if (playlist == nullptr)
	{
		printf("SgPlaylist_AddSong : Failed because playlist %s does not exist\n", playlistName);
		return;
	}

	playlist->AddSong(songName);
}

void SgPlaylist_NextSong()
{
	const auto playlist = SgPlaylist_GetPlaylist(currentplaylist);

	if (playlist == nullptr)
	{
		printf("SgPlaylist_NextSong : Failed because playlist %s does not exist\n", currentplaylist);
		return;
	}

	songindex++;

	if (songindex >= playlist->GetSongs().Size())
		songindex = 0;

	const auto filePath = playlist->GetSongs().GetData(songindex);
	SgPlayMod(RsArchiveFileOpen(filePath));
	strcpy(currentsong, filePath);
}

void SgPlaylist_Play(const char* name)
{
	if (strcmp(currentplaylist, "None") == 0)
	{
		strcpy(currentplaylist, name);
		songindex = -1;
		SgPlaylist_NextSong();
		return;
	}

	if (strcmp(currentplaylist, name) == 0)
		return;

	if (strcmp(requestedplaylist, "None") == 0)
	{
		requestedtime = EclGetAccurateTime();
	}
	strcpy(requestedplaylist, name);
}

void SgUpdate()
{
	// No next playlist
	if (strcmp(requestedplaylist, "None") == 0)
	{
		// If we're playing a playlist and the current mod is finished, play the next
		if ((strcmp(currentplaylist, "None") != 0 && SgModFinished()))
			SgPlaylist_NextSong();

		return;
	}

	// Fade out the current mod
	auto fadeOutAmount = EclGetAccurateTime() - requestedtime;
	fadeOutAmount /= 4000.0f;
	fadeOutAmount *= 20.0f;
	if (20 - fadeOutAmount > 0)
	{
		SgSetModVolume(20 - fadeOutAmount);
		return;
	}

	// Play the requested playlist
	SgStopMod();
	strcpy(currentplaylist, requestedplaylist);
	strcpy(requestedplaylist, "None");

	requestedtime = -1;
	songindex = -1;

	SgSetModVolume(20);
	SgPlaylist_NextSong();
}

void SgPlaySound(const char* path, const char* name)
{
	if (!SgInitialised)
		return;

	auto theName = name;
	if (name == nullptr)
		theName = path;

	// If the file is already in the cache
	if (cache.LookupTree(theName) != nullptr)
	{
		// Get the chunk and play it
		const auto chunk = cache.GetData(theName);

		// If the chunk is null, remove it from the cache
		if (chunk == nullptr)
		{
			cache.RemoveData(theName);
			return;
		}

		// Play the chunk
		Mix_VolumeChunk(chunk, 32);
		Mix_PlayChannelTimed(-1, chunk, 0, -1);
		return;
	}

	// Create a new chunk, add it to the cache and play it
	const auto chunk = Mix_LoadWAV_RW(SDL_RWFromFile(path, "rb"), true);
	if (chunk != nullptr)
	{
		cache.PutData(theName, chunk);
		Mix_VolumeChunk(chunk, 32);
		Mix_PlayChannelTimed(-1, chunk, 0, -1);
	}
}
