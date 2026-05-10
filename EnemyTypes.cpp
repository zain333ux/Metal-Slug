#include "EnemyTypes.h"

#include "Constants.h"
#include "EnemyDownwardBullet.h"
#include "EnemyGrenadeProjectile.h"
#include "EnemyRocketProjectile.h"
#include "EnemyBullet.h"
#include "Level.h"
#include "PlayerSoldier.h"
#include "Projectile.h"
#include "GrenadeProjectile.h"
#include "RocketProjectile.h"
#include "VehicleBullet.h"

#include <cmath>


using namespace std;
using namespace sf;

static const IntRect REBEL_IDLE_FRAMES[] =
{
	IntRect(0, 0, 43, 42), IntRect(43, 0, 43, 42), IntRect(86, 0, 43, 42), IntRect(129, 0, 44, 42)
};

static const IntRect REBEL_RUN_FRAMES[] =
{
	IntRect(0, 0, 39, 42), IntRect(39, 0, 40, 42), IntRect(79, 0, 43, 42), IntRect(122, 0, 45, 42),
	IntRect(167, 0, 40, 42), IntRect(207, 0, 39, 42), IntRect(246, 0, 38, 42), IntRect(284, 0, 40, 42),
	IntRect(324, 0, 43, 42), IntRect(367, 0, 41, 42), IntRect(408, 0, 41, 42), IntRect(449, 0, 39, 42)
};

static const IntRect REBEL_FIRE_FRAMES[] =
{
	IntRect(0, 0, 47, 51), IntRect(47, 0, 51, 51), IntRect(98, 0, 50, 51), IntRect(148, 0, 45, 51),
	IntRect(193, 0, 45, 51), IntRect(238, 0, 43, 51), IntRect(281, 0, 46, 51), IntRect(327, 0, 50, 51),
	IntRect(377, 0, 49, 51), IntRect(426, 0, 46, 51)
};

static const IntRect MARTIAN_POD_MOVE_FRAMES[] =
{
	IntRect(5, 3, 43, 38), IntRect(48, 3, 48, 38), IntRect(99, 3, 44, 38), IntRect(146, 3, 43, 38),
	IntRect(192, 3, 48, 38), IntRect(240, 3, 48, 38), IntRect(288, 4, 48, 37), IntRect(336, 4, 38, 37),
	IntRect(5, 45, 43, 37), IntRect(48, 45, 48, 37), IntRect(99, 45, 43, 37), IntRect(145, 45, 44, 37),
	IntRect(192, 44, 48, 38), IntRect(240, 43, 48, 39), IntRect(288, 44, 48, 38), IntRect(336, 43, 39, 39)
};

static const IntRect MARTIAN_POD_DEATH_FRAMES[] =
{
	IntRect(5, 20, 44, 38), IntRect(52, 18, 49, 40), IntRect(104, 8, 54, 50),
	IntRect(164, 6, 55, 52), IntRect(226, 5, 63, 53), IntRect(295, 4, 64, 54),
	IntRect(5, 69, 61, 52), IntRect(70, 70, 54, 51), IntRect(128, 64, 55, 57),
	IntRect(187, 68, 46, 53), IntRect(237, 69, 44, 52), IntRect(285, 69, 45, 52),
	IntRect(333, 68, 45, 53),
	IntRect(5, 130, 43, 53), IntRect(51, 129, 43, 54), IntRect(97, 127, 40, 56),
	IntRect(143, 126, 41, 57), IntRect(188, 129, 35, 54), IntRect(228, 128, 35, 55),
	IntRect(268, 137, 41, 40), IntRect(310, 139, 65, 34)
};

static const IntRect MARTIAN_POD_ATTACK_FRAMES[] =
{
	IntRect(308, 55, 44, 88), IntRect(368, 55, 36, 88), IntRect(420, 55, 28, 88),
	IntRect(452, 43, 64, 112), IntRect(528, 35, 88, 128), IntRect(628, 27, 104, 144),
	IntRect(744, 23, 112, 152), IntRect(868, 23, 88, 152)
};

