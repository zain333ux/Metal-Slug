#include "EnemyTypes.h"

#include "Constants.h"
#include "EnemyGrenadeProjectile.h"
#include "PlayerSoldier.h"
#include "Projectile.h"

#include <cmath>

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
	maxHealth = 5;
	health = maxHealth;
	scoreValue = 75;
	contactDamage = 12;
	moveSpeed = 70.0f;
	stopDistance = 210.0f;
	detectionRange = 560.0f;
	shootingRange = 430.0f;
	attackRange = 90.0f;
	pistolCooldown = 1.5f;
	pistolTimer = 0.8f;
	pistolEquipped = true;
	shieldState = 0;
	shieldAnimationState = -1;
	hurtTimer = 0.0f;
	shootTimer = 0.0f;
	shotReleaseTimer = 0.0f;
	fallbackColor = sf::Color(90, 130, 210);
	width = 58.0f;
	height = 96.0f;
	body.setSize(sf::Vector2f(width, height));
	setSpriteScale(2.2f);

	if (loadMaskedTexture(idleTexture, "Sprites/Clean/ShieldedSoldier_idle.png") &&
		loadMaskedTexture(runTexture, "Sprites/Clean/ShieldedSoldier_run.png") &&
		loadMaskedTexture(fireTexture, "Sprites/Clean/ShieldedSoldier_fire.png"))
	{
		usingSprite = true;
		setShieldAnimation(0);
	}
}

bool ShieldedSoldier::isHitFromFront(const Projectile& projectile) const
{
	if (facingRight)
	{
		return projectile.getCenterX() >= getCenterX();
	}

	return projectile.getCenterX() <= getCenterX();
}

bool ShieldedSoldier::isHitFromAbove(const Projectile& projectile) const
{
	return projectile.getCenterY() < y + height * 0.35f;
}

bool ShieldedSoldier::applyProjectileHit(Projectile& projectile)
{
	bool bypassShield = projectile.isExplosive() || projectile.isMelee() || isHitFromAbove(projectile);
	bool blockedByShield = !bypassShield && isHitFromFront(projectile);

	if (blockedByShield)
	{
		return false;
	}

	DamageableEntity::takeDamage(projectile.getDamage());
	hurtTimer = 0.25f;
	shieldState = 3;
	return true;
}

Projectile* ShieldedSoldier::createProjectileIfReady()
{
	if (queuedShot && shootTimer <= 0.0f && shotReleaseTimer <= 0.0f)
	{
		shootTimer = 0.60f;
		shotReleaseTimer = 0.22f;
		shieldState = 2;
		return 0;
	}

	if (queuedShot && shotReleaseTimer > 0.0f)
	{
		return 0;
	}

	Projectile* projectile = Enemy::createProjectileIfReady();
	if (projectile != 0)
	{
		shieldState = 2;
	}

	return projectile;
}

void ShieldedSoldier::update(float deltaTime)
{
	if (hurtTimer > 0.0f)
	{
		hurtTimer -= deltaTime;
	}
	if (shootTimer > 0.0f)
	{
		shootTimer -= deltaTime;
	}
	if (shotReleaseTimer > 0.0f)
	{
		shotReleaseTimer -= deltaTime;
	}

	Enemy::update(deltaTime);
	updateShieldAnimation();
}

void ShieldedSoldier::updateShieldAnimation()
{
	// Simple integer state: 0 idle, 1 walk, 2 shoot, 3 hurt.
	if (hurtTimer > 0.0f)
	{
		shieldState = 3;
	}
	else if (shootTimer > 0.0f)
	{
		shieldState = 2;
	}
	else if (velocityX != 0.0f)
	{
		shieldState = 1;
	}
	else
	{
		shieldState = 0;
	}

	if (shieldState == 1)
	{
		setShieldAnimation(1);
	}
	else if (shieldState == 2)
	{
		setShieldAnimation(2);
	}
	else
	{
		setShieldAnimation(0);
	}

	if (hurtTimer > 0.0f)
	{
		body.setFillColor(sf::Color(255, 80, 80));
		sprite.setColor(sf::Color(255, 120, 120));
	}
	else
	{
		body.setFillColor(fallbackColor);
		sprite.setColor(sf::Color::White);
	}
}

