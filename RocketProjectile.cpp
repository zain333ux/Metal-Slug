#include "RocketProjectile.h"

#include "Constants.h"

static const sf::IntRect PLAYER_ROCKET_EXPLOSION_FRAMES[] =
{
	sf::IntRect(0, 0, 32, 37), sf::IntRect(32, 0, 27, 37), sf::IntRect(59, 0, 24, 37),
	sf::IntRect(83, 0, 26, 37), sf::IntRect(109, 0, 27, 37), sf::IntRect(136, 0, 31, 37),
	sf::IntRect(167, 0, 31, 37), sf::IntRect(198, 0, 35, 37), sf::IntRect(233, 0, 37, 37),
	sf::IntRect(270, 0, 37, 37), sf::IntRect(307, 0, 38, 37), sf::IntRect(345, 0, 37, 37),
	sf::IntRect(382, 0, 39, 37), sf::IntRect(421, 0, 33, 37), sf::IntRect(454, 0, 34, 37)
};

static sf::Texture playerRocketTexture;
static bool playerRocketTextureLoaded = false;
static sf::Texture playerRocketExplosionTexture;
static bool playerRocketExplosionTextureLoaded = false;

static void applyPlayerRocketTexture(sf::RectangleShape& body)
{
	if (!playerRocketTextureLoaded)
	{
		sf::Image image;
		if (image.loadFromFile("Sprites/Clean/Bazooka_bullet.png"))
		{
			image.createMaskFromColor(sf::Color::White);
			playerRocketTextureLoaded = playerRocketTexture.loadFromImage(image);
		}
	}

	if (playerRocketTextureLoaded)
	{
		body.setTexture(&playerRocketTexture);
		body.setFillColor(sf::Color::White);
		body.setTextureRect(sf::IntRect(0, 0, 13, 14));
	}
}

static bool loadPlayerRocketExplosionTexture()
{
	if (!playerRocketExplosionTextureLoaded)
	{
		sf::Image image;
		if (image.loadFromFile("Sprites/Clean/Explosion.png"))
		{
			image.createMaskFromColor(sf::Color::White);
			playerRocketExplosionTextureLoaded = playerRocketExplosionTexture.loadFromImage(image);
		}
	}

	return playerRocketExplosionTextureLoaded;
}

RocketProjectile::RocketProjectile(float startX, float startY, bool facingRight, bool airborne)
{
	damage = 35;
	lifeTime = 2.0f;
	width = 32.0f;
	height = 32.0f;
	explosive = true;
	blastRadius = 190.0f;
	exploded = false;
	currentFrame = 0;
	frameTimer = 0.0f;
	explosionCenterX = startX;
	explosionCenterY = startY;
	markFiredWhileAirborne(airborne);
	setPosition(startX, startY);

	if (facingRight)
	{
		setVelocity(650.0f, 0.0f);
	}
	else
	{
		setVelocity(-650.0f, 0.0f);
	}

	body.setSize(sf::Vector2f(width, height));
	body.setOutlineThickness(0.0f);
	body.setFillColor(sf::Color(255, 120, 50));
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
			explosionSprite.setTextureRect(PLAYER_ROCKET_EXPLOSION_FRAMES[currentFrame]);
			centerExplosionSprite();
		}
		body.setPosition(x, y);
		return;
	}

	lifeTime -= deltaTime;
	if (lifeTime <= 0.0f)
	{
		explode();
		return;
	}

	frameTimer += deltaTime;
	if (frameTimer >= 0.08f)
	{
		frameTimer = 0.0f;
		currentFrame += 1;
		if (currentFrame >= 4)
		{
			currentFrame = 0;
		}
		body.setTextureRect(sf::IntRect(currentFrame * 13, 0, 13, 14));
	}

	Entity::update(deltaTime);
	body.setPosition(x, y);

	if (x + width < 0.0f || x > Constants::WORLD_WIDTH_LEVEL_3 + 800.0f)
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

	if (playerRocketExplosionTextureLoaded)
	{
		explosionSprite.setTextureRect(PLAYER_ROCKET_EXPLOSION_FRAMES[0]);
		centerExplosionSprite();
	}
}

void RocketProjectile::draw(sf::RenderWindow& window)
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
	sf::FloatRect bounds = explosionSprite.getGlobalBounds();
	explosionSprite.setPosition(explosionCenterX - bounds.width / 2.0f, explosionCenterY - bounds.height / 2.0f);
}
