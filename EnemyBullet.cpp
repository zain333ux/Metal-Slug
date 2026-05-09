#include "EnemyBullet.h"

#include "Constants.h"

static sf::Texture enemyPistolBulletTexture;
static bool enemyPistolBulletTextureLoaded = false;

static void applyEnemyPistolBulletTexture(sf::RectangleShape& body)
{
	if (!enemyPistolBulletTextureLoaded)
	{
		sf::Image image;
		if (image.loadFromFile("Sprites/Clean/Pistol_bullet.png"))
		{
			image.createMaskFromColor(sf::Color::White);
			enemyPistolBulletTextureLoaded = enemyPistolBulletTexture.loadFromImage(image);
		}
	}

	if (enemyPistolBulletTextureLoaded)
	{
		body.setTexture(&enemyPistolBulletTexture);
		body.setFillColor(sf::Color::White);
	}
}

EnemyBullet::EnemyBullet(float startX, float startY, bool facingRight)
{
	damage = 8;
	lifeTime = 2.0f;
	width = 20.0f;
	height = 20.0f;
	setPlayerOwned(false);
	setPosition(startX, startY);

	if (facingRight)
	{
		setVelocity(Constants::ENEMY_BULLET_SPEED, 0.0f);
	}
	else
	{
		setVelocity(-Constants::ENEMY_BULLET_SPEED, 0.0f);
	}

	body.setSize(sf::Vector2f(width, height));
	body.setOutlineThickness(0.0f);
	body.setFillColor(sf::Color(255, 80, 60));
	applyEnemyPistolBulletTexture(body);
}

void EnemyBullet::update(float deltaTime)
{
	Projectile::update(deltaTime);

	if (x + width < 0.0f || x > 25000.0f)
	{
		deactivate();
	}
}
