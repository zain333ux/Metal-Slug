#include "GrenadeProjectile.h"

#include "Constants.h"

static const sf::IntRect GRENADE_FRAMES[] =
{
	sf::IntRect(0, 0, 18, 27), sf::IntRect(18, 0, 17, 27), sf::IntRect(35, 0, 18, 27), sf::IntRect(53, 0, 20, 27),
	sf::IntRect(73, 0, 22, 27), sf::IntRect(95, 0, 23, 27), sf::IntRect(118, 0, 24, 27), sf::IntRect(142, 0, 24, 27),
	sf::IntRect(166, 0, 23, 27), sf::IntRect(189, 0, 24, 27), sf::IntRect(213, 0, 23, 27), sf::IntRect(236, 0, 22, 27),
	sf::IntRect(258, 0, 22, 27), sf::IntRect(280, 0, 20, 27), sf::IntRect(300, 0, 18, 27), sf::IntRect(318, 0, 19, 27)
};

static const sf::IntRect GRENADE_EXPLOSION_FRAMES[] =
{
	sf::IntRect(0, 0, 32, 37), sf::IntRect(32, 0, 27, 37), sf::IntRect(59, 0, 24, 37),
	sf::IntRect(83, 0, 26, 37), sf::IntRect(109, 0, 27, 37), sf::IntRect(136, 0, 31, 37),
	sf::IntRect(167, 0, 31, 37), sf::IntRect(198, 0, 35, 37), sf::IntRect(233, 0, 37, 37),
	sf::IntRect(270, 0, 37, 37), sf::IntRect(307, 0, 38, 37), sf::IntRect(345, 0, 37, 37),
	sf::IntRect(382, 0, 39, 37), sf::IntRect(421, 0, 33, 37), sf::IntRect(454, 0, 34, 37)
};

static sf::Texture grenadeTexture;
static bool grenadeTextureLoaded = false;
static sf::Texture grenadeExplosionTexture;
static bool grenadeExplosionTextureLoaded = false;

static void applyGrenadeTexture(sf::RectangleShape& body)
{
	if (!grenadeTextureLoaded)
	{
		sf::Image image;
		if (image.loadFromFile("Sprites/Clean/grenade.png"))
		{
			image.createMaskFromColor(sf::Color::White);
			grenadeTextureLoaded = grenadeTexture.loadFromImage(image);
		}
	}

	if (grenadeTextureLoaded)
	{
		body.setTexture(&grenadeTexture);
		body.setTextureRect(GRENADE_FRAMES[0]);
		body.setFillColor(sf::Color::White);
	}
}

static bool loadGrenadeExplosionTexture()
{
	if (!grenadeExplosionTextureLoaded)
	{
		sf::Image image;
		if (image.loadFromFile("Sprites/Clean/Explosion.png"))
		{
			image.createMaskFromColor(sf::Color::White);
			grenadeExplosionTextureLoaded = grenadeExplosionTexture.loadFromImage(image);
		}
	}

	return grenadeExplosionTextureLoaded;
}

GrenadeProjectile::GrenadeProjectile(float startX, float startY, bool facingRight, bool airborne)
{
	damage = 20;
	lifeTime = 2.4f;
	width = 42.0f;
	height = 42.0f;
	explosive = true;
	blastRadius = 180.0f;
	exploded = false;
	currentFrame = 0;
	frameTimer = 0.0f;
	explosionCenterX = startX;
	explosionCenterY = startY;
	markFiredWhileAirborne(airborne);
	setPosition(startX, startY);

	if (facingRight)
	{
		setVelocity(450.0f, -620.0f);
	}
	else
	{
		setVelocity(-450.0f, -620.0f);
	}

	body.setSize(sf::Vector2f(width, height));
	body.setOutlineThickness(0.0f);
	body.setFillColor(sf::Color(80, 220, 90));
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
			frameTimer = 0.0f;
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
		frameTimer = 0.0f;
		currentFrame += 1;
		if (currentFrame >= 16)
		{
			currentFrame = 0;
		}
		body.setTextureRect(GRENADE_FRAMES[currentFrame]);
	}

	Projectile::update(deltaTime);

	if (y + height >= Constants::GROUND_Y)
	{
		y = Constants::GROUND_Y - height;
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
	currentFrame = 0;
	frameTimer = 0.0f;
	velocityX = 0.0f;
	velocityY = 0.0f;

	float centerX = getCenterX();
	float centerY = getCenterY();
	explosionCenterX = centerX;
	explosionCenterY = centerY;
	width = blastRadius * 2.0f;
	height = blastRadius * 2.0f;
	setPosition(centerX - blastRadius, centerY - blastRadius);
	body.setSize(sf::Vector2f(width, height));
	body.setPosition(x, y);
	body.setTexture(0);
	body.setFillColor(sf::Color::Transparent);

	if (grenadeExplosionTextureLoaded)
	{
		explosionSprite.setTextureRect(GRENADE_EXPLOSION_FRAMES[0]);
		centerExplosionSprite();
	}
}

void GrenadeProjectile::draw(sf::RenderWindow& window)
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
	sf::FloatRect bounds = explosionSprite.getGlobalBounds();
	explosionSprite.setPosition(explosionCenterX - bounds.width / 2.0f, explosionCenterY - bounds.height / 2.0f);
}
