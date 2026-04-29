#include "Enemy.h"

#include "Constants.h"
#include "PlayerSoldier.h"

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
	target = 0;
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

	updateAI();
	Soldier::update(deltaTime);
	if (!usingSprite)
	{
		body.setFillColor(sf::Color(210, 70, 70));
	}
}

void Enemy::updateAI()
{
	stopMoving();

	if (target == 0 || !target->isActive())
	{
		return;
	}

	float distanceX = target->getCenterX() - getCenterX();

	if (distanceX > stopDistance)
	{
		moveRight();
	}
	else if (distanceX < -stopDistance)
	{
		moveLeft();
	}
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
