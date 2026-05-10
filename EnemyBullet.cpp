#include "EnemyBullet.h"

#include "Constants.h"


using namespace std;
using namespace sf;

static Texture enemyPistolBulletTexture;
static bool enemyPistolBulletTextureLoaded = false;

static void applyEnemyPistolBulletTexture(RectangleShape& body)
{
	if (!enemyPistolBulletTextureLoaded)
	{
		Image image;
		if (image.loadFromFile("Sprites/Clean/Pistol_bullet.png"))
		{
			image.createMaskFromColor(Color::White);
			enemyPistolBulletTextureLoaded = enemyPistolBulletTexture.loadFromImage(image);
		}
	}

	if (enemyPistolBulletTextureLoaded)
	{
		body.setTexture(&enemyPistolBulletTexture);
		body.setFillColor(Color::White);
	}
}

EnemyBullet::EnemyBullet(float startX, float startY, bool facingRight)
{
	damage = 8;
	lifeTime = 2;
	width = 20;
	height = 20;
	setPlayerOwned(false);
	setPosition(startX, startY);

	if (facingRight)
	{
		setVelocity(Constants::ENEMY_BULLET_SPEED, 0);
	}
	else
	{
		setVelocity(-Constants::ENEMY_BULLET_SPEED, 0);
	}

	body.setSize(Vector2f(width, height));
	body.setOutlineThickness(0);
	body.setFillColor(Color(255, 80, 60));
	applyEnemyPistolBulletTexture(body);
}

void EnemyBullet::update(float deltaTime)
{
	Projectile::update(deltaTime);

	if (x + width < 0 || x > 25000)
	{
		deactivate();
	}
}