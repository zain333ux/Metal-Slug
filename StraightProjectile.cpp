#include "StraightProjectile.h"

#include "Constants.h"

static sf::Texture pistolBulletTexture;
static bool pistolBulletTextureLoaded = false;

static void applyPistolBulletTexture(sf::RectangleShape& body)
{
	if (!pistolBulletTextureLoaded)
	{
		sf::Image image;
		if (image.loadFromFile("Sprites/Clean/Pistol_bullet.png"))
		{
			image.createMaskFromColor(sf::Color::White);
			pistolBulletTextureLoaded = pistolBulletTexture.loadFromImage(image);
		}
	}

	if (pistolBulletTextureLoaded)
	{
		body.setTexture(&pistolBulletTexture);
		body.setFillColor(sf::Color::White);
	}
}

StraightProjectile::StraightProjectile(float startX, float startY, bool facingRight)
	: StraightProjectile(startX, startY, facingRight, false)
{
}

StraightProjectile::StraightProjectile(float startX, float startY, bool facingRight, bool upward)
{
	width = 20.0f;
	height = 20.0f;
	body.setSize(sf::Vector2f(width, height));
	body.setOutlineThickness(0.0f);
	applyPistolBulletTexture(body);
	setPosition(startX, startY);

	if (upward)
	{
		setVelocity(0.0f, -Constants::PLAYER_BULLET_SPEED);
	}
	else if (facingRight)
	{
		setVelocity(Constants::PLAYER_BULLET_SPEED, 0.0f);
	}
	else
	{
		setVelocity(-Constants::PLAYER_BULLET_SPEED, 0.0f);
	}
}

void StraightProjectile::update(float deltaTime)
{
	Projectile::update(deltaTime);

	if (x + width < 0.0f || x > 25000.0f || y + height < 0.0f)
	{
		deactivate();
	}
}
