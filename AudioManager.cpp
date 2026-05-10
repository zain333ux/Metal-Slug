#include "AudioManager.h"

#include <fstream>
#include <iostream>


using namespace std;
using namespace sf;

AudioManager* AudioManager::activeManager = 0;

AudioManager::AudioManager()
{
	musicVolume = 35;
	sfxVolume = 70;
	currentMusicTrack = MUSIC_NONE;

	for (int i = 0; i < 6; i += 1)
	{
		musicAvailable[i] = false;
		musicWarned[i] = false;
	}

	for (int i = 0; i < 5; i += 1)
	{
		sfxLoaded[i] = false;
		sfxWarned[i] = false;
	}

	activeManager = this;
}

void AudioManager::loadAll()
{
	for (int i = MUSIC_MENU; i <= MUSIC_BOSS; i += 1)
	{
		MusicTrack track = static_cast<MusicTrack>(i);
		const char* path = getMusicPath(track);
		musicAvailable[i] = fileExists(path);
		if (!musicAvailable[i] && !musicWarned[i])
		{
			warnMissingFile(path);
			musicWarned[i] = true;
		}
	}

	for (int i = SFX_BULLET_FIRE; i <= SFX_MENU_MOVE; i += 1)
	{
		SoundEffectType type = static_cast<SoundEffectType>(i);
		const char* path = getSfxPath(type);
		sfxLoaded[i] = sfxBuffers[i].loadFromFile(path);
		if (sfxLoaded[i])
		{
			sfxSounds[i].setBuffer(sfxBuffers[i]);
			sfxSounds[i].setVolume(sfxVolume);
		}
		else if (!sfxWarned[i])
		{
			warnMissingFile(path);
			sfxWarned[i] = true;
		}
	}
}

void AudioManager::playMusic(MusicTrack track)
{
	if (track == MUSIC_NONE || track == currentMusicTrack)
	{
		return;
	}

	int index = static_cast<int>(track);
	if (index < MUSIC_NONE || index > MUSIC_BOSS)
	{
		return;
	}

	const char* path = getMusicPath(track);
	if (!musicAvailable[index])
	{
		if (!musicWarned[index])
		{
			warnMissingFile(path);
			musicWarned[index] = true;
		}
		return;
	}

	if (!backgroundMusic.openFromFile(path))
	{
		if (!musicWarned[index])
		{
			warnMissingFile(path);
			musicWarned[index] = true;
		}
		return;
	}

	backgroundMusic.setLoop(true);
	backgroundMusic.setVolume(musicVolume);
	backgroundMusic.play();
	currentMusicTrack = track;
}

void AudioManager::playSound(SoundEffectType type)
{
	int index = static_cast<int>(type);
	if (index < SFX_BULLET_FIRE || index > SFX_MENU_MOVE)
	{
		return;
	}

	if (!sfxLoaded[index])
	{
		if (!sfxWarned[index])
		{
			warnMissingFile(getSfxPath(type));
			sfxWarned[index] = true;
		}
		return;
	}

	sfxSounds[index].setVolume(sfxVolume);
	sfxSounds[index].play();
}

void AudioManager::playGlobalSound(SoundEffectType type)
{
	if (activeManager != 0)
	{
		activeManager->playSound(type);
	}
}

const char* AudioManager::getMusicPath(MusicTrack track) const
{
	if (track == MUSIC_MENU)
	{
		return "Audio/menu_music.ogg";
	}
	if (track == MUSIC_LEVEL1)
	{
		return "Audio/level1.ogg";
	}
	if (track == MUSIC_LEVEL2)
	{
		return "Audio/level2.ogg";
	}
	if (track == MUSIC_LEVEL3)
	{
		return "Audio/level3.ogg";
	}
	if (track == MUSIC_BOSS)
	{
		return "Audio/boss.ogg";
	}
	return "";
}

const char* AudioManager::getSfxPath(SoundEffectType type) const
{
	if (type == SFX_BULLET_FIRE)
	{
		return "Audio/sfx/bulletfire.wav";
	}
	if (type == SFX_EXPLOSION)
	{
		return "Audio/sfx/explosion.wav";
	}
	if (type == SFX_COLLECT)
	{
		return "Audio/sfx/collect.wav";
	}
	if (type == SFX_MENU_SELECT)
	{
		return "Audio/menu_move.wav";
	}
	if (type == SFX_MENU_MOVE)
	{
		return "Audio/menu_move.wav";
	}
	return "";
}

bool AudioManager::fileExists(const char* path) const
{
	ifstream file(path);
	return file.good();
}

void AudioManager::warnMissingFile(const char* path) const
{
	cout << "[AudioManager] Warning: could not load audio file: " << path << endl;
}