#include "EnemyTypes.h"

#include "Constants.h"
#include "EnemyDownwardBullet.h"
#include "EnemyGrenadeProjectile.h"
#include "EnemyRocketProjectile.h"
#include "EnemyBullet.h"
#include "Level.h"
#include "PlayerSoldier.h"
#include "Projectile.h"

#include <cmath>

static const sf::IntRect REBEL_IDLE_FRAMES[] =
{
	sf::IntRect(0, 0, 43, 42), sf::IntRect(43, 0, 43, 42), sf::IntRect(86, 0, 43, 42), sf::IntRect(129, 0, 44, 42)
};

static const sf::IntRect REBEL_RUN_FRAMES[] =
{
	sf::IntRect(0, 0, 39, 42), sf::IntRect(39, 0, 40, 42), sf::IntRect(79, 0, 43, 42), sf::IntRect(122, 0, 45, 42),
	sf::IntRect(167, 0, 40, 42), sf::IntRect(207, 0, 39, 42), sf::IntRect(246, 0, 38, 42), sf::IntRect(284, 0, 40, 42),
	sf::IntRect(324, 0, 43, 42), sf::IntRect(367, 0, 41, 42), sf::IntRect(408, 0, 41, 42), sf::IntRect(449, 0, 39, 42)
};

static const sf::IntRect REBEL_FIRE_FRAMES[] =
{
	sf::IntRect(0, 0, 47, 51), sf::IntRect(47, 0, 51, 51), sf::IntRect(98, 0, 50, 51), sf::IntRect(148, 0, 45, 51),
	sf::IntRect(193, 0, 45, 51), sf::IntRect(238, 0, 43, 51), sf::IntRect(281, 0, 46, 51), sf::IntRect(327, 0, 50, 51),
	sf::IntRect(377, 0, 49, 51), sf::IntRect(426, 0, 46, 51)
};

static const sf::IntRect MARTIAN_POD_MOVE_FRAMES[] =
{
	sf::IntRect(0, 0, 48, 42), sf::IntRect(48, 0, 48, 42), sf::IntRect(96, 0, 48, 42), sf::IntRect(144, 0, 48, 42),
	sf::IntRect(192, 0, 48, 42), sf::IntRect(240, 0, 48, 42), sf::IntRect(288, 0, 48, 42), sf::IntRect(336, 0, 50, 42),
	sf::IntRect(0, 43, 48, 42), sf::IntRect(48, 43, 48, 42), sf::IntRect(96, 43, 48, 42), sf::IntRect(144, 43, 48, 42),
	sf::IntRect(192, 43, 48, 42), sf::IntRect(240, 43, 48, 42), sf::IntRect(288, 43, 48, 42), sf::IntRect(336, 43, 50, 42)
};

static const sf::IntRect MARTIAN_POD_DEATH_FRAMES[] =
{
	sf::IntRect(0, 0, 52, 62), sf::IntRect(52, 0, 52, 62), sf::IntRect(104, 0, 60, 62),
	sf::IntRect(164, 0, 62, 62), sf::IntRect(226, 0, 69, 62), sf::IntRect(295, 0, 91, 62),
	sf::IntRect(0, 62, 70, 62), sf::IntRect(70, 62, 58, 62), sf::IntRect(128, 62, 59, 62),
	sf::IntRect(187, 62, 50, 62), sf::IntRect(237, 62, 48, 62), sf::IntRect(285, 62, 48, 62),
	sf::IntRect(333, 62, 53, 62),
	sf::IntRect(0, 124, 51, 63), sf::IntRect(51, 124, 46, 63), sf::IntRect(97, 124, 46, 63),
	sf::IntRect(143, 124, 45, 63), sf::IntRect(188, 124, 40, 63), sf::IntRect(228, 124, 40, 63),
	sf::IntRect(268, 124, 42, 63), sf::IntRect(310, 124, 76, 63)
};

static const sf::IntRect MARTIAN_POD_ATTACK_FRAMES[] =
{
	sf::IntRect(308, 55, 44, 88), sf::IntRect(368, 55, 36, 88), sf::IntRect(420, 55, 28, 88),
	sf::IntRect(452, 43, 64, 112), sf::IntRect(528, 35, 88, 128), sf::IntRect(628, 27, 104, 144),
	sf::IntRect(744, 23, 112, 152), sf::IntRect(868, 23, 88, 152)
};

