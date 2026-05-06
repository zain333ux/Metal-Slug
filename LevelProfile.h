#pragma once

class LevelProfile
{
public:
	virtual ~LevelProfile();
	virtual const char* getName() const = 0;
	virtual float getWorldWidth() const = 0;
	virtual int getDifficulty() const = 0;
};

class PlainsProfile : public LevelProfile
{
public:
	const char* getName() const;
	float getWorldWidth() const;
	int getDifficulty() const;
};

class MountainProfile : public LevelProfile
{
public:
	const char* getName() const;
	float getWorldWidth() const;
	int getDifficulty() const;
};

class AquaticProfile : public LevelProfile
{
public:
	const char* getName() const;
	float getWorldWidth() const;
	int getDifficulty() const;
};

class LevelProfileFactory
{
public:
	static LevelProfile* createProfile(int option);
};