void ShieldedSoldier::setShieldAnimation(int newState)
{
	if (shieldAnimationState == newState)
	{
		return;
	}

	shieldAnimationState = newState;

	if (newState == 1)
	{
		sprite.setTexture(runTexture, true);
		setSpriteFrame(0, 0, 30, 41);
		playAnimation(0, 0, 12, 0.11f);
	}
	else if (newState == 2)
	{
		sprite.setTexture(fireTexture, true);
		setSpriteFrame(0, 0, 38, 42);
		playAnimation(0, 0, 10, 0.06f);
	}
	else
	{
		sprite.setTexture(idleTexture, true);
		setSpriteFrame(0, 0, 35, 39);
		playAnimation(0, 0, 6, 0.14f);
	}
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
	maxHealth = 2;
	health = maxHealth;
	scoreValue = 100;
	contactDamage = 10;
	moveSpeed = 62.0f;
	stopDistance = 250.0f;
	detectionRange = 620.0f;
	shootingRange = 500.0f;
	attackRange = 95.0f;
	pistolEquipped = false;
	queuedShot = false;
	grenadeState = 0;
	grenadeAnimationState = -1;
	hurtTimer = 0.0f;
	fireTimer = 0.0f;
	throwReleaseTimer = 0.0f;
	grenadeTimer = 0.8f;
	grenadeCooldown = 2.1f;
	queuedGrenade = false;
	fallbackColor = sf::Color(120, 210, 90);
	width = 52.0f;
	height = 96.0f;
	body.setSize(sf::Vector2f(width, height));
	setSpriteScale(2.3f);

	if (loadMaskedTexture(idleTexture, "Sprites/Clean/GrenadeSoldier_idle.png") &&
		loadMaskedTexture(runTexture, "Sprites/Clean/GrenadeSoldier_run.png") &&
		loadMaskedTexture(fireTexture, "Sprites/Clean/GrenadeSoldier_fire.png"))
	{
		usingSprite = true;
		setGrenadeAnimation(0);
	}
}

void GrenadeSoldier::updateAI()
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
	bool canSeePlayer = absoluteDistanceX <= detectionRange && std::abs(distanceY) < 180.0f;

	if (canSeePlayer)
	{
		facingRight = distanceX > 0.0f;

		// The grenade soldier plants his feet before throwing, so the arc is easy to read.
		if (absoluteDistanceX <= shootingRange)
		{
			if (grenadeTimer <= 0.0f && fireTimer <= 0.0f)
			{
				queuedGrenade = true;
				grenadeTimer = grenadeCooldown;
				fireTimer = 0.72f;
				throwReleaseTimer = 0.32f;
			}
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

Projectile* GrenadeSoldier::createProjectileIfReady()
{
	if (!queuedGrenade)
	{
		return 0;
	}
	if (throwReleaseTimer > 0.0f)
	{
		return 0;
	}

	queuedGrenade = false;

	float throwVelocityX = facingRight ? 330.0f : -330.0f;
	if (target != 0)
	{
		float distanceX = target->getCenterX() - getCenterX();
		throwVelocityX = distanceX * 0.9f;
		if (throwVelocityX > 430.0f)
		{
			throwVelocityX = 430.0f;
		}
		if (throwVelocityX < -430.0f)
		{
			throwVelocityX = -430.0f;
		}
		if (throwVelocityX > 0.0f && throwVelocityX < 230.0f)
		{
			throwVelocityX = 230.0f;
		}
		if (throwVelocityX < 0.0f && throwVelocityX > -230.0f)
		{
			throwVelocityX = -230.0f;
		}
	}

	float grenadeX = facingRight ? x + width - 8.0f : x - 10.0f;
	float grenadeY = y + 34.0f;
	return new EnemyGrenadeProjectile(grenadeX, grenadeY, throwVelocityX, -570.0f);
}

bool GrenadeSoldier::applyProjectileHit(Projectile& projectile)
{
	DamageableEntity::takeDamage(projectile.getDamage());
	hurtTimer = 0.25f;
	return true;
}

void GrenadeSoldier::update(float deltaTime)
{
	if (hurtTimer > 0.0f)
	{
		hurtTimer -= deltaTime;
	}
	if (fireTimer > 0.0f)
	{
		fireTimer -= deltaTime;
	}
	if (throwReleaseTimer > 0.0f)
	{
		throwReleaseTimer -= deltaTime;
	}
	if (grenadeTimer > 0.0f)
	{
		grenadeTimer -= deltaTime;
	}

	Enemy::update(deltaTime);
	updateGrenadeAnimation();
}

void GrenadeSoldier::updateGrenadeAnimation()
{
	// Simple integer state: 0 idle, 1 run, 2 fire.
	if (fireTimer > 0.0f)
	{
		grenadeState = 2;
	}
	else if (velocityX != 0.0f)
	{
		grenadeState = 1;
	}
	else
	{
		grenadeState = 0;
	}

	setGrenadeAnimation(grenadeState);

	if (hurtTimer > 0.0f)
	{
		body.setFillColor(sf::Color(255, 80, 80));
		sprite.setColor(sf::Color(255, 120, 120));
	}
	else
	{
		body.setFillColor(fallbackColor);
		sprite.setColor(sf::Color::White);
	}
}

void GrenadeSoldier::setGrenadeAnimation(int newState)
{
	if (grenadeAnimationState == newState)
	{
		return;
	}

	grenadeAnimationState = newState;

	if (newState == 1)
	{
		sprite.setTexture(runTexture, true);
		setSpriteFrame(0, 0, 26, 40);
		playAnimation(0, 0, 12, 0.10f);
	}
	else if (newState == 2)
	{
		sprite.setTexture(fireTexture, true);
		setSpriteFrame(0, 0, 43, 42);
		playAnimation(0, 0, 16, 0.045f);
	}
	else
	{
		sprite.setTexture(idleTexture, true);
		setSpriteFrame(0, 0, 28, 38);
		playAnimation(0, 0, 4, 0.14f);
	}
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