static const sf::IntRect MARTIAN_IDLE_FRAMES[] =
{
	sf::IntRect(0, 0, 52, 43), sf::IntRect(52, 0, 52, 43), sf::IntRect(104, 0, 52, 43), sf::IntRect(156, 0, 52, 43),
	sf::IntRect(208, 0, 52, 43), sf::IntRect(260, 0, 52, 43), sf::IntRect(312, 0, 52, 43), sf::IntRect(364, 0, 52, 43),
	sf::IntRect(416, 0, 52, 43), sf::IntRect(468, 0, 52, 43), sf::IntRect(520, 0, 52, 43), sf::IntRect(572, 0, 52, 43),
	sf::IntRect(624, 0, 52, 43), sf::IntRect(676, 0, 52, 43), sf::IntRect(728, 0, 52, 43), sf::IntRect(780, 0, 52, 43)
};

static const sf::IntRect MARTIAN_WALK_FRAMES[] =
{
	sf::IntRect(0, 0, 51, 43), sf::IntRect(51, 0, 51, 43), sf::IntRect(102, 0, 51, 43), sf::IntRect(153, 0, 51, 43),
	sf::IntRect(204, 0, 51, 43), sf::IntRect(255, 0, 51, 43), sf::IntRect(306, 0, 51, 43), sf::IntRect(357, 0, 51, 43),
	sf::IntRect(408, 0, 51, 43), sf::IntRect(459, 0, 51, 43), sf::IntRect(510, 0, 51, 43), sf::IntRect(561, 0, 51, 43),
	sf::IntRect(612, 0, 51, 43), sf::IntRect(663, 0, 51, 43), sf::IntRect(714, 0, 51, 43), sf::IntRect(765, 0, 51, 43)
};

static const sf::IntRect MARTIAN_FIRE_FRAMES[] =
{
	sf::IntRect(0, 0, 71, 43), sf::IntRect(71, 0, 71, 43), sf::IntRect(142, 0, 71, 43), sf::IntRect(213, 0, 71, 43),
	sf::IntRect(284, 0, 71, 43), sf::IntRect(355, 0, 71, 43), sf::IntRect(426, 0, 71, 43), sf::IntRect(497, 0, 71, 43),
	sf::IntRect(568, 0, 71, 43), sf::IntRect(639, 0, 71, 43), sf::IntRect(710, 0, 71, 43), sf::IntRect(781, 0, 71, 43),
	sf::IntRect(852, 0, 71, 43), sf::IntRect(923, 0, 71, 43), sf::IntRect(994, 0, 71, 43), sf::IntRect(1065, 0, 71, 43),
	sf::IntRect(1136, 0, 71, 43), sf::IntRect(1207, 0, 71, 43)
};

static const sf::IntRect MARTIAN_TELEPORT_FRAMES[] =
{
	sf::IntRect(0, 0, 66, 65), sf::IntRect(66, 0, 66, 65), sf::IntRect(132, 0, 66, 65), sf::IntRect(198, 0, 66, 65),
	sf::IntRect(264, 0, 66, 65), sf::IntRect(330, 0, 66, 65), sf::IntRect(396, 0, 66, 65), sf::IntRect(462, 0, 66, 65),
	sf::IntRect(528, 0, 66, 65), sf::IntRect(594, 0, 66, 65), sf::IntRect(660, 0, 66, 65), sf::IntRect(726, 0, 66, 65),
	sf::IntRect(792, 0, 66, 65), sf::IntRect(0, 66, 66, 65), sf::IntRect(66, 66, 66, 65)
};

