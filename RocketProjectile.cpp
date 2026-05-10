#include "RocketProjectile.h"

#include "AudioManager.h"
#include "Constants.h"


using namespace std;
using namespace sf;

static const IntRect PLAYER_ROCKET_EXPLOSION_FRAMES[] =
{
	IntRect(0, 0, 32, 37), IntRect(32, 0, 27, 37), IntRect(59, 0, 24, 37),
	IntRect(83, 0, 26, 37), IntRect(109, 0, 27, 37), IntRect(136, 0, 31, 37),
	IntRect(167, 0, 31, 37), IntRect(198, 0, 35, 37), IntRect(233, 0, 37, 37),
	IntRect(270, 0, 37, 37), IntRect(307, 0, 38, 37), IntRect(345, 0, 37, 37),
	IntRect(382, 0, 39, 37), IntRect(421, 0, 33, 37), IntRect(454, 0, 34, 37)
};

static Texture playerRocketTexture;
static bool playerRocketTextureLoaded = false;
static Texture playerRocketExplosionTexture;
static bool playerRocketExplosionTextureLoaded = false;

static void applyPlayerRocketTexture(RectangleShape& body)
{
	if (!playerRocketTextureLoaded)
	{
		// texture is loaded once and reused by all player rockets
		Image image;
		if (image.loadFromFile("Sprites/Clean/Bazooka_bullet.png"))
		{
			image.createMaskFromColor(Color::White);
			playerRocketTextureLoaded = playerRocketTexture.loadFromImage(image);
		}
	}

	if (playerRocketTextureLoaded)
	{
		body.setTexture(&playerRocketTexture);
		body.setFillColor(Color::White);
		body.setTextureRect(IntRect(0, 0, 13, 14));
	}
}

static bool loadPlayerRocketExplosionTexture()
{
	if (!playerRocketExplosionTextureLoaded)
	{
		Image image;
		if (image.loadFromFile("Sprites/Clean/Explosion.png"))
		{
			image.createMaskFromColor(Color::White);
			playerRocketExplosionTextureLoaded = playerRocketExplosionTexture.loadFromImage(image);
		}
	}

	return playerRocketExplosionTextureLoaded;
}

RocketProjectile::RocketProjectile(float startX, float startY, bool facingRight, bool airborne)
{
	damage = 35;
	lifeTime = 2;
	width = 32;
	height = 32;
	explosive = true;
	blastRadius = 190;
	exploded = false;
	currentFrame = 0;
	frameTimer = 0;
	explosionCenterX = startX;
	explosionCenterY = startY;
	markFiredWhileAirborne(airborne);
	setPosition(startX, startY);

	if (facingRight)
	{
		setVelocity(650, 0);
	}
	else
	{
		setVelocity(-650, 0);
	}

	body.setSize(Vector2f(width, height));
	body.setOutlineThickness(0);
	body.setFillColor(Color(255, 120, 50));
	applyPlayerRocketTexture(body);

	if (loadPlayerRocketExplosionTexture())
	{
		explosionSprite.setTexture(playerRocketExplosionTexture);
		explosionSprite.setTextureRect(PLAYER_ROCKET_EXPLOSION_FRAMES[0]);
		explosionSprite.setScale(3.4f, 3.4f);
	}
}

void RocketProjectile::update(float deltaTime)
{
	if (exploded)
	{
		// explosion animation controls when rocket disappears
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
			explosionSprite.setTextureRect(PLAYER_ROCKET_EXPLOSION_FRAMES[currentFrame]);
			centerExplosionSprite();
		}
		body.setPosition(x, y);
		return;
	}

	lifeTime -= deltaTime;
	if (lifeTime <= 0)
	{
		explode();
		return;
	}

	frameTimer += deltaTime;
	if (frameTimer >= 0.08f)
	{
		// rocket sprite has small flame loop while flying
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

	if (x + width < 0 || x > Constants::WORLD_WIDTH_LEVEL_3 + 800)
	{
		explode();
	}
}

void RocketProjectile::explode()
{
	if (exploded)
	{
		return;
	}

	exploded = true;
	AudioManager::playGlobalSound(SFX_EXPLOSION);
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

	if (playerRocketExplosionTextureLoaded)
	{
		explosionSprite.setTextureRect(PLAYER_ROCKET_EXPLOSION_FRAMES[0]);
		centerExplosionSprite();
	}
}

void RocketProjectile::draw(RenderWindow& window)
{
	if (!visible)
	{
		return;
	}

	if (exploded && playerRocketExplosionTextureLoaded)
	{
		window.draw(explosionSprite);
		return;
	}

	Projectile::draw(window);
}

void RocketProjectile::onCollision()
{
	explode();
	damage = 0;
}

void RocketProjectile::centerExplosionSprite()
{
	FloatRect bounds = explosionSprite.getGlobalBounds();
	explosionSprite.setPosition(explosionCenterX - bounds.width / 2, explosionCenterY - bounds.height / 2);
}
