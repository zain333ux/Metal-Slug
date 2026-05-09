#include "EnemyRocketProjectile.h"

#include "Constants.h"

static const sf::IntRect BAZOOKA_EXPLOSION_FRAMES[] =
{
	sf::IntRect(0, 0, 32, 37), sf::IntRect(32, 0, 27, 37), sf::IntRect(59, 0, 24, 37),
	sf::IntRect(83, 0, 26, 37), sf::IntRect(109, 0, 27, 37), sf::IntRect(136, 0, 31, 37),
	sf::IntRect(167, 0, 31, 37), sf::IntRect(198, 0, 35, 37), sf::IntRect(233, 0, 37, 37),
	sf::IntRect(270, 0, 37, 37), sf::IntRect(307, 0, 38, 37), sf::IntRect(345, 0, 37, 37),
	sf::IntRect(382, 0, 39, 37), sf::IntRect(421, 0, 33, 37), sf::IntRect(454, 0, 34, 37)
};

static sf::Texture enemyRocketTexture;
static bool enemyRocketTextureLoaded = false;
static sf::Texture bazookaExplosionTexture;
static bool bazookaExplosionTextureLoaded = false;

static void applyEnemyRocketTexture(sf::RectangleShape& body)
{
	if (!enemyRocketTextureLoaded)
	{
		sf::Image image;
		if (image.loadFromFile("Sprites/Clean/Bazooka_bullet.png"))
		{
			image.createMaskFromColor(sf::Color::White);
			enemyRocketTextureLoaded = enemyRocketTexture.loadFromImage(image);
		}
	}

	if (enemyRocketTextureLoaded)
	{
		body.setTexture(&enemyRocketTexture);
		body.setFillColor(sf::Color::White);
		body.setTextureRect(sf::IntRect(0, 0, 13, 14));
	}
}

static bool loadBazookaExplosionTexture()
{
	if (!bazookaExplosionTextureLoaded)
	{
		sf::Image image;
		if (image.loadFromFile("Sprites/Clean/Explosion.png"))
		{
			image.createMaskFromColor(sf::Color::White);
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
	width = 32.0f;
	height = 32.0f;
	explosive = true;
	blastRadius = 145.0f;
	exploded = false;
	armed = false;
	armTimer = 0.16f;
	explosionTimer = 0.18f;
	currentFrame = 0;
	frameTimer = 0.0f;
	explosionCenterX = startX;
	explosionCenterY = startY;
	setPlayerOwned(false);
	setPosition(startX, startY);
	setVelocity(throwVelocityX, throwVelocityY);

	body.setSize(sf::Vector2f(width, height));
	body.setOutlineThickness(0.0f);
	body.setFillColor(sf::Color(255, 120, 50));
	applyEnemyRocketTexture(body);
	body.setOutlineColor(sf::Color::Black);

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
			frameTimer = 0.0f;
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
		if (armTimer <= 0.0f)
		{
			armed = true;
			damage = storedDamage;
		}
	}

	lifeTime -= deltaTime;
	if (lifeTime <= 0.0f)
	{
		explode();
		return;
	}

	velocityY += Constants::GRAVITY * 0.62f * deltaTime;
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

	float groundY = getGroundY();
	if (y + height >= groundY)
	{
		y = groundY - height;
		explode();
	}
}

void EnemyRocketProjectile::draw(sf::RenderWindow& window)
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
	sf::FloatRect bounds = explosionSprite.getGlobalBounds();
	explosionSprite.setPosition(explosionCenterX - bounds.width / 2.0f, explosionCenterY - bounds.height / 2.0f);
}