static const sf::IntRect MARTIAN_DEATH_FRAMES[] =
{
	sf::IntRect(0, 0, 54, 43), sf::IntRect(54, 0, 54, 43), sf::IntRect(108, 0, 54, 43), sf::IntRect(162, 0, 54, 43),
	sf::IntRect(216, 0, 54, 43), sf::IntRect(270, 0, 54, 43), sf::IntRect(324, 0, 54, 43), sf::IntRect(378, 0, 54, 43),
	sf::IntRect(432, 0, 54, 43), sf::IntRect(486, 0, 54, 43), sf::IntRect(540, 0, 54, 43), sf::IntRect(594, 0, 54, 43),
	sf::IntRect(648, 0, 54, 43), sf::IntRect(702, 0, 54, 43), sf::IntRect(756, 0, 54, 43), sf::IntRect(810, 0, 48, 43),
	sf::IntRect(0, 43, 54, 43), sf::IntRect(54, 43, 54, 43), sf::IntRect(108, 43, 54, 43), sf::IntRect(162, 43, 54, 43),
	sf::IntRect(216, 43, 54, 43), sf::IntRect(270, 43, 54, 43), sf::IntRect(324, 43, 54, 43), sf::IntRect(378, 43, 54, 43),
	sf::IntRect(432, 43, 54, 43), sf::IntRect(486, 43, 54, 43), sf::IntRect(540, 43, 54, 43), sf::IntRect(594, 43, 54, 43),
	sf::IntRect(648, 43, 54, 43), sf::IntRect(702, 43, 54, 43), sf::IntRect(756, 43, 54, 43), sf::IntRect(810, 43, 48, 43),
	sf::IntRect(0, 86, 54, 43), sf::IntRect(54, 86, 54, 43), sf::IntRect(108, 86, 54, 43), sf::IntRect(162, 86, 54, 43),
	sf::IntRect(216, 86, 54, 43), sf::IntRect(270, 86, 54, 43), sf::IntRect(324, 86, 54, 43), sf::IntRect(378, 86, 54, 43),
	sf::IntRect(432, 86, 54, 43), sf::IntRect(486, 86, 54, 43), sf::IntRect(540, 86, 54, 43), sf::IntRect(594, 86, 54, 43),
	sf::IntRect(648, 86, 54, 43), sf::IntRect(702, 86, 54, 43), sf::IntRect(756, 86, 54, 43), sf::IntRect(810, 86, 48, 43)
};

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
	rebelState = 0;
	rebelAnimationState = -1;
	hurtTimer = 0.0f;
	shootTimer = 0.0f;
	shotReleaseTimer = 0.0f;
	queuedRebelShot = false;
	currentFrames = 0;
	customFrameCount = 0;
	customFrameIndex = 0;
	customFrameTimer = 0.0f;
	customFrameDuration = 0.12f;
	fallbackColor = sf::Color(210, 70, 70);
	width = 52.0f;
	height = 96.0f;
	body.setSize(sf::Vector2f(width, height));
	setSpriteScale(2.25f);
	usingSprite = false;

	if (loadMaskedTexture(idleTexture, "Sprites/Clean/RebelSoldier_idle.png") &&
		loadMaskedTexture(runTexture, "Sprites/Clean/RebelSoldier_run.png") &&
		loadMaskedTexture(fireTexture, "Sprites/Clean/RebelSoldier_fire.png"))
	{
		usingSprite = true;
		setRebelAnimation(0);
	}
}

void RebelSoldier::update(float deltaTime)
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
	updateRebelAnimation(deltaTime);
}

Projectile* RebelSoldier::createProjectileIfReady()
{
	if (queuedShot && !queuedRebelShot && shootTimer <= 0.0f && shotReleaseTimer <= 0.0f)
	{
		queuedRebelShot = true;
		shootTimer = 0.50f;
		shotReleaseTimer = 0.24f;
		rebelState = 2;
		return 0;
	}

	if (!queuedRebelShot || shotReleaseTimer > 0.0f)
	{
		return 0;
	}

	queuedRebelShot = false;
	queuedShot = false;
	float bulletX = facingRight ? x + width : x - 18.0f;
	float bulletY = y + 42.0f;
	return new EnemyBullet(bulletX, bulletY, facingRight);
}

bool RebelSoldier::applyProjectileHit(Projectile& projectile)
{
	DamageableEntity::takeDamage(projectile.getDamage());
	hurtTimer = 0.25f;
	return true;
}

void RebelSoldier::updateRebelAnimation(float deltaTime)
{
	if (shootTimer > 0.0f)
	{
		rebelState = 2;
	}
	else if (velocityX != 0.0f)
	{
		rebelState = 1;
	}
	else
	{
		rebelState = 0;
	}

	setRebelAnimation(rebelState);
	updateCustomAnimation(deltaTime);

	if (hurtTimer > 0.0f)
	{
		sprite.setColor(sf::Color(255, 120, 120));
		body.setFillColor(sf::Color(255, 80, 80));
	}
	else
	{
		sprite.setColor(sf::Color::White);
		body.setFillColor(fallbackColor);
	}
}

void RebelSoldier::setRebelAnimation(int newState)
{
	if (rebelAnimationState == newState)
	{
		return;
	}

	rebelAnimationState = newState;
	if (newState == 1)
	{
		setCustomAnimation(runTexture, REBEL_RUN_FRAMES, 12, 0.08f);
	}
	else if (newState == 2)
	{
		setCustomAnimation(fireTexture, REBEL_FIRE_FRAMES, 10, 0.05f);
	}
	else
	{
		setCustomAnimation(idleTexture, REBEL_IDLE_FRAMES, 4, 0.14f);
	}
}

void RebelSoldier::setCustomAnimation(sf::Texture& newTexture, const sf::IntRect* frames, int frameCount, float frameDuration)
{
	currentFrames = frames;
	customFrameCount = frameCount;
	customFrameIndex = 0;
	customFrameTimer = 0.0f;
	customFrameDuration = frameDuration;
	sprite.setTexture(newTexture, true);
	sprite.setTextureRect(currentFrames[0]);
	updateVisualPosition();
}

