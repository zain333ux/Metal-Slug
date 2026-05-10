#include "Enemy.h"

#include "Constants.h"
#include "EnemyBullet.h"
#include "Level.h"
#include "PlayerSoldier.h"

#include <cmath>


using namespace std;
using namespace sf;

Enemy::Enemy()
{
	maxHealth = 30;
	health = maxHealth;
	scoreValue = 100;
	contactDamage = 10;
	moveSpeed = Constants::ENEMY_MOVE_SPEED;
	facingRight = false;
	grounded = false;
	movementMaxX = Constants::WORLD_WIDTH_LEVEL_3 + 2200;
	activeLevel = 0;
	stopDistance = Constants::ENEMY_STOP_DISTANCE;
	contactDamageCooldown = 1;
	contactDamageTimer = 0;
	patrolLeft = 720;
	patrolRight = 1080;
	patrolDirection = -1;
	detectionRange = 520;
	attackRange = 82;
	shootingRange = 420;
	pistolCooldown = 1.25f;
	pistolTimer = 0.4f;
	pistolEquipped = false;
	queuedShot = false;
	target = 0;
	fallbackColor = Color(210, 70, 70);
	canMoveInAir = false;
	inWater = false;
	usingSprite = false;
	spriteFacesLeft = true;
	spriteScale = 2.1f;
	frameWidth = Constants::PLAYER_FRAME_SIZE;
	frameHeight = Constants::PLAYER_FRAME_SIZE;
	animationRow = 0;
	animationStartFrame = 0;
	animationFrameCount = 1;
	currentAnimationFrame = 0;
	animationTimer = 0;
	animationFrameDuration = 0.15f;
	deathProcessed = false;
	width = 52;
	height = 96;
	setPosition(900, 500);
	body.setSize(Vector2f(width, height));
	body.setOutlineColor(Color::Black);
	body.setOutlineThickness(2);
	body.setFillColor(Color(210, 70, 70));
	setSpriteScale(2.1f);
}

void Enemy::setSpawnPosition(float newX, float newY)
{
	setPosition(newX, newY);
	patrolLeft = newX - 180;
	patrolRight = newX + 180;
	if (patrolLeft < 0)
	{
		patrolLeft = 0;
	}
}

void Enemy::setTarget(PlayerSoldier* newTarget)
{
	target = newTarget;
}

void Enemy::update(float deltaTime)
{
	if (contactDamageTimer > 0)
	{
		contactDamageTimer -= deltaTime;
	}
	if (pistolTimer > 0)
	{
		pistolTimer -= deltaTime;
	}

	if (activeLevel != 0)
	{
		inWater = activeLevel->isWaterInBounds(x + 4, x + width - 4, y + height * 0.45f, y + height + 10);
	}

	updateAI();
	if (activeLevel != 0 && grounded && velocityX != 0)
	{
		float lookAheadX = velocityX > 0 ? x + width + 14 : x - 14;
		float nextGroundY = activeLevel->getGroundYAt(lookAheadX);
		float currentBottom = y + height;
		if (nextGroundY > currentBottom + 34)
		{
			velocityX = 0;
			patrolDirection *= -1;
		}
	}

	bool wasInWater = false;
	if (activeLevel != 0)
	{
		wasInWater = inWater;
	}

	float gravity = Constants::GRAVITY;
	if (wasInWater)
	{
		gravity *= 0.28f;
		velocityX *= 0.75f;
	}
	velocityY += gravity * deltaTime;
	if (wasInWater && velocityY > 150)
	{
		velocityY = 150;
	}

	float previousBottom = y + height;
	Entity::update(deltaTime);

	float landingY = static_cast<float>(Constants::GROUND_Y);
	if (activeLevel != 0)
	{
		landingY = activeLevel->getLandingY(x, x + width, previousBottom, y + height);
	}

	inWater = activeLevel != 0 &&
		activeLevel->isWaterInBounds(x + 4, x + width - 4, y + height * 0.45f, y + height + 10);

	if (y + height >= landingY - 8 && velocityY >= 0)
	{
		y = landingY - height;
		velocityY = 0;
		grounded = true;
	}
	else if (y + height >= landingY)
	{
		y = landingY - height;
		if (velocityY > 0)
		{
			velocityY = 0;
		}
		grounded = true;
	}
	else
	{
		grounded = false;
	}

	if (activeLevel != 0 && y + height > activeLevel->getWorldHeight())
	{
		y = activeLevel->getWorldHeight() - height;
		velocityY = 0;
		grounded = true;
	}

	if (y < 0)
	{
		y = 0;
		if (velocityY < 0)
		{
			velocityY = 0;
		}
	}

	if (x < 0)
	{
		x = 0;
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

void Enemy::draw(RenderWindow& window)
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
		if (healthRatio < 0)
		{
			healthRatio = 0;
		}

		RectangleShape healthBack;
		healthBack.setPosition(x, y - 12);
		healthBack.setSize(Vector2f(width, 6));
		healthBack.setFillColor(Color(80, 20, 20));
		window.draw(healthBack);

		RectangleShape healthFront;
		healthFront.setPosition(x, y - 12);
		healthFront.setSize(Vector2f(width * healthRatio, 6));
		healthFront.setFillColor(Color(60, 220, 90));
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

	if (!grounded && !canMoveInAir && !inWater)
	{
		return;
	}

	float distanceX = target->getCenterX() - getCenterX();
	float distanceY = target->getCenterY() - getCenterY();
	float absoluteDistanceX = abs(distanceX);
	bool canSeePlayer = absoluteDistanceX <= detectionRange && abs(distanceY) < 150;

	if (canSeePlayer)
	{
		if (distanceX > 0)
		{
			facingRight = true;
		}
		else
		{
			facingRight = false;
		}

		if (pistolEquipped && absoluteDistanceX <= shootingRange && pistolTimer <= 0)
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
	float bulletX = facingRight ? x + width : x - 18;
	float bulletY = y + 42;
	return new EnemyBullet(bulletX, bulletY, facingRight);
}

Entity* Enemy::createSpawnedEntityIfReady()
{
	return 0;
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
	velocityX = 0;
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

bool Enemy::loadMaskedTexture(Texture& targetTexture, const char* fileName)
{
	Image image;
	if (!image.loadFromFile(fileName))
	{
		return false;
	}

	image.createMaskFromColor(Color::White);
	return targetTexture.loadFromImage(image);
}

void Enemy::setSpriteFrame(int left, int top, int newFrameWidth, int newFrameHeight)
{
	frameWidth = newFrameWidth;
	frameHeight = newFrameHeight;
	sprite.setTextureRect(IntRect(left, top, frameWidth, frameHeight));
	updateVisualPosition();
}

void Enemy::setSpriteScale(float scale)
{
	if (scale <= 0)
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
	if (frameDuration <= 0)
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
	animationTimer = 0;
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

	FloatRect spriteBounds = sprite.getGlobalBounds();
	float spriteX = x + width / 2 - spriteBounds.width / 2;
	if (drawFlipped)
	{
		spriteX = x + width / 2 + spriteBounds.width / 2;
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
		animationTimer = 0;
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
	return contactDamageTimer <= 0;
}

void Enemy::restartContactDamageCooldown()
{
	contactDamageTimer = contactDamageCooldown;
}

const char* Enemy::getEnemyName() const
{
	return "Rebel Soldier";
}

bool Enemy::hasSpriteVisual() const
{
	return usingSprite;
}

bool Enemy::hasProcessedDeath() const
{
	return deathProcessed;
}

void Enemy::markDeathProcessed()
{
	deathProcessed = true;
}