#include "EnemyGrenadeProjectile.h"

#include "Constants.h"

static const sf::IntRect ENEMY_GRENADE_FRAMES[] =
{
	sf::IntRect(0, 0, 18, 27), sf::IntRect(18, 0, 17, 27), sf::IntRect(35, 0, 18, 27), sf::IntRect(53, 0, 20, 27),
	sf::IntRect(73, 0, 22, 27), sf::IntRect(95, 0, 23, 27), sf::IntRect(118, 0, 24, 27), sf::IntRect(142, 0, 24, 27),
	sf::IntRect(166, 0, 23, 27), sf::IntRect(189, 0, 24, 27), sf::IntRect(213, 0, 23, 27), sf::IntRect(236, 0, 22, 27),
	sf::IntRect(258, 0, 22, 27), sf::IntRect(280, 0, 20, 27), sf::IntRect(300, 0, 18, 27), sf::IntRect(318, 0, 19, 27)
};

static const sf::IntRect ENEMY_GRENADE_EXPLOSION_FRAMES[] =
{
	sf::IntRect(0, 0, 32, 37), sf::IntRect(32, 0, 27, 37), sf::IntRect(59, 0, 24, 37),
	sf::IntRect(83, 0, 26, 37), sf::IntRect(109, 0, 27, 37), sf::IntRect(136, 0, 31, 37),
	sf::IntRect(167, 0, 31, 37), sf::IntRect(198, 0, 35, 37), sf::IntRect(233, 0, 37, 37),
	sf::IntRect(270, 0, 37, 37), sf::IntRect(307, 0, 38, 37), sf::IntRect(345, 0, 37, 37),
	sf::IntRect(382, 0, 39, 37), sf::IntRect(421, 0, 33, 37), sf::IntRect(454, 0, 34, 37)
};

static sf::Texture enemyGrenadeTexture;
static bool enemyGrenadeTextureLoaded = false;
static sf::Texture enemyGrenadeExplosionTexture;
static bool enemyGrenadeExplosionTextureLoaded = false;

static void applyEnemyGrenadeTexture(sf::RectangleShape& body)
{
	if (!enemyGrenadeTextureLoaded)
	{
		sf::Image image;
		if (image.loadFromFile("Sprites/Clean/grenade.png"))
		{
			image.createMaskFromColor(sf::Color::White);
			enemyGrenadeTextureLoaded = enemyGrenadeTexture.loadFromImage(image);
		}
	}

	if (enemyGrenadeTextureLoaded)
	{
		body.setTexture(&enemyGrenadeTexture);
		body.setTextureRect(ENEMY_GRENADE_FRAMES[0]);
		body.setFillColor(sf::Color::White);
	}
}

static bool loadEnemyGrenadeExplosionTexture()
{
	if (!enemyGrenadeExplosionTextureLoaded)
	{
		sf::Image image;
		if (image.loadFromFile("Sprites/Clean/Explosion.png"))
		{
			image.createMaskFromColor(sf::Color::White);
			enemyGrenadeExplosionTextureLoaded = enemyGrenadeExplosionTexture.loadFromImage(image);
		}
	}

	return enemyGrenadeExplosionTextureLoaded;
}

EnemyGrenadeProjectile::EnemyGrenadeProjectile(float startX, float startY, float throwVelocityX, float throwVelocityY)
{
	damage = 18;
	lifeTime = 2.6f;
	width = 42.0f;
	height = 42.0f;
	explosive = true;
	blastRadius = 120.0f;
	exploded = false;
	explosionTimer = 0.0f;
	currentFrame = 0;
	frameTimer = 0.0f;
	explosionCenterX = startX;
	explosionCenterY = startY;
	setPlayerOwned(false);
	setPosition(startX, startY);
	setVelocity(throwVelocityX, throwVelocityY);

	body.setSize(sf::Vector2f(width, height));
	body.setOutlineThickness(0.0f);
	body.setFillColor(sf::Color(80, 220, 90));
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
	explosionTimer = 0.0f;
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
			frameTimer = 0.0f;
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

	lifeTime -= deltaTime;
	if (lifeTime <= 0.0f)
	{
		explode();
		return;
	}

	velocityY += Constants::GRAVITY * 0.75f * deltaTime;
	frameTimer += deltaTime;
	if (frameTimer >= 0.055f)
	{
		frameTimer = 0.0f;
		currentFrame += 1;
		if (currentFrame >= 16)
		{
			currentFrame = 0;
		}
		body.setTextureRect(ENEMY_GRENADE_FRAMES[currentFrame]);
	}

	Entity::update(deltaTime);
	body.setPosition(x, y);

	if (y + height >= Constants::GROUND_Y)
	{
		y = Constants::GROUND_Y - height;
		explode();
	}
}

void EnemyGrenadeProjectile::draw(sf::RenderWindow& window)
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
	explode();
	damage = 0;
}

void EnemyGrenadeProjectile::centerExplosionSprite()
{
	sf::FloatRect bounds = explosionSprite.getGlobalBounds();
	explosionSprite.setPosition(explosionCenterX - bounds.width / 2.0f, explosionCenterY - bounds.height / 2.0f);
}
