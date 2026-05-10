#include "Boss.h"

#include "Collectible.h"
#include "Constants.h"
#include "EnemyBullet.h"
#include "EnemyFactory.h"
#include "EnemyGrenadeProjectile.h"
#include "EnemyRocketProjectile.h"
#include "EntityManager.h"
#include "LevelManager.h"
#include "PlayerSoldier.h"
#include "Projectile.h"

#include <cmath>
#include <cstdlib>


using namespace std;
using namespace sf;

namespace
{
	float clampBossValue(float value, float minimum, float maximum)
	{
		if (value < minimum)
		{
			return minimum;
		}
		if (value > maximum)
		{
			return maximum;
		}
		return value;
	}

	float aimedVelocityX(const Boss& boss, const PlayerSoldier* player, float speed)
	{
		if (player == 0)
		{
			return speed;
		}

		float dx = player->getCenterX() - boss.getCenterX();
		float dy = player->getCenterY() - boss.getCenterY();
		float length = sqrt(dx * dx + dy * dy);
		if (length < 1)
		{
			return speed;
		}
		return dx / length * speed;
	}

	float aimedVelocityY(const Boss& boss, const PlayerSoldier* player, float speed)
	{
		if (player == 0)
		{
			return 0;
		}

		float dx = player->getCenterX() - boss.getCenterX();
		float dy = player->getCenterY() - boss.getCenterY();
		float length = sqrt(dx * dx + dy * dy);
		if (length < 1)
		{
			return 0;
		}
		return dy / length * speed;
	}
}

Boss::Boss(BossType newBossType, BiomeType newBiomeType, bool newFinalPhase)
{
	bossType = newBossType;
	biomeType = newBiomeType;
	finalPhase = newFinalPhase;
	retreating = false;
	retreatFinished = false;
	arenaLeft = 0;
	arenaRight = Constants::BOSS_WORLD_WIDTH;
	arenaTop = 0;
	arenaBottom = Constants::WORLD_HEIGHT;
	attackTimer = 0.9f;
	minionSpawnTimer = 4;
	waitingForMinionBatchClear = false;
	pendingProjectileCount = 0;
	maxHealth = 150;
	if (finalPhase)
	{
		maxHealth /= 2;
	}
	health = maxHealth;
	scoreValue = 500;
	contactDamage = 24;
	detectionRange = 1600;
	shootingRange = 1300;
	stopDistance = 340;
	moveSpeed = 70;
	canMoveInAir = true;
	fallbackColor = Color(190, 70, 70);
	width = 210;
	height = 145;
	body.setSize(Vector2f(width, height));
	body.setFillColor(fallbackColor);
	body.setOutlineColor(Color::Black);
	body.setOutlineThickness(3);
}

Boss::~Boss()
{
}

bool Boss::loadStaticBossTexture(const char* fileName, float scale)
{
	if (!texture.loadFromFile(fileName))
	{
		usingSprite = false;
		return false;
	}

	Vector2u textureSize = texture.getSize();
	sprite.setTexture(texture);
	spriteScale = scale;
	spriteFacesLeft = true;
	usingSprite = true;
	width = static_cast<float>(textureSize.x) * spriteScale;
	height = static_cast<float>(textureSize.y) * spriteScale;
	body.setSize(Vector2f(width, height));
	updateBossVisual();
	return true;
}

void Boss::enqueueProjectile(int projectileKind)
{
	if (pendingProjectileCount >= 8)
	{
		return;
	}
	pendingProjectiles[pendingProjectileCount] = projectileKind;
	pendingProjectileCount += 1;
}

void Boss::updateBossVisual()
{
	body.setPosition(x, y);
	if (!usingSprite)
	{
		return;
	}

	sprite.setScale(spriteScale, spriteScale);
	FloatRect bounds = sprite.getGlobalBounds();
	sprite.setPosition(x + width / 2 - bounds.width / 2, y + height - bounds.height);
}

void Boss::update(float deltaTime)
{
	if (contactDamageTimer > 0)
	{
		contactDamageTimer -= deltaTime;
	}

	Entity::update(deltaTime);
	clampToArena();

	if (activeLevel != 0)
	{
		if (x < 0)
		{
			x = 0;
		}
		if (x + width > movementMaxX && !retreating)
		{
			x = movementMaxX - width;
		}
	}
	updateBossVisual();
}

