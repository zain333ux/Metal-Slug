#include "GrenadeProjectile.h"

#include "AudioManager.h"
#include "Constants.h"


using namespace std;
using namespace sf;

static const IntRect GRENADE_FRAMES[] =
{
	IntRect(0, 0, 18, 27), IntRect(18, 0, 17, 27), IntRect(35, 0, 18, 27), IntRect(53, 0, 20, 27),
	IntRect(73, 0, 22, 27), IntRect(95, 0, 23, 27), IntRect(118, 0, 24, 27), IntRect(142, 0, 24, 27),
	IntRect(166, 0, 23, 27), IntRect(189, 0, 24, 27), IntRect(213, 0, 23, 27), IntRect(236, 0, 22, 27),
	IntRect(258, 0, 22, 27), IntRect(280, 0, 20, 27), IntRect(300, 0, 18, 27), IntRect(318, 0, 19, 27)
};

static const IntRect GRENADE_EXPLOSION_FRAMES[] =
{
	IntRect(0, 0, 32, 37), IntRect(32, 0, 27, 37), IntRect(59, 0, 24, 37),
	IntRect(83, 0, 26, 37), IntRect(109, 0, 27, 37), IntRect(136, 0, 31, 37),
	IntRect(167, 0, 31, 37), IntRect(198, 0, 35, 37), IntRect(233, 0, 37, 37),
	IntRect(270, 0, 37, 37), IntRect(307, 0, 38, 37), IntRect(345, 0, 37, 37),
	IntRect(382, 0, 39, 37), IntRect(421, 0, 33, 37), IntRect(454, 0, 34, 37)
};

static Texture grenadeTexture;
static bool grenadeTextureLoaded = false;
static Texture grenadeExplosionTexture;
static bool grenadeExplosionTextureLoaded = false;

static void applyGrenadeTexture(RectangleShape& body)
{
	if (!grenadeTextureLoaded)
	{
		Image image;
		if (image.loadFromFile("Sprites/Clean/grenade.png"))
		{
			image.createMaskFromColor(Color::White);
			grenadeTextureLoaded = grenadeTexture.loadFromImage(image);
		}
	}

	if (grenadeTextureLoaded)
	{
		body.setTexture(&grenadeTexture);
		body.setTextureRect(GRENADE_FRAMES[0]);
		body.setFillColor(Color::White);
	}
}

static bool loadGrenadeExplosionTexture()
{
	if (!grenadeExplosionTextureLoaded)
	{
		Image image;
		if (image.loadFromFile("Sprites/Clean/Explosion.png"))
		{
			image.createMaskFromColor(Color::White);
			grenadeExplosionTextureLoaded = grenadeExplosionTexture.loadFromImage(image);
		}
	}

	return grenadeExplosionTextureLoaded;
}

GrenadeProjectile::GrenadeProjectile(float startX, float startY, bool facingRight, bool airborne)
{
	damage = 20;
	lifeTime = 2.4f;
	width = 42;
	height = 42;
	explosive = true;
	blastRadius = 180;
	exploded = false;
	currentFrame = 0;
	frameTimer = 0;
	explosionCenterX = startX;
	explosionCenterY = startY;
	markFiredWhileAirborne(airborne);
	setPosition(startX, startY);

	if (facingRight)
	{
		setVelocity(450, -620);
	}
	else
	{
		setVelocity(-450, -620);
	}

	body.setSize(Vector2f(width, height));
	body.setOutlineThickness(0);
	body.setFillColor(Color(80, 220, 90));
	applyGrenadeTexture(body);

	if (loadGrenadeExplosionTexture())
	{
		explosionSprite.setTexture(grenadeExplosionTexture);
		explosionSprite.setTextureRect(GRENADE_EXPLOSION_FRAMES[0]);
		explosionSprite.setScale(3.4f, 3.4f);
	}
}

void GrenadeProjectile::update(float deltaTime)
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
			explosionSprite.setTextureRect(GRENADE_EXPLOSION_FRAMES[currentFrame]);
			centerExplosionSprite();
		}
		body.setPosition(x, y);
		return;
	}

	velocityY += Constants::GRAVITY * 0.8f * deltaTime;
	frameTimer += deltaTime;
	if (frameTimer >= 0.055f)
	{
		frameTimer = 0;
		currentFrame += 1;
		if (currentFrame >= 16)
		{
			currentFrame = 0;
		}
		body.setTextureRect(GRENADE_FRAMES[currentFrame]);
	}

	Projectile::update(deltaTime);

	float groundY = getGroundY();
	if (y + height >= groundY)
	{
		y = groundY - height;
		explode();
	}
}

void GrenadeProjectile::explode()
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

	if (grenadeExplosionTextureLoaded)
	{
		explosionSprite.setTextureRect(GRENADE_EXPLOSION_FRAMES[0]);
		centerExplosionSprite();
	}
}

void GrenadeProjectile::draw(RenderWindow& window)
{
	if (!visible)
	{
		return;
	}

	if (exploded && grenadeExplosionTextureLoaded)
	{
		window.draw(explosionSprite);
		return;
	}

	Projectile::draw(window);
}

void GrenadeProjectile::onCollision()
{
	explode();
	damage = 0;
}

void GrenadeProjectile::centerExplosionSprite()
{
	FloatRect bounds = explosionSprite.getGlobalBounds();
	explosionSprite.setPosition(explosionCenterX - bounds.width / 2, explosionCenterY - bounds.height / 2);
}