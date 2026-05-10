#pragma once

#include "BallisticProjectile.h"

class EnemyGrenadeProjectile : public BallisticProjectile
{
private:
	bool exploded;
	bool armed;
	float armTimer;
	int storedDamage;
	float explosionTimer;
	int currentFrame;
	float frameTimer;
	float explosionCenterX;
	float explosionCenterY;
	sf::Sprite explosionSprite;

	void explode();
	void centerExplosionSprite();

public:
	EnemyGrenadeProjectile(float startX, float startY, float throwVelocityX, float throwVelocityY);

	void update(float deltaTime);
	void draw(sf::RenderWindow& window);
	void onCollision();
};