void Boss::clampToArena()
{
	float maxX = arenaRight - width;
	float maxY = arenaBottom - height;
	if (maxX < arenaLeft)
	{
		maxX = arenaLeft;
	}
	if (maxY < arenaTop)
	{
		maxY = arenaTop;
	}

	x = clampBossValue(x, arenaLeft, maxX);
	y = clampBossValue(y, arenaTop, maxY);
}

void Boss::updateMinionBatch(EntityManager& entities)
{
	if (waitingForMinionBatchClear && entities.countActiveNonBossEnemies() == 0)
	{
		Collectible* crate = new Collectible(COLLECTIBLE_TURKEY, getCenterX() - 16, y - 32);
		entities.addEntity(crate);
		waitingForMinionBatchClear = false;
		minionSpawnTimer = 7;
	}
}

void Boss::draw(RenderWindow& window)
{
	Enemy::draw(window);
}

bool Boss::shouldRetreat() const
{
	return false;
}

bool Boss::isBossDead() const
{
	return health <= 0;
}

void Boss::takeDamage(int amount)
{
	if (amount <= 0 || health <= 0)
	{
		return;
	}

	health -= amount;
	if (health < 0)
	{
		health = 0;
	}
}

bool Boss::applyProjectileHit(Projectile& projectile)
{
	int bossDamage = 3;
	if (projectile.isExplosive())
	{
		bossDamage = 20;
	}
	else if (projectile.isMelee())
	{
		bossDamage = 10;
	}

	if (projectile.getDamage() <= 0)
	{
		return false;
	}

	takeDamage(bossDamage);
	return false;
}

Projectile* Boss::createProjectileIfReady()
{
	if (pendingProjectileCount <= 0)
	{
		return 0;
	}

	int projectileKind = pendingProjectiles[0];
	for (int i = 1; i < pendingProjectileCount; i += 1)
	{
		pendingProjectiles[i - 1] = pendingProjectiles[i];
	}
	pendingProjectileCount -= 1;
	return buildProjectile(projectileKind);
}

bool Boss::hasRetreated() const
{
	return retreatFinished;
}

bool Boss::isRetreating() const
{
	return retreating;
}

bool Boss::isFinalPhaseBoss() const
{
	return finalPhase;
}

BossType Boss::getBossType() const
{
	return bossType;
}

const char* Boss::getBossDisplayName() const
{
	return getEnemyName();
}

int Boss::getMaxHealth() const
{
	return maxHealth;
}

void Boss::setArenaBounds(float left, float right, float top, float bottom)
{
	arenaLeft = left;
	arenaRight = right;
	arenaTop = top;
	arenaBottom = bottom;
	clampToArena();
	updateBossVisual();
}

IronNokanaBoss::IronNokanaBoss(float startX, float worldY, bool finalPhaseBoss)
	: Boss(BossType::IronNokana, finalPhaseBoss ? BiomeType::Merged : BiomeType::Plains, finalPhaseBoss)
{
	float ironScale = 1.55f;
	float ironXOffset = 0;    // negative = left, positive = right

	width = 180 * ironScale;
	height = 121 * ironScale;

	moveSpeed = 0;
	patrolDirectionTimer = 0;
	fallbackColor = Color(150, 92, 52);

	body.setSize(Vector2f(width, height));
	body.setFillColor(fallbackColor);

	loadStaticBossTexture("Sprites/Clean/Iron_Nokana.png", ironScale);

	setSpawnPosition(startX + ironXOffset, worldY - height);

	updateBossVisual();
}

void IronNokanaBoss::update(float deltaTime, PlayerSoldier& player, EntityManager& entities, LevelManager& level)
{
	(void)level;
	if (player.getCenterX() > getCenterX())
	{
		facingRight = true;
	}
	else
	{
		facingRight = false;
	}

	if (attackTimer > 0)
	{
		attackTimer -= deltaTime;
	}
	if (minionSpawnTimer > 0)
	{
		minionSpawnTimer -= deltaTime;
	}

	if (attackTimer <= 0)
	{
		attack(player, entities);
		attackTimer = 1.65f;
	}
	updateMinionBatch(entities);
	if (minionSpawnTimer <= 0 && !waitingForMinionBatchClear)
	{
		spawnMinions(entities);
	}

}

void IronNokanaBoss::attack(PlayerSoldier& player, EntityManager& entities)
{
	(void)entities;
	float closeDistance = abs(player.getCenterX() - getCenterX());
	enqueueProjectile(0);
	if (closeDistance < 260 && player.getCenterY() > y + 40)
	{
		enqueueProjectile(1);
	}
	if ((rand() % 3) == 0)
	{
		enqueueProjectile(2);
	}
}

