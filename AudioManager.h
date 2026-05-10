#pragma once

#include <SFML/Audio.hpp>

enum MusicTrack
{
	MUSIC_NONE,
	MUSIC_MENU,
	MUSIC_LEVEL1,
	MUSIC_LEVEL2,
	MUSIC_LEVEL3,
	MUSIC_BOSS
};

enum SoundEffectType
{
	SFX_BULLET_FIRE,
	SFX_EXPLOSION,
	SFX_COLLECT,
	SFX_MENU_SELECT,
	SFX_MENU_MOVE
};

class AudioManager
{
private:
	sf::Music backgroundMusic;
	sf::SoundBuffer sfxBuffers[5];
	sf::Sound sfxSounds[5];
	bool musicAvailable[6];
	bool musicWarned[6];
	bool sfxLoaded[5];
	bool sfxWarned[5];
	float musicVolume;
	float sfxVolume;
	MusicTrack currentMusicTrack;

	const char* getMusicPath(MusicTrack track) const;
	const char* getSfxPath(SoundEffectType type) const;
	bool fileExists(const char* path) const;
	void warnMissingFile(const char* path) const;

	static AudioManager* activeManager;

public:
	AudioManager();

	void loadAll();

	void playMusic(MusicTrack track);
	void playSound(SoundEffectType type);

	static void playGlobalSound(SoundEffectType type);
};
