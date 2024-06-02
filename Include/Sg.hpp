#pragma once

#include <LList.hpp>

class SgPlaylist
{
	char _name[0x40]{"New playlist"};
	LList<char*> _songs;

public:
	~SgPlaylist();
	const char* GetName();
	LList<char*>& GetSongs();
	void SetName(const char* value);
	void AddSong(const char* name);
};

void SgInitialise();
void SgShutdown();
void SgPlaylist_Initialise();
SgPlaylist* SgPlaylist_GetPlaylist(const char* name);
void SgSetModVolume(int value);
void SgPlaylist_Create(const char* name);
void SgPlaylist_AddSong(char const* playlistName, char const* songName);
void SgPlaylist_Play(const char* name);
void SgUpdate();
void SgPlaySound(const char* path, const char* name);