void IronNokanaBoss::spawnMinions(EntityManager& entities)
{
	int count = 2 + rand() % 3;
	for (int i = 0; i < count; i += 1)
	{
		EnemyKind kind = ENEMY_REBEL;
		if (i == 1)
		{
			kind = ENEMY_BAZOOKA;
		}
		else if (i == 2)
		{
			kind = ENEMY_GRENADE;
		}
		Enemy* minion = EnemyFactory::createEnemy(kind, x - 140 - i * 70, y + height - 96, target);
		entities.addEntity(minion);
	}
	waitingForMinionBatchClear = true;
	minionSpawnTimer = 6;
}

void IronNokanaBoss::startRetreat()
{
	retreating = false;
	velocityX = 0;
	velocityY = 0;
}

void IronNokanaBoss::updateRetreat(float deltaTime)
{
	(void)deltaTime;
}

Projectile* IronNokanaBoss::buildProjectile(int projectileKind)
{
	float startX = facingRight ? x + width - 22 : x - 20;
	float startY = y + 48;
	if (projectileKind == 1)
	{
		return new EnemyBullet(startX, y + 94, facingRight);
	}
	if (projectileKind == 2)
	{
		float vx = facingRight ? 250 : -250;
		return new EnemyGrenadeProjectile(startX, y + 28, vx, -500);
	}
	return new EnemyRocketProjectile(startX, startY, aimedVelocityX(*this, target, 380), aimedVelocityY(*this, target, 380) - 80);
}

const char* IronNokanaBoss::getEnemyName() const
{
	return "Iron Nokana";
}

SeaSatanBoss::SeaSatanBoss(float startX, float startY, bool finalPhaseBoss)
	: Boss(BossType::SeaSatan, finalPhaseBoss ? BiomeType::Merged : BiomeType::Aquatic, finalPhaseBoss)
{
	width = 364 * 1;
	height = 227 * 1;
	moveSpeed = 58;
	patrolLeftBound = startX - 420;
	patrolRightBound = startX + 420;
	fallbackColor = Color(45, 110, 170);
	body.setSize(Vector2f(width, height));
	body.setFillColor(fallbackColor);
	loadStaticBossTexture("Sprites/Clean/Sea_Satan.png", 1);
	setSpawnPosition(startX, startY);
	velocityX = finalPhase ? 0 : -moveSpeed;
	updateBossVisual();
}

void SeaSatanBoss::update(float deltaTime, PlayerSoldier& player, EntityManager& entities, LevelManager& level)
{
	(void)level;
	facingRight = player.getCenterX() > getCenterX();

	if (finalPhase)
	{
		velocityX = 0;
		velocityY = 0;
	}
	else if (x <= patrolLeftBound)
	{
		velocityX = moveSpeed;
	}
	else if (x + width >= patrolRightBound)
	{
		velocityX = -moveSpeed;
	}

	if (attackTimer > 0)
	{
		attackTimer -= deltaTime;
	}
	if (minionSpawnTimer > 0)
	{
		minionSpawnTimer -= deltaTime;
	}
	if (attackTimer <= 0)
	{
		attack(player, entities);
		attackTimer = 1.35f;
	}
	updateMinionBatch(entities);
	if (minionSpawnTimer <= 0 && !waitingForMinionBatchClear)
	{
		spawnMinions(entities);
	}

}

void SeaSatanBoss::attack(PlayerSoldier& player, EntityManager& entities)
{
	(void)player;
	(void)entities;
	enqueueProjectile(0);
	enqueueProjectile(1);
	if ((rand() % 2) == 0)
	{
		enqueueProjectile(2);
	}
	if (target != 0 && target->getCenterY() < y)
	{
		enqueueProjectile(3);
	}
}

void SeaSatanBoss::spawnMinions(EntityManager& entities)
{
	int count = 2 + rand() % 3;
	for (int i = 0; i < count; i += 1)
	{
		EnemyKind kind = (i % 2 == 0) ? ENEMY_SUB_VEHICLE : ENEMY_ZOMBIE;
		Enemy* minion = EnemyFactory::createEnemy(kind, x - 180 + i * 95, y + 120, target);
		entities.addEntity(minion);
	}
	waitingForMinionBatchClear = true;
	minionSpawnTimer = 7;
}

