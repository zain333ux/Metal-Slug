#include "EnemyGrenadeProjectile.h"

#include "AudioManager.h"
#include "Constants.h"


using namespace std;
using namespace sf;

static const IntRect ENEMY_GRENADE_FRAMES[] =
{
	IntRect(0, 0, 18, 27), IntRect(18, 0, 17, 27), IntRect(35, 0, 18, 27), IntRect(53, 0, 20, 27),
	IntRect(73, 0, 22, 27), IntRect(95, 0, 23, 27), IntRect(118, 0, 24, 27), IntRect(142, 0, 24, 27),
	IntRect(166, 0, 23, 27), IntRect(189, 0, 24, 27), IntRect(213, 0, 23, 27), IntRect(236, 0, 22, 27),
	IntRect(258, 0, 22, 27), IntRect(280, 0, 20, 27), IntRect(300, 0, 18, 27), IntRect(318, 0, 19, 27)
};

static const IntRect ENEMY_GRENADE_EXPLOSION_FRAMES[] =
{
	IntRect(0, 0, 32, 37), IntRect(32, 0, 27, 37), IntRect(59, 0, 24, 37),
	IntRect(83, 0, 26, 37), IntRect(109, 0, 27, 37), IntRect(136, 0, 31, 37),
	IntRect(167, 0, 31, 37), IntRect(198, 0, 35, 37), IntRect(233, 0, 37, 37),
	IntRect(270, 0, 37, 37), IntRect(307, 0, 38, 37), IntRect(345, 0, 37, 37),
	IntRect(382, 0, 39, 37), IntRect(421, 0, 33, 37), IntRect(454, 0, 34, 37)
};

static Texture enemyGrenadeTexture;
static bool enemyGrenadeTextureLoaded = false;
static Texture enemyGrenadeExplosionTexture;
static bool enemyGrenadeExplosionTextureLoaded = false;

static void applyEnemyGrenadeTexture(RectangleShape& body)
{
	if (!enemyGrenadeTextureLoaded)
	{
		Image image;
		if (image.loadFromFile("Sprites/Clean/grenade.png"))
		{
			image.createMaskFromColor(Color::White);
			enemyGrenadeTextureLoaded = enemyGrenadeTexture.loadFromImage(image);
		}
	}

	if (enemyGrenadeTextureLoaded)
	{
		body.setTexture(&enemyGrenadeTexture);
		body.setTextureRect(ENEMY_GRENADE_FRAMES[0]);
		body.setFillColor(Color::White);
	}
}

static bool loadEnemyGrenadeExplosionTexture()
{
	if (!enemyGrenadeExplosionTextureLoaded)
	{
		Image image;
		if (image.loadFromFile("Sprites/Clean/Explosion.png"))
		{
			image.createMaskFromColor(Color::White);
			enemyGrenadeExplosionTextureLoaded = enemyGrenadeExplosionTexture.loadFromImage(image);
		}
	}

	return enemyGrenadeExplosionTextureLoaded;
}

EnemyGrenadeProjectile::EnemyGrenadeProjectile(float startX, float startY, float throwVelocityX, float throwVelocityY)
{
	storedDamage = 18;
	damage = 0;
	lifeTime = 2.6f;
	width = 42;
	height = 42;
	explosive = true;
	blastRadius = 120;
	exploded = false;
	armed = false;
	armTimer = 0.16f;
	explosionTimer = 0;
	currentFrame = 0;
	frameTimer = 0;
	explosionCenterX = startX;
	explosionCenterY = startY;
	setPlayerOwned(false);
	setPosition(startX, startY);
	setVelocity(throwVelocityX, throwVelocityY);

	body.setSize(Vector2f(width, height));
	body.setOutlineThickness(0);
	body.setFillColor(Color(80, 220, 90));
	applyEnemyGrenadeTexture(body);

	if (loadEnemyGrenadeExplosionTexture())
	{
		explosionSprite.setTexture(enemyGrenadeExplosionTexture);
		explosionSprite.setTextureRect(ENEMY_GRENADE_EXPLOSION_FRAMES[0]);
		explosionSprite.setScale(3.4f, 3.4f);
	}
}

void EnemyGrenadeProjectile::explode()
{
	if (exploded)
	{
		return;
	}

	exploded = true;
	AudioManager::playGlobalSound(SFX_EXPLOSION);
	explosionTimer = 0;
	currentFrame = 0;
	frameTimer = 0;
	velocityX = 0;
	velocityY = 0;

	float centerX = getCenterX();
	float centerY = getCenterY();
	explosionCenterX = centerX;
	explosionCenterY = centerY;
	width = blastRadius * 2;
	height = blastRadius * 2;
	setPosition(centerX - blastRadius, centerY - blastRadius);
	body.setSize(Vector2f(width, height));
	body.setPosition(x, y);
	body.setTexture(0);
	body.setFillColor(Color::Transparent);

	if (enemyGrenadeExplosionTextureLoaded)
	{
		explosionSprite.setTextureRect(ENEMY_GRENADE_EXPLOSION_FRAMES[0]);
		centerExplosionSprite();
	}
}

void EnemyGrenadeProjectile::update(float deltaTime)
{
	if (exploded)
	{
		frameTimer += deltaTime;
		if (frameTimer >= 0.05f)
		{
			frameTimer = 0;
			currentFrame += 1;
			if (currentFrame >= 15)
			{
				deactivate();
				return;
			}
			explosionSprite.setTextureRect(ENEMY_GRENADE_EXPLOSION_FRAMES[currentFrame]);
			centerExplosionSprite();
		}
		body.setPosition(x, y);
		return;
	}

	if (!armed)
	{
		armTimer -= deltaTime;
		if (armTimer <= 0)
		{
			armed = true;
			damage = storedDamage;
		}
	}

	lifeTime -= deltaTime;
	if (lifeTime <= 0)
	{
		explode();
		return;
	}

	velocityY += Constants::GRAVITY * 0.75f * deltaTime;
	frameTimer += deltaTime;
	if (frameTimer >= 0.055f)
	{
		frameTimer = 0;
		currentFrame += 1;
		if (currentFrame >= 16)
		{
			currentFrame = 0;
		}
		body.setTextureRect(ENEMY_GRENADE_FRAMES[currentFrame]);
	}

	Entity::update(deltaTime);
	body.setPosition(x, y);

	float groundY = getGroundY();
	if (y + height >= groundY)
	{
		y = groundY - height;
		explode();
	}
}

void EnemyGrenadeProjectile::draw(RenderWindow& window)
{
	if (!visible)
	{
		return;
	}

	if (exploded && enemyGrenadeExplosionTextureLoaded)
	{
		window.draw(explosionSprite);
		return;
	}

	Projectile::draw(window);
}

void EnemyGrenadeProjectile::onCollision()
{
	if (!armed)
	{
		return;
	}

	explode();
	damage = 0;
}

void EnemyGrenadeProjectile::centerExplosionSprite()
{
	FloatRect bounds = explosionSprite.getGlobalBounds();
	explosionSprite.setPosition(explosionCenterX - bounds.width / 2, explosionCenterY - bounds.height / 2);
}