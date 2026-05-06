#include "EnemyTypes.h"

#include "Constants.h"
#include "PlayerSoldier.h"

RebelSoldier::RebelSoldier()
{
	maxHealth = 2;
	health = maxHealth;
	scoreValue = 50;
	contactDamage = 6;
	moveSpeed = 95.0f;
	stopDistance = 185.0f;
	detectionRange = 560.0f;
	shootingRange = 430.0f;
	attackRange = 70.0f;
	pistolCooldown = 1.35f;
	pistolTimer = 0.6f;
	pistolEquipped = true;
	fallbackColor = sf::Color(210, 70, 70);
}

const char* RebelSoldier::getEnemyName() const
{
	return "Rebel Soldier";
}

ShieldedSoldier::ShieldedSoldier()
{
	maxHealth = 50;
	health = maxHealth;
	scoreValue = 75;
	contactDamage = 12;
	fallbackColor = sf::Color(90, 130, 210);
}

void ShieldedSoldier::takeDamage(int damage)
{
	DamageableEntity::takeDamage(damage / 2);
}

const char* ShieldedSoldier::getEnemyName() const
{
	return "Shielded Soldier";
}

BazookaSoldier::BazookaSoldier()
{
	maxHealth = 25;
	health = maxHealth;
	scoreValue = 100;
	contactDamage = 16;
	stopDistance = 180.0f;
	detectionRange = 680.0f;
	attackRange = 150.0f;
	fallbackColor = sf::Color(230, 150, 55);
}

const char* BazookaSoldier::getEnemyName() const
{
	return "Bazooka Soldier";
}

GrenadeSoldier::GrenadeSoldier()
{
	maxHealth = 25;
	health = maxHealth;
	scoreValue = 100;
	contactDamage = 14;
	stopDistance = 150.0f;
	detectionRange = 620.0f;
	attackRange = 130.0f;
	fallbackColor = sf::Color(120, 210, 90);
}

const char* GrenadeSoldier::getEnemyName() const
{
	return "Grenade Soldier";
}

ZombieEnemy::ZombieEnemy()
{
	maxHealth = 55;
	health = maxHealth;
	scoreValue = 100;
	moveSpeed = 75.0f;
	contactDamage = 18;
	detectionRange = 430.0f;
	attackRange = 80.0f;
	fallbackColor = sf::Color(130, 210, 140);
}

const char* ZombieEnemy::getEnemyName() const
{
	return "Zombie";
}

MummyEnemy::MummyEnemy()
{
	maxHealth = 70;
	health = maxHealth;
	scoreValue = 150;
	moveSpeed = 65.0f;
	contactDamage = 22;
	detectionRange = 390.0f;
	attackRange = 80.0f;
	fallbackColor = sf::Color(225, 215, 165);
}

const char* MummyEnemy::getEnemyName() const
{
	return "Mummy";
}

MartianEnemy::MartianEnemy()
{
	maxHealth = 35;
	health = maxHealth;
	scoreValue = 200;
	moveSpeed = 145.0f;
	contactDamage = 20;
	stopDistance = 120.0f;
	detectionRange = 780.0f;
	attackRange = 105.0f;
	fallbackColor = sf::Color(190, 90, 230);
	canMoveInAir = true;
	setPosition(900.0f, 360.0f);
}

void MartianEnemy::updateAI()
{
	Enemy::updateAI();
	if (target != 0 && target->isActive())
	{
		if (y > 430.0f)
		{
			velocityY = -80.0f;
		}
	}
}

const char* MartianEnemy::getEnemyName() const
{
	return "Martian";
}

BossEnemy::BossEnemy(int bossPhase)
{
	phase = bossPhase;
	maxHealth = 260;
	health = maxHealth;
	scoreValue = 500;
	width = 140.0f;
	height = 150.0f;
	moveSpeed = 95.0f;
	stopDistance = 220.0f;
	contactDamage = 25;
	detectionRange = 900.0f;
	attackRange = 145.0f;
	fallbackColor = sf::Color(180, 80, 255);
	body.setSize(sf::Vector2f(width, height));

	if (phase >= 4)
	{
		maxHealth = 500;
		health = maxHealth;
		scoreValue = 1500;
		fallbackColor = sf::Color(255, 80, 120);
	}
	if (phase == 2 || phase >= 4)
	{
		canMoveInAir = true;
	}
}

void BossEnemy::updateAI()
{
	Enemy::updateAI();
	if (phase == 2 && y > 420.0f)
	{
		velocityY = -70.0f;
	}
	else if (phase == 3 && y < Constants::GROUND_Y - height)
	{
		velocityY = 45.0f;
	}
}

const char* BossEnemy::getEnemyName() const
{
	if (phase >= 4)
	{
		return "Ultimate Boss";
	}

	return "Boss Phase";
}