void SeaSatanBoss::startRetreat()
{
	retreating = false;
	velocityY = 0;
}

void SeaSatanBoss::updateRetreat(float deltaTime)
{
	(void)deltaTime;
}

Projectile* SeaSatanBoss::buildProjectile(int projectileKind)
{
	if (projectileKind == 0)
	{
		return new EnemyBullet(x - 18, y + 48, false);
	}
	if (projectileKind == 1)
	{
		return new EnemyBullet(x + width, y + 48, true);
	}
	if (projectileKind == 3)
	{
		return new EnemyRocketProjectile(getCenterX(), y, 0, -430);
	}
	return new EnemyRocketProjectile(getCenterX(), getCenterY(), aimedVelocityX(*this, target, 360), aimedVelocityY(*this, target, 360));
}

const char* SeaSatanBoss::getEnemyName() const
{
	return "Sea Satan";
}

HairbusterRibertsBoss::HairbusterRibertsBoss(float startX, float startY, bool finalPhaseBoss)
	: Boss(BossType::HairbusterRiberts, finalPhaseBoss ? BiomeType::Merged : BiomeType::Aerial, finalPhaseBoss)
{
	width = 180 * 1.3f;
	height = 105 * 1.3f;
	moveSpeed = 82;
	patrolLeftBound = startX - 500;
	patrolRightBound = startX + 500;
	fallbackColor = Color(165, 165, 85);
	body.setSize(Vector2f(width, height));
	body.setFillColor(fallbackColor);
	loadStaticBossTexture("Sprites/Clean/Hairbuster.png", 1.3f);
	setSpawnPosition(startX, startY);
	velocityX = moveSpeed;
	updateBossVisual();
}

void HairbusterRibertsBoss::update(float deltaTime, PlayerSoldier& player, EntityManager& entities, LevelManager& level)
{
	(void)level;
	facingRight = player.getCenterX() > getCenterX();
	if (x <= patrolLeftBound)
	{
		velocityX = moveSpeed;
	}
	else if (x + width >= patrolRightBound)
	{
		velocityX = -moveSpeed;
	}

	if (attackTimer > 0)
	{
		attackTimer -= deltaTime;
	}
	if (minionSpawnTimer > 0)
	{
		minionSpawnTimer -= deltaTime;
	}
	if (attackTimer <= 0)
	{
		attack(player, entities);
		attackTimer = 1.45f;
	}
	updateMinionBatch(entities);
	if (minionSpawnTimer <= 0 && !waitingForMinionBatchClear)
	{
		spawnMinions(entities);
	}

}

void HairbusterRibertsBoss::attack(PlayerSoldier& player, EntityManager& entities)
{
	(void)player;
	(void)entities;
	enqueueProjectile(0);
	enqueueProjectile(1);
	if ((rand() % 2) == 0)
	{
		enqueueProjectile(2);
	}
}

void HairbusterRibertsBoss::spawnMinions(EntityManager& entities)
{
	int count = 2 + rand() % 3;
	for (int i = 0; i < count; i += 1)
	{
		EnemyKind kind = (i % 2 == 0) ? ENEMY_FLYING_TARA : ENEMY_MARTIAN;
		Enemy* minion = EnemyFactory::createEnemy(kind, x - 220 + i * 110, y + 80, target);
		entities.addEntity(minion);
	}
	waitingForMinionBatchClear = true;
	minionSpawnTimer = 7.5f;
}

void HairbusterRibertsBoss::startRetreat()
{
	retreating = false;
	velocityY = 0;
}

void HairbusterRibertsBoss::updateRetreat(float deltaTime)
{
	(void)deltaTime;
}

Projectile* HairbusterRibertsBoss::buildProjectile(int projectileKind)
{
	if (projectileKind == 1)
	{
		return new EnemyGrenadeProjectile(getCenterX(), y + height - 10, facingRight ? 120 : -120, 120);
	}
	if (projectileKind == 2)
	{
		return new EnemyRocketProjectile(getCenterX() + 36, getCenterY(), aimedVelocityX(*this, target, 390) + 70, aimedVelocityY(*this, target, 390));
	}
	return new EnemyRocketProjectile(getCenterX() - 36, getCenterY(), aimedVelocityX(*this, target, 390) - 70, aimedVelocityY(*this, target, 390));
}

const char* HairbusterRibertsBoss::getEnemyName() const
{
	return "Hairbuster Riberts";
}