static const IntRect MARTIAN_IDLE_FRAMES[] =
{
	IntRect(0, 0, 52, 43), IntRect(52, 0, 52, 43), IntRect(104, 0, 52, 43), IntRect(156, 0, 52, 43),
	IntRect(208, 0, 52, 43), IntRect(260, 0, 52, 43), IntRect(312, 0, 52, 43), IntRect(364, 0, 52, 43),
	IntRect(416, 0, 52, 43), IntRect(468, 0, 52, 43), IntRect(520, 0, 52, 43), IntRect(572, 0, 52, 43),
	IntRect(624, 0, 52, 43), IntRect(676, 0, 52, 43), IntRect(728, 0, 52, 43), IntRect(780, 0, 52, 43)
};

static const IntRect MARTIAN_WALK_FRAMES[] =
{
	IntRect(0, 0, 51, 43), IntRect(51, 0, 51, 43), IntRect(102, 0, 51, 43), IntRect(153, 0, 51, 43),
	IntRect(204, 0, 51, 43), IntRect(255, 0, 51, 43), IntRect(306, 0, 51, 43), IntRect(357, 0, 51, 43),
	IntRect(408, 0, 51, 43), IntRect(459, 0, 51, 43), IntRect(510, 0, 51, 43), IntRect(561, 0, 51, 43),
	IntRect(612, 0, 51, 43), IntRect(663, 0, 51, 43), IntRect(714, 0, 51, 43), IntRect(765, 0, 51, 43)
};

static const IntRect MARTIAN_FIRE_FRAMES[] =
{
	IntRect(0, 0, 71, 43), IntRect(71, 0, 71, 43), IntRect(142, 0, 71, 43), IntRect(213, 0, 71, 43),
	IntRect(284, 0, 71, 43), IntRect(355, 0, 71, 43), IntRect(426, 0, 71, 43), IntRect(497, 0, 71, 43),
	IntRect(568, 0, 71, 43), IntRect(639, 0, 71, 43), IntRect(710, 0, 71, 43), IntRect(781, 0, 71, 43),
	IntRect(852, 0, 71, 43), IntRect(923, 0, 71, 43), IntRect(994, 0, 71, 43), IntRect(1065, 0, 71, 43),
	IntRect(1136, 0, 71, 43), IntRect(1207, 0, 71, 43)
};

static const IntRect MARTIAN_TELEPORT_FRAMES[] =
{
	IntRect(0, 0, 66, 65), IntRect(66, 0, 66, 65), IntRect(132, 0, 66, 65), IntRect(198, 0, 66, 65),
	IntRect(264, 0, 66, 65), IntRect(330, 0, 66, 65), IntRect(396, 0, 66, 65), IntRect(462, 0, 66, 65),
	IntRect(528, 0, 66, 65), IntRect(594, 0, 66, 65), IntRect(660, 0, 66, 65), IntRect(726, 0, 66, 65),
	IntRect(792, 0, 66, 65), IntRect(0, 66, 66, 65), IntRect(66, 66, 66, 65)
};

static const IntRect MARTIAN_DEATH_FRAMES[] =
{
	IntRect(10, 9, 40, 34), IntRect(62, 9, 41, 34), IntRect(115, 9, 42, 34), IntRect(169, 9, 40, 34),
	IntRect(226, 9, 41, 34), IntRect(284, 9, 38, 34), IntRect(334, 9, 38, 34), IntRect(385, 9, 37, 34),
	IntRect(437, 9, 36, 34), IntRect(489, 9, 35, 34), IntRect(542, 9, 37, 34), IntRect(595, 10, 39, 33),
	IntRect(649, 13, 38, 30), IntRect(702, 12, 54, 31), IntRect(756, 13, 33, 30), IntRect(822, 15, 29, 28),
	IntRect(14, 43, 34, 43), IntRect(72, 43, 29, 43), IntRect(128, 43, 31, 43), IntRect(183, 43, 33, 43),
	IntRect(216, 43, 54, 43), IntRect(270, 43, 54, 43), IntRect(324, 43, 54, 43), IntRect(378, 43, 54, 43),
	IntRect(432, 43, 54, 43), IntRect(486, 43, 54, 43), IntRect(540, 43, 54, 43), IntRect(594, 43, 42, 43),
	IntRect(666, 43, 24, 5), IntRect(720, 43, 24, 5), IntRect(760, 43, 50, 5), IntRect(810, 43, 41, 5),
	IntRect(8, 86, 44, 36), IntRect(63, 86, 45, 36), IntRect(116, 86, 46, 36), IntRect(162, 86, 54, 36),
	IntRect(216, 86, 54, 36), IntRect(270, 86, 54, 36), IntRect(324, 86, 54, 36), IntRect(378, 86, 54, 36),
	IntRect(432, 86, 54, 36), IntRect(486, 86, 54, 36), IntRect(540, 86, 54, 36), IntRect(594, 86, 27, 36),
	IntRect(648, 86, 1, 1), IntRect(702, 86, 1, 1), IntRect(756, 86, 1, 1), IntRect(810, 86, 1, 1)
};

