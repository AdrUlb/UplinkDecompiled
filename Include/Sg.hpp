#pragma once

#include <LList.hpp>

class SgPlaylist
{
	char name[0x40]{"New playlist"};
	LList<char*> songs;

public:
	~SgPlaylist();

	const char* GetName()
	{
		return name;
	}

	void SetName(const char* value);
	void AddSong(const char* name);
};

void SgInitialise();
void SgPlaylist_Initialise();
void SgSetModVolume(int value);
void SgPlaylist_Create(const char* name);
void SgPlaylist_AddSong(char const* playlistName, char const* songName);
