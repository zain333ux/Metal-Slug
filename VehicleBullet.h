#pragma once

#include "Projectile.h"

class VehicleBullet : public Projectile
{
private:
	static bool textureLoaded;
	static sf::Texture bulletTexture;
	sf::Sprite bulletSprite;
	bool usingSprite;
	bool facingRight;
	float spriteScale;

	static bool loadBulletTexture();
	void updateSpritePosition();

public:
	VehicleBullet(float startX, float startY, bool newFacingRight);

	void update(float deltaTime);
	void draw(sf::RenderWindow& window);
};
