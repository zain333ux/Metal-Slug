#include "LevelProfile.h"


using namespace std;

LevelProfile::~LevelProfile()
{
}

const char* PlainsProfile::getName() const
{
	return "Campaign Plains";
}

float PlainsProfile::getWorldWidth() const
{
	return 5200;
}

int PlainsProfile::getDifficulty() const
{
	return 1;
}

const char* MountainProfile::getName() const
{
	return "Campaign Mountains";
}

float MountainProfile::getWorldWidth() const
{
	return 6200;
}

int MountainProfile::getDifficulty() const
{
	return 2;
}

const char* AquaticProfile::getName() const
{
	return "Campaign Aquatic";
}

float AquaticProfile::getWorldWidth() const
{
	return 7000;
}

int AquaticProfile::getDifficulty() const
{
	return 3;
}

LevelProfile* LevelProfileFactory::createProfile(int option)
{
	if (option == 1)
	{
		return new MountainProfile();
	}
	if (option == 2)
	{
		return new AquaticProfile();
	}

	return new PlainsProfile();
}