void RebelSoldier::updateCustomAnimation(float deltaTime)
{
	if (!usingSprite || currentFrames == 0 || customFrameCount <= 1)
	{
		return;
	}

	customFrameTimer += deltaTime;
	if (customFrameTimer < customFrameDuration)
	{
		return;
	}

	customFrameTimer = 0.0f;
	customFrameIndex += 1;
	if (customFrameIndex >= customFrameCount)
	{
		customFrameIndex = 0;
	}

	sprite.setTextureRect(currentFrames[customFrameIndex]);
	updateVisualPosition();
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
	maxHealth = 2;
	health = maxHealth;
	scoreValue = 100;
	contactDamage = 12;
	moveSpeed = 52.0f;
	stopDistance = 300.0f;
	detectionRange = 760.0f;
	shootingRange = 620.0f;
	attackRange = 110.0f;
	pistolEquipped = false;
	queuedShot = false;
	bazookaState = 0;
	bazookaAnimationState = -1;
	hurtTimer = 0.0f;
	fireTimer = 0.0f;
	rocketReleaseTimer = 0.0f;
	reloadTimer = 1.0f;
	rocketCooldown = 2.7f;
	queuedRocket = false;
	fallbackColor = sf::Color(230, 150, 55);
	width = 56.0f;
	height = 96.0f;
	body.setSize(sf::Vector2f(width, height));
	setSpriteScale(2.2f);

	if (loadMaskedTexture(idleTexture, "Sprites/Clean/BazookaSoldier_idle.png") &&
		loadMaskedTexture(runTexture, "Sprites/Clean/BazookaSoldier_run.png") &&
		loadMaskedTexture(fireTexture, "Sprites/Clean/BazookaSoldier_fire.png"))
	{
		usingSprite = true;
		setBazookaAnimation(0);
	}
}

