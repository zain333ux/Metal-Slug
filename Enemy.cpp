#include "Enemy.h"

#include "Constants.h"
#include "EnemyBullet.h"
#include "Level.h"
#include "PlayerSoldier.h"

#include <cmath>

Enemy::Enemy()
{
	maxHealth = 30;
	health = maxHealth;
	scoreValue = 100;
	contactDamage = 10;
	moveSpeed = Constants::ENEMY_MOVE_SPEED;
	facingRight = false;
	grounded = false;
	movementMaxX = Constants::WORLD_WIDTH_LEVEL_3 + 2200.0f;
	activeLevel = 0;
	stopDistance = Constants::ENEMY_STOP_DISTANCE;
	contactDamageCooldown = 1.0f;
	contactDamageTimer = 0.0f;
	patrolLeft = 720.0f;
	patrolRight = 1080.0f;
	patrolDirection = -1;
	detectionRange = 520.0f;
	attackRange = 82.0f;
	shootingRange = 420.0f;
	pistolCooldown = 1.25f;
	pistolTimer = 0.4f;
	pistolEquipped = false;
	queuedShot = false;
	target = 0;
	fallbackColor = sf::Color(210, 70, 70);
	canMoveInAir = false;
	usingSprite = false;
	spriteFacesLeft = true;
	spriteScale = 2.1f;
	frameWidth = Constants::PLAYER_FRAME_SIZE;
	frameHeight = Constants::PLAYER_FRAME_SIZE;
	animationRow = 0;
	animationStartFrame = 0;
	animationFrameCount = 1;
	currentAnimationFrame = 0;
	animationTimer = 0.0f;
	animationFrameDuration = 0.15f;
	width = 52.0f;
	height = 96.0f;
	setPosition(900.0f, 500.0f);
	body.setSize(sf::Vector2f(width, height));
	body.setOutlineColor(sf::Color::Black);
	body.setOutlineThickness(2.0f);
	body.setFillColor(sf::Color(210, 70, 70));
	setSpriteScale(2.1f);
	if (loadSpriteSheet("Sprites/Clean/enemy_rebel_sheet.png"))
	{
		setSpriteFrame(0, 0, 96, 96);
	}
}

void Enemy::setSpawnPosition(float newX, float newY)
{
	setPosition(newX, newY);
	patrolLeft = newX - 180.0f;
	patrolRight = newX + 180.0f;
	if (patrolLeft < 0.0f)
	{
		patrolLeft = 0.0f;
	}
}

void Enemy::setTarget(PlayerSoldier* newTarget)
{
	target = newTarget;
}

void Enemy::update(float deltaTime)
{
	if (contactDamageTimer > 0.0f)
	{
		contactDamageTimer -= deltaTime;
	}
	if (pistolTimer > 0.0f)
	{
		pistolTimer -= deltaTime;
	}

	updateAI();
	velocityY += Constants::GRAVITY * deltaTime;

	float previousBottom = y + height;
	Entity::update(deltaTime);

	float landingY = static_cast<float>(Constants::GROUND_Y);
	if (activeLevel != 0)
	{
		landingY = activeLevel->getLandingY(x, x + width, previousBottom, y + height);
	}

	if (y + height >= landingY)
	{
		y = landingY - height;
		velocityY = 0.0f;
		grounded = true;
	}
	else
	{
		grounded = false;
	}

	if (x < 0.0f)
	{
		x = 0.0f;
	}
	if (x + width > movementMaxX)
	{
		x = movementMaxX - width;
	}

	updateVisualPosition();
	updateAnimation(deltaTime);
	if (!usingSprite)
	{
		body.setFillColor(fallbackColor);
	}
}

void Enemy::draw(sf::RenderWindow& window)
{
	if (!visible)
	{
		return;
	}

	if (usingSprite)
	{
		window.draw(sprite);
	}
	else
	{
		window.draw(body);
	}

	if (maxHealth > 0)
	{
		float healthRatio = static_cast<float>(health) / static_cast<float>(maxHealth);
		if (healthRatio < 0.0f)
		{
			healthRatio = 0.0f;
		}

		sf::RectangleShape healthBack;
		healthBack.setPosition(x, y - 12.0f);
		healthBack.setSize(sf::Vector2f(width, 6.0f));
		healthBack.setFillColor(sf::Color(80, 20, 20));
		window.draw(healthBack);

		sf::RectangleShape healthFront;
		healthFront.setPosition(x, y - 12.0f);
		healthFront.setSize(sf::Vector2f(width * healthRatio, 6.0f));
		healthFront.setFillColor(sf::Color(60, 220, 90));
		window.draw(healthFront);
	}
}

void Enemy::updateAI()
{
	stopMoving();

	if (target == 0 || !target->isActive())
	{
		return;
	}

	if (!grounded && !canMoveInAir)
	{
		return;
	}

	float distanceX = target->getCenterX() - getCenterX();
	float distanceY = target->getCenterY() - getCenterY();
	float absoluteDistanceX = std::abs(distanceX);
	bool canSeePlayer = absoluteDistanceX <= detectionRange && std::abs(distanceY) < 150.0f;

	if (canSeePlayer)
	{
		if (distanceX > 0.0f)
		{
			facingRight = true;
		}
		else
		{
			facingRight = false;
		}

		if (pistolEquipped && absoluteDistanceX <= shootingRange && pistolTimer <= 0.0f)
		{
			queuedShot = true;
			pistolTimer = pistolCooldown;
		}

		if (absoluteDistanceX <= attackRange || (pistolEquipped && absoluteDistanceX <= shootingRange))
		{
			stopMoving();
			return;
		}

		if (distanceX > stopDistance)
		{
			moveRight();
		}
		else if (distanceX < -stopDistance)
		{
			moveLeft();
		}
		return;
	}

	if (x <= patrolLeft)
	{
		patrolDirection = 1;
	}
	else if (x + width >= patrolRight)
	{
		patrolDirection = -1;
	}

	if (patrolDirection > 0)
	{
		moveRight();
	}
	else
	{
		moveLeft();
	}
}

