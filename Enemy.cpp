#include "Enemy.h"

#include "Constants.h"
#include "EnemyBullet.h"
#include "PlayerSoldier.h"

#include <cmath>

Enemy::Enemy()
{
	maxHealth = 30;
	health = maxHealth;
	scoreValue = 100;
	contactDamage = 10;
	moveSpeed = Constants::ENEMY_MOVE_SPEED;
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
	setPosition(900.0f, 500.0f);
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
	Soldier::update(deltaTime);
	if (!usingSprite)
	{
		body.setFillColor(fallbackColor);
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
