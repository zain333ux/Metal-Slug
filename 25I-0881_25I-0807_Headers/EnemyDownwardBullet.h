#pragma once

#include "StraightProjectile.h"

class Level;

class EnemyDownwardBullet : public StraightProjectile
{
private:
	Level* activeLevel;
	sf::Texture bulletTexture;
	sf::Sprite bulletSprite;
	bool spriteLoaded;
	float animationTimer;
	int currentFrame;
	static const int FRAME_COUNT = 4;

public:
	EnemyDownwardBullet(float startX, float startY, Level* level);

	void update(float deltaTime);
	void draw(sf::RenderWindow& window);
};