Projectile* Enemy::createProjectileIfReady()
{
	if (!queuedShot)
	{
		return 0;
	}

	queuedShot = false;
	float bulletX = facingRight ? x + width : x - 18.0f;
	float bulletY = y + 42.0f;
	return new EnemyBullet(bulletX, bulletY, facingRight);
}

void Enemy::moveLeft()
{
	velocityX = -moveSpeed;
	facingRight = false;
}

void Enemy::moveRight()
{
	velocityX = moveSpeed;
	facingRight = true;
}

void Enemy::stopMoving()
{
	velocityX = 0.0f;
}

bool Enemy::loadSpriteSheet(const char* fileName)
{
	if (!texture.loadFromFile(fileName))
	{
		usingSprite = false;
		return false;
	}

	sprite.setTexture(texture);
	sprite.setScale(spriteScale, spriteScale);
	usingSprite = true;
	updateVisualPosition();
	return true;
}

bool Enemy::loadMaskedTexture(sf::Texture& targetTexture, const char* fileName)
{
	sf::Image image;
	if (!image.loadFromFile(fileName))
	{
		return false;
	}

	image.createMaskFromColor(sf::Color::White);
	return targetTexture.loadFromImage(image);
}

void Enemy::setSpriteFrame(int left, int top, int newFrameWidth, int newFrameHeight)
{
	frameWidth = newFrameWidth;
	frameHeight = newFrameHeight;
	sprite.setTextureRect(sf::IntRect(left, top, frameWidth, frameHeight));
	updateVisualPosition();
}

void Enemy::setSpriteScale(float scale)
{
	if (scale <= 0.0f)
	{
		return;
	}

	spriteScale = scale;
	sprite.setScale(spriteScale, spriteScale);
	updateVisualPosition();
}

void Enemy::playAnimation(int row, int frameCount, float frameDuration)
{
	playAnimation(row, 0, frameCount, frameDuration);
}

void Enemy::playAnimation(int row, int startFrame, int frameCount, float frameDuration)
{
	if (frameCount < 1)
	{
		frameCount = 1;
	}
	if (frameDuration <= 0.0f)
	{
		frameDuration = 0.15f;
	}
	if (startFrame < 0)
	{
		startFrame = 0;
	}
	if (animationRow == row && animationStartFrame == startFrame && animationFrameCount == frameCount)
	{
		animationFrameDuration = frameDuration;
		return;
	}

	animationRow = row;
	animationStartFrame = startFrame;
	animationFrameCount = frameCount;
	currentAnimationFrame = 0;
	animationTimer = 0.0f;
	animationFrameDuration = frameDuration;
	setSpriteFrame(animationStartFrame * frameWidth, animationRow * frameHeight, frameWidth, frameHeight);
}

void Enemy::updateVisualPosition()
{
	body.setPosition(x, y);

	if (!usingSprite)
	{
		return;
	}

	bool drawFlipped = !facingRight;
	if (spriteFacesLeft)
	{
		drawFlipped = facingRight;
	}

	if (drawFlipped)
	{
		sprite.setScale(-spriteScale, spriteScale);
	}
	else
	{
		sprite.setScale(spriteScale, spriteScale);
	}

	sf::FloatRect spriteBounds = sprite.getGlobalBounds();
	float spriteX = x + width / 2.0f - spriteBounds.width / 2.0f;
	if (drawFlipped)
	{
		spriteX = x + width / 2.0f + spriteBounds.width / 2.0f;
	}
	float spriteY = y + height - spriteBounds.height;
	sprite.setPosition(spriteX, spriteY);
}

void Enemy::updateAnimation(float deltaTime)
{
	if (!usingSprite || animationFrameCount <= 1)
	{
		return;
	}

	animationTimer += deltaTime;
	if (animationTimer >= animationFrameDuration)
	{
		animationTimer = 0.0f;
		currentAnimationFrame += 1;
		if (currentAnimationFrame >= animationFrameCount)
		{
			currentAnimationFrame = 0;
		}
		setSpriteFrame((animationStartFrame + currentAnimationFrame) * frameWidth, animationRow * frameHeight, frameWidth, frameHeight);
	}
}

void Enemy::setMovementMaxX(float maxX)
{
	if (maxX > width)
	{
		movementMaxX = maxX;
	}
}

void Enemy::setActiveLevel(Level* level)
{
	activeLevel = level;
}

bool Enemy::applyProjectileHit(Projectile& projectile)
{
	takeDamage(projectile.getDamage());
	return true;
}

int Enemy::getScoreValue() const
{
	return scoreValue;
}

int Enemy::getContactDamage() const
{
	return contactDamage;
}

bool Enemy::canDealContactDamage() const
{
	return contactDamageTimer <= 0.0f;
}

void Enemy::restartContactDamageCooldown()
{
	contactDamageTimer = contactDamageCooldown;
}

const char* Enemy::getEnemyName() const
{
	return "Rebel Soldier";
}