void BazookaSoldier::updateAI()
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
	bool canSeePlayer = absoluteDistanceX <= detectionRange && std::abs(distanceY) < 190.0f;

	if (canSeePlayer)
	{
		facingRight = distanceX > 0.0f;

		// Heavy artillery: stop first, fire once, then wait through a reload.
		if (absoluteDistanceX <= shootingRange)
		{
			if (reloadTimer <= 0.0f && fireTimer <= 0.0f)
			{
				queuedRocket = true;
				reloadTimer = rocketCooldown;
				fireTimer = 0.54f;
				rocketReleaseTimer = 0.20f;
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

Projectile* BazookaSoldier::createProjectileIfReady()
{
	if (!queuedRocket)
	{
		return 0;
	}
	if (rocketReleaseTimer > 0.0f)
	{
		return 0;
	}

	queuedRocket = false;

	float rocketVelocityX = facingRight ? 360.0f : -360.0f;
	if (target != 0)
	{
		float distanceX = target->getCenterX() - getCenterX();
		rocketVelocityX = distanceX * 0.65f;
		if (rocketVelocityX > 430.0f)
		{
			rocketVelocityX = 430.0f;
		}
		if (rocketVelocityX < -430.0f)
		{
			rocketVelocityX = -430.0f;
		}
		if (rocketVelocityX > 0.0f && rocketVelocityX < 260.0f)
		{
			rocketVelocityX = 260.0f;
		}
		if (rocketVelocityX < 0.0f && rocketVelocityX > -260.0f)
		{
			rocketVelocityX = -260.0f;
		}
	}

	float rocketX = facingRight ? x + width + 8.0f : x - 42.0f;
	float rocketY = y + 30.0f;
	return new EnemyRocketProjectile(rocketX, rocketY, rocketVelocityX, -640.0f);
}

bool BazookaSoldier::applyProjectileHit(Projectile& projectile)
{
	DamageableEntity::takeDamage(projectile.getDamage());
	hurtTimer = 0.25f;
	return true;
}

void BazookaSoldier::update(float deltaTime)
{
	if (hurtTimer > 0.0f)
	{
		hurtTimer -= deltaTime;
	}
	if (fireTimer > 0.0f)
	{
		fireTimer -= deltaTime;
	}
	if (rocketReleaseTimer > 0.0f)
	{
		rocketReleaseTimer -= deltaTime;
	}
	if (reloadTimer > 0.0f)
	{
		reloadTimer -= deltaTime;
	}

	Enemy::update(deltaTime);
	updateBazookaAnimation();
}

void BazookaSoldier::updateBazookaAnimation()
{
	// Simple integer state: 0 idle, 1 run, 2 fire.
	if (fireTimer > 0.0f)
	{
		bazookaState = 2;
	}
	else if (velocityX != 0.0f)
	{
		bazookaState = 1;
	}
	else
	{
		bazookaState = 0;
	}

	setBazookaAnimation(bazookaState);

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

void BazookaSoldier::setBazookaAnimation(int newState)
{
	if (bazookaAnimationState == newState)
	{
		return;
	}

	bazookaAnimationState = newState;

	if (newState == 1)
	{
		sprite.setTexture(runTexture, true);
		setSpriteFrame(0, 0, 45, 42);
		playAnimation(0, 0, 11, 0.11f);
	}
	else if (newState == 2)
	{
		sprite.setTexture(fireTexture, true);
		setSpriteFrame(0, 0, 42, 44);
		playAnimation(0, 0, 3, 0.18f);
	}
	else
	{
		sprite.setTexture(idleTexture, true);
		setSpriteFrame(0, 0, 42, 45);
		playAnimation(0, 0, 6, 0.15f);
	}
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

MartianInfantry::MartianInfantry(float startX, float startY)
{
	maxHealth = 20;
	health = maxHealth;
	scoreValue = 200;
	contactDamage = 0;
	moveSpeed = 82.0f;
	stopDistance = 170.0f;
	detectionRange = 620.0f;
	shootingRange = 390.0f;
	pistolCooldown = 1.35f;
	pistolTimer = 0.5f;
	pistolEquipped = false;
	queuedShot = false;
	queuedShotReady = false;
	teleporting = true;
	dying = false;
	martianState = 4;
	martianAnimationState = -1;
	hurtTimer = 0.0f;
	fireTimer = 0.0f;
	shotReleaseTimer = 0.0f;
	pistolReloadTimer = 0.6f;
	currentFrames = 0;
	customFrameCount = 0;
	customFrameIndex = 0;
	customAnimationLoop = true;
	customAnimationFinished = false;
	customFrameTimer = 0.0f;
	customFrameDuration = 0.12f;
	fallbackColor = sf::Color(170, 220, 130);
	width = 44.0f;
	height = 84.0f;
	body.setSize(sf::Vector2f(width, height));
	setSpriteScale(2.05f);
	setPosition(startX, startY);

	if (loadMaskedTexture(idleTexture, "Sprites/Clean/Martian_idle.png") &&
		loadMaskedTexture(walkTexture, "Sprites/Clean/Martian_walk.png") &&
		loadMaskedTexture(fireTexture, "Sprites/Clean/Martian_fire.png") &&
		loadMaskedTexture(deathTexture, "Sprites/Clean/Martian_death.png") &&
		loadMaskedTexture(teleportTexture, "Sprites/Clean/Martian_teleport.png"))
	{
		usingSprite = true;
		setMartianAnimation(4);
	}
}

void MartianInfantry::updateAI()
{
	stopMoving();

	if (teleporting || dying || target == 0 || !target->isActive())
	{
		return;
	}

	if (!grounded)
	{
		return;
	}

	float distanceX = target->getCenterX() - getCenterX();
	float absoluteDistanceX = std::abs(distanceX);
	facingRight = distanceX > 0.0f;

	if (absoluteDistanceX <= shootingRange)
	{
		if (pistolReloadTimer <= 0.0f && fireTimer <= 0.0f)
		{
			queuedShotReady = true;
			pistolReloadTimer = pistolCooldown;
			fireTimer = 0.72f;
			shotReleaseTimer = 0.34f;
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
}

void MartianInfantry::update(float deltaTime)
{
	if (hurtTimer > 0.0f)
	{
		hurtTimer -= deltaTime;
	}
	if (fireTimer > 0.0f)
	{
		fireTimer -= deltaTime;
	}
	if (shotReleaseTimer > 0.0f)
	{
		shotReleaseTimer -= deltaTime;
	}
	if (pistolReloadTimer > 0.0f)
	{
		pistolReloadTimer -= deltaTime;
	}

	if (dying)
	{
		stopMoving();
		updateCustomAnimation(deltaTime);
		if (customAnimationFinished)
		{
			deactivate();
		}
		return;
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

	if (teleporting && customAnimationFinished)
	{
		finishTeleport();
	}

	if (!teleporting)
	{
		if (fireTimer > 0.0f)
		{
			setMartianAnimation(2);
		}
		else if (velocityX != 0.0f)
		{
			setMartianAnimation(1);
		}
		else
		{
			setMartianAnimation(0);
		}
	}

	updateVisualPosition();
	updateCustomAnimation(deltaTime);

	if (hurtTimer > 0.0f)
	{
		sprite.setColor(sf::Color(255, 120, 120));
		body.setFillColor(sf::Color(255, 80, 80));
	}
	else
	{
		sprite.setColor(sf::Color::White);
		body.setFillColor(fallbackColor);
	}
}

Projectile* MartianInfantry::createProjectileIfReady()
{
	if (!queuedShotReady || shotReleaseTimer > 0.0f || teleporting || dying)
	{
		return 0;
	}

	queuedShotReady = false;
	float bulletX = facingRight ? x + width : x - 18.0f;
	float bulletY = y + 35.0f;
	return new EnemyBullet(bulletX, bulletY, facingRight);
}

bool MartianInfantry::applyProjectileHit(Projectile& projectile)
{
	if (dying || teleporting || health <= 0)
	{
		return false;
	}

	health -= projectile.getDamage();
	hurtTimer = 0.25f;
	if (health <= 0)
	{
		health = 0;
		dying = true;
		contactDamage = 0;
		stopMoving();
		setMartianAnimation(3);
	}

	return true;
}

void MartianInfantry::finishTeleport()
{
	teleporting = false;
	contactDamage = 8;
	setMartianAnimation(0);
}

void MartianInfantry::setMartianAnimation(int newState)
{
	if (martianAnimationState == newState)
	{
		return;
	}

	martianAnimationState = newState;
	if (newState == 1)
	{
		setCustomAnimation(walkTexture, MARTIAN_WALK_FRAMES, 16, true, 0.08f);
	}
	else if (newState == 2)
	{
		setCustomAnimation(fireTexture, MARTIAN_FIRE_FRAMES, 18, false, 0.04f);
	}
	else if (newState == 3)
	{
		setCustomAnimation(deathTexture, MARTIAN_DEATH_FRAMES, 48, false, 0.045f);
	}
	else if (newState == 4)
	{
		setCustomAnimation(teleportTexture, MARTIAN_TELEPORT_FRAMES, 15, false, 0.07f);
	}
	else
	{
		setCustomAnimation(idleTexture, MARTIAN_IDLE_FRAMES, 16, true, 0.12f);
	}
}

void MartianInfantry::setCustomAnimation(sf::Texture& newTexture, const sf::IntRect* frames, int frameCount, bool loop, float frameDuration)
{
	currentFrames = frames;
	customFrameCount = frameCount;
	customAnimationLoop = loop;
	customFrameDuration = frameDuration;
	customFrameIndex = 0;
	customFrameTimer = 0.0f;
	customAnimationFinished = false;
	sprite.setTexture(newTexture, true);
	sprite.setTextureRect(currentFrames[0]);
	updateVisualPosition();
}

void MartianInfantry::updateCustomAnimation(float deltaTime)
{
	if (!usingSprite || currentFrames == 0 || customFrameCount <= 1 || customAnimationFinished)
	{
		return;
	}

	customFrameTimer += deltaTime;
	if (customFrameTimer < customFrameDuration)
	{
		return;
	}

	customFrameTimer = 0.0f;
	if (customFrameIndex + 1 >= customFrameCount)
	{
		if (customAnimationLoop)
		{
			customFrameIndex = 0;
		}
		else
		{
			customFrameIndex = customFrameCount - 1;
			sprite.setTextureRect(currentFrames[customFrameIndex]);
			updateVisualPosition();
			customAnimationFinished = true;
			return;
		}
	}
	else
	{
		customFrameIndex += 1;
	}

	sprite.setTextureRect(currentFrames[customFrameIndex]);
	updateVisualPosition();
}

const char* MartianInfantry::getEnemyName() const
{
	return "Martian Infantry";
}

MartianPod::MartianPod()
{
	maxHealth = 30;
	health = maxHealth;
	scoreValue = 0;
	contactDamage = 0;
	moveSpeed = 105.0f;
	detectionRange = 620.0f;
	canMoveInAir = true;
	podState = 0;
	podAnimationState = -1;
	hoverTimer = 0.0f;
	hoverBaseY = 330.0f;
	hurtTimer = 0.0f;
	attackTimer = 0.0f;
	bulletReleaseTimer = 0.0f;
	attackCooldownTimer = 1.0f;
	spawnDelayTimer = 1.0f;
	attackReleased = false;
	dying = false;
	readyToSpawnInfantry = false;
	bulletsToFire = 0;
	currentFrames = 0;
	customFrameCount = 0;
	customFrameIndex = 0;
	customAnimationLoop = true;
	customAnimationFinished = false;
	customFrameTimer = 0.0f;
	customFrameDuration = 0.1f;
	attackFrameIndex = 0;
	attackFrameTimer = 0.0f;
	fallbackColor = sf::Color(190, 90, 230);
	width = 88.0f;
	height = 70.0f;
	body.setSize(sf::Vector2f(width, height));
	setSpriteScale(1.9f);
	setPosition(900.0f, hoverBaseY);
	patrolLeft = 760.0f;
	patrolRight = 1180.0f;

	if (loadMaskedTexture(movementTexture, "Sprites/Clean/MartianPod_movement.png") &&
		loadMaskedTexture(attackTexture, "Sprites/Clean/MartianPod_Bullet.png") &&
		loadMaskedTexture(deathTexture, "Sprites/Clean/MartianPod_death.png"))
	{
		usingSprite = true;
		attackSprite.setTexture(attackTexture);
		attackSprite.setScale(0.70f, 0.70f);
		setPodAnimation(0);
	}
}

void MartianPod::updateAI()
{
	if (dying)
	{
		return;
	}

	float distanceX = 0.0f;
	float distanceY = 0.0f;
	bool canSeePlayer = false;
	if (target != 0 && target->isActive())
	{
		distanceX = target->getCenterX() - getCenterX();
		distanceY = target->getCenterY() - getCenterY();
		canSeePlayer = std::abs(distanceX) <= detectionRange && distanceY > -40.0f && distanceY < 520.0f;
	}

	if (attackTimer <= 0.0f && canSeePlayer && attackCooldownTimer <= 0.0f)
	{
		attackTimer = 0.72f;
		bulletReleaseTimer = 0.42f;
		attackCooldownTimer = 1.8f;
		attackReleased = false;
		bulletsToFire = 0;
		attackFrameIndex = 0;
		attackFrameTimer = 0.0f;
		stopMoving();
		setPodAnimation(1);
		return;
	}

	if (attackTimer > 0.0f)
	{
		stopMoving();
		return;
	}

	if (canSeePlayer && std::abs(distanceX) > 70.0f)
	{
		if (distanceX > 0.0f)
		{
			moveRight();
		}
		else
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

void MartianPod::update(float deltaTime)
{
	if (hurtTimer > 0.0f)
	{
		hurtTimer -= deltaTime;
	}
	if (attackCooldownTimer > 0.0f)
	{
		attackCooldownTimer -= deltaTime;
	}

	if (dying)
	{
		stopMoving();
		updateCustomAnimation(deltaTime);
		if (customAnimationFinished)
		{
			visible = false;
			spawnDelayTimer -= deltaTime;
			if (spawnDelayTimer <= 0.0f)
			{
				readyToSpawnInfantry = true;
			}
		}
		return;
	}

	updateAI();
	updateAttack(deltaTime);
	updateMovement(deltaTime);
	updateCustomAnimation(deltaTime);
	updateAttackAnimation(deltaTime);

	if (hurtTimer > 0.0f)
	{
		sprite.setColor(sf::Color(255, 120, 120));
		body.setFillColor(sf::Color(255, 80, 80));
	}
	else
	{
		sprite.setColor(sf::Color::White);
		body.setFillColor(fallbackColor);
	}
}

void MartianPod::updateMovement(float deltaTime)
{
	if (hoverTimer <= 0.0f)
	{
		hoverBaseY = y;
	}
	hoverTimer += deltaTime;
	Entity::update(deltaTime);
	y = hoverBaseY + std::sin(hoverTimer * 3.0f) * 8.0f;

	if (x <= 0.0f)
	{
		x = 0.0f;
	}
	if (x + width > movementMaxX)
	{
		x = movementMaxX - width;
	}

	updateVisualPosition();
}

void MartianPod::updateAttack(float deltaTime)
{
	if (attackTimer <= 0.0f)
	{
		if (podAnimationState != 0)
		{
			setPodAnimation(0);
		}
		return;
	}

	attackTimer -= deltaTime;
	if (bulletReleaseTimer > 0.0f)
	{
		bulletReleaseTimer -= deltaTime;
	}

	// Bullets appear on the final charge frames instead of at the start of the animation.
	if (!attackReleased && bulletReleaseTimer <= 0.0f)
	{
		attackReleased = true;
		bulletsToFire = 3;
	}

	if (attackTimer <= 0.0f)
	{
		setPodAnimation(0);
	}
}

Projectile* MartianPod::createProjectileIfReady()
{
	if (bulletsToFire <= 0 || dying)
	{
		return 0;
	}

	bulletsToFire -= 1;
	float offset = static_cast<float>(bulletsToFire - 1) * 18.0f;
	return new EnemyDownwardBullet(getCenterX() + offset - 5.0f, y + height - 4.0f, activeLevel);
}

Entity* MartianPod::createSpawnedEntityIfReady()
{
	if (!readyToSpawnInfantry)
	{
		return 0;
	}

	readyToSpawnInfantry = false;
	float spawnX = findTeleportX();
	float groundY = static_cast<float>(Constants::GROUND_Y);
	if (activeLevel != 0)
	{
		groundY = activeLevel->getGroundYAt(spawnX);
	}

	MartianInfantry* infantry = new MartianInfantry(spawnX, groundY - 84.0f);
	infantry->setTarget(target);
	deactivate();
	return infantry;
}

bool MartianPod::applyProjectileHit(Projectile& projectile)
{
	if (dying || health <= 0)
	{
		return false;
	}

	health -= projectile.getDamage();
	hurtTimer = 0.25f;
	if (health <= 0)
	{
		health = 0;
		startDeathAnimation();
	}

	return true;
}

void MartianPod::startDeathAnimation()
{
	dying = true;
	contactDamage = 0;
	bulletsToFire = 0;
	attackTimer = 0.0f;
	stopMoving();
	setPodAnimation(2);
}

void MartianPod::draw(sf::RenderWindow& window)
{
	if (!visible)
	{
		return;
	}

	if (!dying && attackTimer > 0.0f && !attackReleased)
	{
		window.draw(attackSprite);
	}

	Enemy::draw(window);
}

void MartianPod::setPodAnimation(int newState)
{
	if (podAnimationState == newState)
	{
		return;
	}

	podAnimationState = newState;
	if (newState == 2)
	{
		setCustomAnimation(deathTexture, MARTIAN_POD_DEATH_FRAMES, 21, false, 0.055f);
	}
	else
	{
		setCustomAnimation(movementTexture, MARTIAN_POD_MOVE_FRAMES, 16, true, 0.09f);
	}
}

void MartianPod::setCustomAnimation(sf::Texture& newTexture, const sf::IntRect* frames, int frameCount, bool loop, float frameDuration)
{
	currentFrames = frames;
	customFrameCount = frameCount;
	customAnimationLoop = loop;
	customFrameDuration = frameDuration;
	customFrameIndex = 0;
	customFrameTimer = 0.0f;
	customAnimationFinished = false;
	sprite.setTexture(newTexture, true);
	sprite.setTextureRect(currentFrames[0]);
	updateVisualPosition();
}

void MartianPod::updateCustomAnimation(float deltaTime)
{
	if (!usingSprite || currentFrames == 0 || customFrameCount <= 1 || customAnimationFinished)
	{
		return;
	}

	customFrameTimer += deltaTime;
	if (customFrameTimer < customFrameDuration)
	{
		return;
	}

	customFrameTimer = 0.0f;
	if (customFrameIndex + 1 >= customFrameCount)
	{
		if (customAnimationLoop)
		{
			customFrameIndex = 0;
		}
		else
		{
			customFrameIndex = customFrameCount - 1;
			sprite.setTextureRect(currentFrames[customFrameIndex]);
			updateVisualPosition();
			customAnimationFinished = true;
			return;
		}
	}
	else
	{
		customFrameIndex += 1;
	}

	sprite.setTextureRect(currentFrames[customFrameIndex]);
	updateVisualPosition();
}

void MartianPod::updateAttackAnimation(float deltaTime)
{
	if (attackTimer <= 0.0f)
	{
		return;
	}

	if (attackReleased)
	{
		return;
	}

	attackFrameTimer += deltaTime;
	if (attackFrameTimer >= 0.06f)
	{
		attackFrameTimer = 0.0f;
		attackFrameIndex += 1;
		if (attackFrameIndex >= 8)
		{
			attackFrameIndex = 7;
		}
	}

	attackSprite.setTextureRect(MARTIAN_POD_ATTACK_FRAMES[attackFrameIndex]);
	sf::FloatRect bounds = attackSprite.getGlobalBounds();
	attackSprite.setPosition(getCenterX() - bounds.width / 2.0f, y + height - 4.0f);
}

float MartianPod::findTeleportX() const
{
	float spawnX = x;
	if (target != 0 && target->isActive())
	{
		if (target->getCenterX() < getCenterX())
		{
			spawnX = target->getX() + 160.0f;
		}
		else
		{
			spawnX = target->getX() - 160.0f;
		}
	}

	if (spawnX < 120.0f)
	{
		spawnX = 120.0f;
	}
	if (spawnX > movementMaxX - 120.0f)
	{
		spawnX = movementMaxX - 120.0f;
	}
	return spawnX;
}

const char* MartianPod::getEnemyName() const
{
	return "Martian Pod";
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
