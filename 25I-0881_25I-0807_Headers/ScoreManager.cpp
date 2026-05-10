#include "ScoreManager.h"


using namespace std;

ScoreManager::ScoreManager()
{
	score = 0;
}

void ScoreManager::reset()
{
	score = 0;
}

void ScoreManager::add(int amount)
{
	if (amount > 0)
	{
		score += amount;
	}
}

void ScoreManager::addSurvivalClear()
{
	score += 1000;
}

int ScoreManager::getScore() const
{
	return score;
}

int ScoreManager::meleeBonus()
{
	return 50;
}

int ScoreManager::aerialBonus()
{
	return 100;
}

int ScoreManager::multiKillBonus(int killCount)
{
	if (killCount >= 3)
	{
		return 300 + (killCount - 3) * 50;
	}

	if (killCount == 2)
	{
		return 200;
	}

	return 0;
}