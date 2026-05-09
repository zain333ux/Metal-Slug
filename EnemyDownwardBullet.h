#pragma once

#include "Projectile.h"

class Level;

class EnemyDownwardBullet : public Projectile
{
private:
	Level* activeLevel;

public:
	EnemyDownwardBullet(float startX, float startY, Level* level);

	void update(float deltaTime);
};
