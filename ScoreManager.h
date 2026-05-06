#pragma once

class ScoreManager
{
private:
	int score;

public:
	ScoreManager();

	void reset();
	void add(int amount);
	void addSurvivalClear();
	void addCampaignClear();
	int getScore() const;

	static int meleeBonus();
	static int aerialBonus();
	static int multiKillBonus(int killCount);
};
