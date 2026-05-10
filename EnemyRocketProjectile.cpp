#include "EnemyRocketProjectile.h"

#include "AudioManager.h"
#include "Constants.h"


using namespace std;
using namespace sf;

static const IntRect BAZOOKA_EXPLOSION_FRAMES[] =
{
	IntRect(0, 0, 32, 37), IntRect(32, 0, 27, 37), IntRect(59, 0, 24, 37),
	IntRect(83, 0, 26, 37), IntRect(109, 0, 27, 37), IntRect(136, 0, 31, 37),
	IntRect(167, 0, 31, 37), IntRect(198, 0, 35, 37), IntRect(233, 0, 37, 37),
	IntRect(270, 0, 37, 37), IntRect(307, 0, 38, 37), IntRect(345, 0, 37, 37),
	IntRect(382, 0, 39, 37), IntRect(421, 0, 33, 37), IntRect(454, 0, 34, 37)
};

static Texture enemyRocketTexture;
static bool enemyRocketTextureLoaded = false;
static Texture bazookaExplosionTexture;
static bool bazookaExplosionTextureLoaded = false;

static void applyEnemyRocketTexture(RectangleShape& body)
{
	if (!enemyRocketTextureLoaded)
	{
		Image image;
		if (image.loadFromFile("25I-0881_25I-0807_Assets/Sprites/Clean/Bazooka_bullet.png"))
		{
			image.createMaskFromColor(Color::White);
			enemyRocketTextureLoaded = enemyRocketTexture.loadFromImage(image);
		}
	}

	if (enemyRocketTextureLoaded)
	{
		body.setTexture(&enemyRocketTexture);
		body.setFillColor(Color::White);
		body.setTextureRect(IntRect(0, 0, 13, 14));
	}
}

static bool loadBazookaExplosionTexture()
{
	if (!bazookaExplosionTextureLoaded)
	{
		Image image;
		if (image.loadFromFile("25I-0881_25I-0807_Assets/Sprites/Clean/Explosion.png"))
		{
			image.createMaskFromColor(Color::White);
			bazookaExplosionTextureLoaded = bazookaExplosionTexture.loadFromImage(image);
		}
	}

	return bazookaExplosionTextureLoaded;
}

EnemyRocketProjectile::EnemyRocketProjectile(float startX, float startY, float throwVelocityX, float throwVelocityY)
{
	storedDamage = 28;
	damage = 0;
	lifeTime = 2.8f;
	width = 32;
	height = 32;
	explosive = true;
	blastRadius = 145;
	exploded = false;
	armed = false;
	armTimer = 0.16f;
	explosionTimer = 0.18f;
	currentFrame = 0;
	frameTimer = 0;
	explosionCenterX = startX;
	explosionCenterY = startY;
	setPlayerOwned(false);
	setPosition(startX, startY);
	setVelocity(throwVelocityX, throwVelocityY);

	body.setSize(Vector2f(width, height));
	body.setOutlineThickness(0);
	body.setFillColor(Color(255, 120, 50));
	applyEnemyRocketTexture(body);
	body.setOutlineColor(Color::Black);

	if (loadBazookaExplosionTexture())
	{
		explosionSprite.setTexture(bazookaExplosionTexture);
		explosionSprite.setTextureRect(BAZOOKA_EXPLOSION_FRAMES[0]);
		explosionSprite.setScale(3.4f, 3.4f);
	}
}

void EnemyRocketProjectile::explode()
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

	if (bazookaExplosionTextureLoaded)
	{
		explosionSprite.setTextureRect(BAZOOKA_EXPLOSION_FRAMES[0]);
		centerExplosionSprite();
	}
}

void EnemyRocketProjectile::update(float deltaTime)
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
			explosionSprite.setTextureRect(BAZOOKA_EXPLOSION_FRAMES[currentFrame]);
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

	velocityY += Constants::GRAVITY * 0.62f * deltaTime;
	frameTimer += deltaTime;
	if (frameTimer >= 0.08f)
	{
		frameTimer = 0;
		currentFrame += 1;
		if (currentFrame >= 4)
		{
			currentFrame = 0;
		}
		body.setTextureRect(IntRect(currentFrame * 13, 0, 13, 14));
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

void EnemyRocketProjectile::draw(RenderWindow& window)
{
	if (!visible)
	{
		return;
	}

	if (exploded && bazookaExplosionTextureLoaded)
	{
		window.draw(explosionSprite);
		return;
	}

	Projectile::draw(window);
}

void EnemyRocketProjectile::onCollision()
{
	if (!armed)
	{
		return;
	}

	explode();
	damage = 0;
}

void EnemyRocketProjectile::centerExplosionSprite()
{
	FloatRect bounds = explosionSprite.getGlobalBounds();
	explosionSprite.setPosition(explosionCenterX - bounds.width / 2, explosionCenterY - bounds.height / 2);
}