RebelSoldier::RebelSoldier()
{
	maxHealth = 20;
	health = maxHealth;
	scoreValue = 50;
	contactDamage = 6;
	moveSpeed = 95;
	stopDistance = 185;
	detectionRange = 560;
	shootingRange = 430;
	attackRange = 70;
	pistolCooldown = 1.35f;
	pistolTimer = 0.6f;
	pistolEquipped = true;
	rebelState = 0;
	rebelAnimationState = -1;
	hurtTimer = 0;
	shootTimer = 0;
	shotReleaseTimer = 0;
	queuedRebelShot = false;
	currentFrames = 0;
	customFrameCount = 0;
	customFrameIndex = 0;
	customFrameTimer = 0;
	customFrameDuration = 0.12f;
	fallbackColor = Color(210, 70, 70);
	width = 52;
	height = 96;
	body.setSize(Vector2f(width, height));
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
	if (hurtTimer > 0)
	{
		hurtTimer -= deltaTime;
	}
	if (shootTimer > 0)
	{
		shootTimer -= deltaTime;
	}
	if (shotReleaseTimer > 0)
	{
		shotReleaseTimer -= deltaTime;
	}

	Enemy::update(deltaTime);
	updateRebelAnimation(deltaTime);
}

Projectile* RebelSoldier::createProjectileIfReady()
{
	if (queuedShot && !queuedRebelShot && shootTimer <= 0 && shotReleaseTimer <= 0)
	{
		queuedRebelShot = true;
		shootTimer = 0.50f;
		shotReleaseTimer = 0.24f;
		rebelState = 2;
		return 0;
	}

	if (!queuedRebelShot || shotReleaseTimer > 0)
	{
		return 0;
	}

	queuedRebelShot = false;
	queuedShot = false;
	float bulletX = facingRight ? x + width : x - 18;
	float bulletY = y + 42;
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
	if (shootTimer > 0)
	{
		rebelState = 2;
	}
	else if (velocityX != 0)
	{
		rebelState = 1;
	}
	else
	{
		rebelState = 0;
	}

	setRebelAnimation(rebelState);
	updateCustomAnimation(deltaTime);

	if (hurtTimer > 0)
	{
		sprite.setColor(Color(255, 120, 120));
		body.setFillColor(Color(255, 80, 80));
	}
	else
	{
		sprite.setColor(Color::White);
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

void RebelSoldier::setCustomAnimation(Texture& newTexture, const IntRect* frames, int frameCount, float frameDuration)
{
	currentFrames = frames;
	customFrameCount = frameCount;
	customFrameIndex = 0;
	customFrameTimer = 0;
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

	customFrameTimer = 0;
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
	maxHealth = 50;
	health = maxHealth;
	scoreValue = 75;
	contactDamage = 12;
	moveSpeed = 70;
	stopDistance = 210;
	detectionRange = 560;
	shootingRange = 430;
	attackRange = 90;
	pistolCooldown = 1.5f;
	pistolTimer = 0.8f;
	pistolEquipped = true;
	shieldState = 0;
	shieldAnimationState = -1;
	hurtTimer = 0;
	shootTimer = 0;
	shotReleaseTimer = 0;
	fallbackColor = Color(90, 130, 210);
	width = 58;
	height = 96;
	body.setSize(Vector2f(width, height));
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
	if (queuedShot && shootTimer <= 0 && shotReleaseTimer <= 0)
	{
		shootTimer = 0.60f;
		shotReleaseTimer = 0.22f;
		shieldState = 2;
		return 0;
	}

	if (queuedShot && shotReleaseTimer > 0)
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
	if (hurtTimer > 0)
	{
		hurtTimer -= deltaTime;
	}
	if (shootTimer > 0)
	{
		shootTimer -= deltaTime;
	}
	if (shotReleaseTimer > 0)
	{
		shotReleaseTimer -= deltaTime;
	}

	Enemy::update(deltaTime);
	updateShieldAnimation();
}

void ShieldedSoldier::updateShieldAnimation()
{
	// Simple integer state: 0 idle, 1 walk, 2 shoot, 3 hurt.
	if (hurtTimer > 0)
	{
		shieldState = 3;
	}
	else if (shootTimer > 0)
	{
		shieldState = 2;
	}
	else if (velocityX != 0)
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

	if (hurtTimer > 0)
	{
		body.setFillColor(Color(255, 80, 80));
		sprite.setColor(Color(255, 120, 120));
	}
	else
	{
		body.setFillColor(fallbackColor);
		sprite.setColor(Color::White);
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
	maxHealth = 20;
	health = maxHealth;
	scoreValue = 100;
	contactDamage = 12;
	moveSpeed = 52;
	stopDistance = 300;
	detectionRange = 760;
	shootingRange = 620;
	attackRange = 110;
	pistolEquipped = false;
	queuedShot = false;
	bazookaState = 0;
	bazookaAnimationState = -1;
	hurtTimer = 0;
	fireTimer = 0;
	rocketReleaseTimer = 0;
	reloadTimer = 1;
	rocketCooldown = 2.7f;
	queuedRocket = false;
	fallbackColor = Color(230, 150, 55);
	width = 56;
	height = 96;
	body.setSize(Vector2f(width, height));
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
	float absoluteDistanceX = abs(distanceX);
	bool canSeePlayer = absoluteDistanceX <= detectionRange && abs(distanceY) < 190;

	if (canSeePlayer)
	{
		facingRight = distanceX > 0;

		// Heavy artillery: stop first, fire once, then wait through a reload.
		if (absoluteDistanceX <= shootingRange)
		{
			if (reloadTimer <= 0 && fireTimer <= 0)
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
	if (rocketReleaseTimer > 0)
	{
		return 0;
	}

	queuedRocket = false;

	float rocketVelocityX = facingRight ? 360 : -360;
	if (target != 0)
	{
		float distanceX = target->getCenterX() - getCenterX();
		rocketVelocityX = distanceX * 0.65f;
		if (rocketVelocityX > 430)
		{
			rocketVelocityX = 430;
		}
		if (rocketVelocityX < -430)
		{
			rocketVelocityX = -430;
		}
		if (rocketVelocityX > 0 && rocketVelocityX < 260)
		{
			rocketVelocityX = 260;
		}
		if (rocketVelocityX < 0 && rocketVelocityX > -260)
		{
			rocketVelocityX = -260;
		}
	}

	float rocketX = facingRight ? x + width + 34 : x - 66;
	float rocketY = y + 36;
	return new EnemyRocketProjectile(rocketX, rocketY, rocketVelocityX, -640);
}

bool BazookaSoldier::applyProjectileHit(Projectile& projectile)
{
	DamageableEntity::takeDamage(projectile.getDamage());
	hurtTimer = 0.25f;
	return true;
}

void BazookaSoldier::update(float deltaTime)
{
	if (hurtTimer > 0)
	{
		hurtTimer -= deltaTime;
	}
	if (fireTimer > 0)
	{
		fireTimer -= deltaTime;
	}
	if (rocketReleaseTimer > 0)
	{
		rocketReleaseTimer -= deltaTime;
	}
	if (reloadTimer > 0)
	{
		reloadTimer -= deltaTime;
	}

	Enemy::update(deltaTime);
	updateBazookaAnimation();
}

void BazookaSoldier::updateBazookaAnimation()
{
	// Simple integer state: 0 idle, 1 run, 2 fire.
	if (fireTimer > 0)
	{
		bazookaState = 2;
	}
	else if (velocityX != 0)
	{
		bazookaState = 1;
	}
	else
	{
		bazookaState = 0;
	}

	setBazookaAnimation(bazookaState);

	if (hurtTimer > 0)
	{
		body.setFillColor(Color(255, 80, 80));
		sprite.setColor(Color(255, 120, 120));
	}
	else
	{
		body.setFillColor(fallbackColor);
		sprite.setColor(Color::White);
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
	maxHealth = 20;
	health = maxHealth;
	scoreValue = 100;
	contactDamage = 10;
	moveSpeed = 62;
	stopDistance = 250;
	detectionRange = 620;
	shootingRange = 500;
	attackRange = 95;
	pistolEquipped = false;
	queuedShot = false;
	grenadeState = 0;
	grenadeAnimationState = -1;
	hurtTimer = 0;
	fireTimer = 0;
	throwReleaseTimer = 0;
	grenadeTimer = 0.8f;
	grenadeCooldown = 2.1f;
	queuedGrenade = false;
	fallbackColor = Color(120, 210, 90);
	width = 52;
	height = 96;
	body.setSize(Vector2f(width, height));
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
	float absoluteDistanceX = abs(distanceX);
	bool canSeePlayer = absoluteDistanceX <= detectionRange && abs(distanceY) < 180;

	if (canSeePlayer)
	{
		facingRight = distanceX > 0;

		// The grenade soldier plants his feet before throwing, so the arc is easy to read.
		if (absoluteDistanceX <= shootingRange)
		{
			if (grenadeTimer <= 0 && fireTimer <= 0)
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
	if (throwReleaseTimer > 0)
	{
		return 0;
	}

	queuedGrenade = false;

	float throwVelocityX = facingRight ? 330 : -330;
	if (target != 0)
	{
		float distanceX = target->getCenterX() - getCenterX();
		throwVelocityX = distanceX * 0.9f;
		if (throwVelocityX > 430)
		{
			throwVelocityX = 430;
		}
		if (throwVelocityX < -430)
		{
			throwVelocityX = -430;
		}
		if (throwVelocityX > 0 && throwVelocityX < 230)
		{
			throwVelocityX = 230;
		}
		if (throwVelocityX < 0 && throwVelocityX > -230)
		{
			throwVelocityX = -230;
		}
	}

	float grenadeX = facingRight ? x + width + 18 : x - 48;
	float grenadeY = y + 36;
	return new EnemyGrenadeProjectile(grenadeX, grenadeY, throwVelocityX, -570);
}

bool GrenadeSoldier::applyProjectileHit(Projectile& projectile)
{
	DamageableEntity::takeDamage(projectile.getDamage());
	hurtTimer = 0.25f;
	return true;
}

void GrenadeSoldier::update(float deltaTime)
{
	if (hurtTimer > 0)
	{
		hurtTimer -= deltaTime;
	}
	if (fireTimer > 0)
	{
		fireTimer -= deltaTime;
	}
	if (throwReleaseTimer > 0)
	{
		throwReleaseTimer -= deltaTime;
	}
	if (grenadeTimer > 0)
	{
		grenadeTimer -= deltaTime;
	}

	Enemy::update(deltaTime);
	updateGrenadeAnimation();
}

void GrenadeSoldier::updateGrenadeAnimation()
{
	// Simple integer state: 0 idle, 1 run, 2 fire.
	if (fireTimer > 0)
	{
		grenadeState = 2;
	}
	else if (velocityX != 0)
	{
		grenadeState = 1;
	}
	else
	{
		grenadeState = 0;
	}

	setGrenadeAnimation(grenadeState);

	if (hurtTimer > 0)
	{
		body.setFillColor(Color(255, 80, 80));
		sprite.setColor(Color(255, 120, 120));
	}
	else
	{
		body.setFillColor(fallbackColor);
		sprite.setColor(Color::White);
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
	maxHealth = 35;
	health = maxHealth;
	scoreValue = 100;
	moveSpeed = Constants::ENEMY_MOVE_SPEED * 0.5f;
	contactDamage = 0;
	detectionRange = 430;
	stopDistance = 0;
	attackRange = 0;
	fallbackColor = Color(130, 210, 140);
	width = 52;
	height = 96;
	body.setSize(Vector2f(width, height));
	setSpriteScale(2.35f);
	spriteFacesLeft = false;

	if (loadMaskedTexture(walkTexture, "Sprites/Clean/Zombie_walk.png"))
	{
		usingSprite = true;
		sprite.setTexture(walkTexture, true);
		setSpriteFrame(0, 0, 45, 41);
		playAnimation(0, 0, 24, 0.08f);
	}
}

void ZombieEnemy::update(float deltaTime)
{
	Enemy::update(deltaTime);
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
	moveSpeed = Constants::ENEMY_MOVE_SPEED * 0.5f;
	contactDamage = 0;
	detectionRange = 390;
	stopDistance = 0;
	attackRange = 0;
	fallbackColor = Color(225, 215, 165);
	mummyAnimationState = -1;
	hurtTimer = 0;
	width = 52;
	height = 96;
	body.setSize(Vector2f(width, height));
	setSpriteScale(2.25f);

	if (loadMaskedTexture(idleTexture, "Sprites/Clean/MummyWarrior_idle.png") &&
		loadMaskedTexture(walkTexture, "Sprites/Clean/MummyWarrior_walk.png"))
	{
		usingSprite = true;
		updateMummyAnimation();
	}
}

void MummyEnemy::update(float deltaTime)
{
	if (hurtTimer > 0)
	{
		hurtTimer -= deltaTime;
	}

	Enemy::update(deltaTime);
	updateMummyAnimation();

	if (hurtTimer > 0)
	{
		sprite.setColor(Color(255, 120, 120));
	}
	else
	{
		sprite.setColor(Color::White);
	}
}

bool MummyEnemy::applyProjectileHit(Projectile& projectile)
{
	bool canDamage = projectile.isExplosive() ||
		dynamic_cast<VehicleBullet*>(&projectile) != 0 ||
		dynamic_cast<GrenadeProjectile*>(&projectile) != 0 ||
		dynamic_cast<RocketProjectile*>(&projectile) != 0;

	if (!canDamage)
	{
		return false;
	}

	DamageableEntity::takeDamage(projectile.getDamage());
	hurtTimer = 0.25f;
	return true;
}

void MummyEnemy::updateMummyAnimation()
{
	int nextState = velocityX != 0 ? 1 : 0;
	if (nextState == mummyAnimationState)
	{
		return;
	}

	mummyAnimationState = nextState;
	if (mummyAnimationState == 1)
	{
		sprite.setTexture(walkTexture, true);
		setSpriteFrame(0, 0, 36, 45);
		playAnimation(0, 0, 18, 0.075f);
	}
	else
	{
		sprite.setTexture(idleTexture, true);
		setSpriteFrame(0, 0, 40, 45);
		playAnimation(0, 0, 3, 0.16f);
	}
}

const char* MummyEnemy::getEnemyName() const
{
	return "Mummy";
}

MartianInfantry::MartianInfantry(float startX, float startY)
{
	maxHealth = 40;
	health = maxHealth;
	scoreValue = 200;
	contactDamage = 0;
	moveSpeed = 82;
	stopDistance = 170;
	detectionRange = 620;
	shootingRange = 390;
	pistolCooldown = 1.35f;
	pistolTimer = 0.5f;
	pistolEquipped = false;
	queuedShot = false;
	queuedShotReady = false;
	teleporting = true;
	dying = false;
	martianState = 4;
	martianAnimationState = -1;
	hurtTimer = 0;
	fireTimer = 0;
	shotReleaseTimer = 0;
	pistolReloadTimer = 0.6f;
	currentFrames = 0;
	customFrameCount = 0;
	customFrameIndex = 0;
	customAnimationLoop = true;
	customAnimationFinished = false;
	customFrameTimer = 0;
	customFrameDuration = 0.12f;
	fallbackColor = Color(170, 220, 130);
	width = 44;
	height = 84;
	body.setSize(Vector2f(width, height));
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
	float absoluteDistanceX = abs(distanceX);
	facingRight = distanceX > 0;

	if (absoluteDistanceX <= shootingRange)
	{
		if (pistolReloadTimer <= 0 && fireTimer <= 0)
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
	if (hurtTimer > 0)
	{
		hurtTimer -= deltaTime;
	}
	if (fireTimer > 0)
	{
		fireTimer -= deltaTime;
	}
	if (shotReleaseTimer > 0)
	{
		shotReleaseTimer -= deltaTime;
	}
	if (pistolReloadTimer > 0)
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

	if (teleporting && customAnimationFinished)
	{
		finishTeleport();
	}

	if (!teleporting)
	{
		if (fireTimer > 0)
		{
			setMartianAnimation(2);
		}
		else if (velocityX != 0)
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

	if (hurtTimer > 0)
	{
		sprite.setColor(Color(255, 120, 120));
		body.setFillColor(Color(255, 80, 80));
	}
	else
	{
		sprite.setColor(Color::White);
		body.setFillColor(fallbackColor);
	}
}

Projectile* MartianInfantry::createProjectileIfReady()
{
	if (!queuedShotReady || shotReleaseTimer > 0 || teleporting || dying)
	{
		return 0;
	}

	queuedShotReady = false;
	float bulletX = facingRight ? x + width : x - 18;
	float bulletY = y + 35;
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

void MartianInfantry::setCustomAnimation(Texture& newTexture, const IntRect* frames, int frameCount, bool loop, float frameDuration)
{
	currentFrames = frames;
	customFrameCount = frameCount;
	customAnimationLoop = loop;
	customFrameDuration = frameDuration;
	customFrameIndex = 0;
	customFrameTimer = 0;
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

	customFrameTimer = 0;
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
	maxHealth = 40;
	health = maxHealth;
	scoreValue = 0;
	contactDamage = 0;
	moveSpeed = 105;
	detectionRange = 620;
	canMoveInAir = true;
	podState = 0;
	podAnimationState = -1;
	hoverTimer = 0;
	hoverBaseY = 330;
	hurtTimer = 0;
	attackTimer = 0;
	bulletReleaseTimer = 0;
	attackCooldownTimer = 1;
	spawnDelayTimer = 1;
	attackReleased = false;
	dying = false;
	readyToSpawnInfantry = false;
	bulletsToFire = 0;
	currentFrames = 0;
	customFrameCount = 0;
	customFrameIndex = 0;
	customAnimationLoop = true;
	customAnimationFinished = false;
	customFrameTimer = 0;
	customFrameDuration = 0.1f;
	attackFrameIndex = 0;
	attackFrameTimer = 0;
	fallbackColor = Color(190, 90, 230);
	width = 88;
	height = 70;
	body.setSize(Vector2f(width, height));
	body.setOutlineThickness(0);
	body.setFillColor(Color::Transparent);
	setSpriteScale(1.9f);
	setPosition(900, hoverBaseY);
	patrolLeft = 760;
	patrolRight = 1180;

	if (loadPodTexture(movementTexture, "Sprites/Clean/MartianPod_movement.png") &&
		loadPodTexture(attackTexture, "Sprites/Clean/MartianPod_Bullet.png") &&
		loadPodTexture(deathTexture, "Sprites/Clean/MartianPod_death.png"))
	{
		usingSprite = true;
		attackSprite.setTexture(attackTexture);
		attackSprite.setScale(0.70f, 0.70f);
		setPodAnimation(0);
	}
}

bool MartianPod::loadPodTexture(Texture& targetTexture, const char* fileName)
{
	Image image;
	if (!image.loadFromFile(fileName))
	{
		return false;
	}

	image.createMaskFromColor(Color(255, 0, 255));
	image.createMaskFromColor(Color::White);
	return targetTexture.loadFromImage(image);
}

void MartianPod::setSpawnPosition(float newX, float newY)
{
	setPosition(newX, newY);
	hoverBaseY = newY;
	hoverTimer = 0;
	patrolLeft = newX - 240;
	patrolRight = newX + 240;
	if (patrolLeft < 0)
	{
		patrolLeft = 0;
	}
}
void MartianPod::updateAI()
{
	if (dying)
	{
		return;
	}

	float distanceX = 0;
	float distanceY = 0;
	bool canSeePlayer = false;
	if (target != 0 && target->isActive())
	{
		distanceX = target->getCenterX() - getCenterX();
		distanceY = target->getCenterY() - getCenterY();
		canSeePlayer = abs(distanceX) <= detectionRange && distanceY > -40 && distanceY < 520;
	}

	if (attackTimer <= 0 && canSeePlayer && attackCooldownTimer <= 0)
	{
		attackTimer = 0.72f;
		bulletReleaseTimer = 0.42f;
		attackCooldownTimer = 1.8f;
		attackReleased = false;
		bulletsToFire = 0;
		attackFrameIndex = 0;
		attackFrameTimer = 0;
		stopMoving();
		setPodAnimation(1);
		return;
	}

	if (attackTimer > 0)
	{
		stopMoving();
		return;
	}

	if (canSeePlayer && abs(distanceX) > 70)
	{
		if (distanceX > 0)
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
	if (hurtTimer > 0)
	{
		hurtTimer -= deltaTime;
	}
	if (attackCooldownTimer > 0)
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
			if (spawnDelayTimer <= 0)
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

	if (hurtTimer > 0)
	{
		sprite.setColor(Color(255, 120, 120));
		body.setFillColor(Color(255, 80, 80));
	}
	else
	{
		sprite.setColor(Color::White);
		body.setFillColor(fallbackColor);
	}
}

void MartianPod::updateMovement(float deltaTime)
{
	if (hoverTimer <= 0)
	{
		float groundY = static_cast<float>(Constants::GROUND_Y);
		if (activeLevel != 0)
		{
			groundY = activeLevel->getGroundYAt(getCenterX());
		}
		hoverBaseY = groundY - 400;
	}
	hoverTimer += deltaTime;
	Entity::update(deltaTime);
	y = hoverBaseY + sin(hoverTimer * 3) * 8;

	if (x <= 0)
	{
		x = 0;
	}
	if (x + width > movementMaxX)
	{
		x = movementMaxX - width;
	}

	updateVisualPosition();
}

void MartianPod::updateAttack(float deltaTime)
{
	if (attackTimer <= 0)
	{
		if (podAnimationState != 0)
		{
			setPodAnimation(0);
		}
		return;
	}

	attackTimer -= deltaTime;
	if (bulletReleaseTimer > 0)
	{
		bulletReleaseTimer -= deltaTime;
	}

	// Bullets appear on the final charge frames instead of at the start of the animation.
	if (!attackReleased && bulletReleaseTimer <= 0)
	{
		attackReleased = true;
		bulletsToFire = 3;
	}

	if (attackTimer <= 0)
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
	float offset = static_cast<float>(bulletsToFire - 1) * 18;
	return new EnemyDownwardBullet(getCenterX() + offset - 5, y + height - 4, activeLevel);
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

	MartianInfantry* infantry = new MartianInfantry(spawnX, groundY - 84);
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
	attackTimer = 0;
	stopMoving();
	setPodAnimation(2);
}

void MartianPod::draw(RenderWindow& window)
{
	if (!visible)
	{
		return;
	}

	if (!dying && attackTimer > 0 && !attackReleased)
	{
		window.draw(attackSprite);
	}

	if (usingSprite)
	{
		window.draw(sprite);
	}
	else
	{
		window.draw(body);
	}
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

void MartianPod::setCustomAnimation(Texture& newTexture, const IntRect* frames, int frameCount, bool loop, float frameDuration)
{
	currentFrames = frames;
	customFrameCount = frameCount;
	customAnimationLoop = loop;
	customFrameDuration = frameDuration;
	customFrameIndex = 0;
	customFrameTimer = 0;
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

	customFrameTimer = 0;
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
	if (attackTimer <= 0)
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
		attackFrameTimer = 0;
		attackFrameIndex += 1;
		if (attackFrameIndex >= 8)
		{
			attackFrameIndex = 7;
		}
	}

	attackSprite.setTextureRect(MARTIAN_POD_ATTACK_FRAMES[attackFrameIndex]);
	FloatRect bounds = attackSprite.getGlobalBounds();
	attackSprite.setPosition(getCenterX() - bounds.width / 2, y + height - 4);
}

float MartianPod::findTeleportX() const
{
	float spawnX = x;
	if (target != 0 && target->isActive())
	{
		if (target->getCenterX() < getCenterX())
		{
			spawnX = target->getX() + 160;
		}
		else
		{
			spawnX = target->getX() - 160;
		}
	}

	if (spawnX < 120)
	{
		spawnX = 120;
	}
	if (spawnX > movementMaxX - 120)
	{
		spawnX = movementMaxX - 120;
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
	width = 140;
	height = 150;
	moveSpeed = 95;
	stopDistance = 220;
	contactDamage = 25;
	detectionRange = 900;
	attackRange = 145;
	fallbackColor = Color(180, 80, 255);
	body.setSize(Vector2f(width, height));

	if (phase >= 4)
	{
		maxHealth = 500;
		health = maxHealth;
		scoreValue = 1500;
		fallbackColor = Color(255, 80, 120);
	}
	if (phase == 2 || phase >= 4)
	{
		canMoveInAir = true;
	}
}

void BossEnemy::updateAI()
{
	Enemy::updateAI();
	if (phase == 2 && y > 420)
	{
		velocityY = -70;
	}
	else if (phase == 3 && y < Constants::GROUND_Y - height)
	{
		velocityY = 45;
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