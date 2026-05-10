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
#include <iostream>

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
		float length = std::sqrt(dx * dx + dy * dy);
		if (length < 1.0f)
		{
			return speed;
		}
		return dx / length * speed;
	}

	float aimedVelocityY(const Boss& boss, const PlayerSoldier* player, float speed)
	{
		if (player == 0)
		{
			return 0.0f;
		}

		float dx = player->getCenterX() - boss.getCenterX();
		float dy = player->getCenterY() - boss.getCenterY();
		float length = std::sqrt(dx * dx + dy * dy);
		if (length < 1.0f)
		{
			return 0.0f;
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
	arenaLeft = 0.0f;
	arenaRight = Constants::BOSS_WORLD_WIDTH;
	arenaTop = 0.0f;
	arenaBottom = Constants::WORLD_HEIGHT;
	attackTimer = 0.9f;
	minionSpawnTimer = 4.0f;
	waitingForMinionBatchClear = false;
	pendingProjectileCount = 0;
	maxHealth = 300;
	health = maxHealth;
	scoreValue = 500;
	contactDamage = 24;
	detectionRange = 1600.0f;
	shootingRange = 1300.0f;
	stopDistance = 340.0f;
	moveSpeed = 70.0f;
	canMoveInAir = true;
	fallbackColor = sf::Color(190, 70, 70);
	width = 210.0f;
	height = 145.0f;
	body.setSize(sf::Vector2f(width, height));
	body.setFillColor(fallbackColor);
	body.setOutlineColor(sf::Color::Black);
	body.setOutlineThickness(3.0f);
}

Boss::~Boss()
{
}

bool Boss::loadStaticBossTexture(const char* fileName, float scale)
{
	if (!texture.loadFromFile(fileName))
	{
		usingSprite = false;
		std::cout << "Boss texture failed to load: " << fileName << std::endl;
		return false;
	}

	sf::Vector2u textureSize = texture.getSize();
	sprite.setTexture(texture);
	spriteScale = scale;
	spriteFacesLeft = true;
	usingSprite = true;
	width = static_cast<float>(textureSize.x) * spriteScale;
	height = static_cast<float>(textureSize.y) * spriteScale;
	body.setSize(sf::Vector2f(width, height));
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
	sf::FloatRect bounds = sprite.getGlobalBounds();
	sprite.setPosition(x + width / 2.0f - bounds.width / 2.0f, y + height - bounds.height);
}

void Boss::update(float deltaTime)
{
	if (contactDamageTimer > 0.0f)
	{
		contactDamageTimer -= deltaTime;
	}

	Entity::update(deltaTime);
	clampToArena();

	if (activeLevel != 0)
	{
		if (x < 0.0f)
		{
			x = 0.0f;
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
		Collectible* crate = new Collectible(COLLECTIBLE_TURKEY, getCenterX() - 16.0f, y - 32.0f);
		entities.addEntity(crate);
		waitingForMinionBatchClear = false;
		minionSpawnTimer = 7.0f;
	}
}

void Boss::draw(sf::RenderWindow& window)
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
	std::cout << getBossDisplayName() << " took " << amount << " damage, HP now "
		<< health << "/" << maxHealth << std::endl;
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

	std::cout << "Bullet hit " << getBossDisplayName() << " once, bullet destroyed." << std::endl;
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
	float ironScale = 1.20f;     // CHANGE THIS to make boss bigger/smaller
	float ironXOffset = 0.0f;    // negative = left, positive = right
	float ironYOffset = -200.0f;  // negative = up, positive = down

	width = 180.0f * ironScale;
	height = 121.0f * ironScale;

	moveSpeed = 0.0f;
	patrolDirectionTimer = 0.0f;
	fallbackColor = sf::Color(150, 92, 52);

	body.setSize(sf::Vector2f(width, height));
	body.setFillColor(fallbackColor);

	loadStaticBossTexture("Sprites/Clean/Iron_Nokana.png", ironScale);

	setSpawnPosition(startX + ironXOffset, worldY + ironYOffset);

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

	if (attackTimer > 0.0f)
	{
		attackTimer -= deltaTime;
	}
	if (minionSpawnTimer > 0.0f)
	{
		minionSpawnTimer -= deltaTime;
	}

	if (attackTimer <= 0.0f)
	{
		attack(player, entities);
		attackTimer = 1.65f;
	}
	updateMinionBatch(entities);
	if (minionSpawnTimer <= 0.0f && !waitingForMinionBatchClear)
	{
		spawnMinions(entities);
	}
}

void IronNokanaBoss::attack(PlayerSoldier& player, EntityManager& entities)
{
	(void)entities;
	float closeDistance = std::abs(player.getCenterX() - getCenterX());
	enqueueProjectile(0);
	if (closeDistance < 260.0f && player.getCenterY() > y + 40.0f)
	{
		enqueueProjectile(1);
	}
	if ((std::rand() % 3) == 0)
	{
		enqueueProjectile(2);
	}
}

void IronNokanaBoss::spawnMinions(EntityManager& entities)
{
	int count = 2 + std::rand() % 3;
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
		Enemy* minion = EnemyFactory::createEnemy(kind, x - 140.0f - i * 70.0f, y + height - 96.0f, target);
		entities.addEntity(minion);
	}
	waitingForMinionBatchClear = true;
	minionSpawnTimer = 6.0f;
}

void IronNokanaBoss::startRetreat()
{
	retreating = false;
	velocityX = 0.0f;
	velocityY = 0.0f;
}

void IronNokanaBoss::updateRetreat(float deltaTime)
{
	(void)deltaTime;
}

Projectile* IronNokanaBoss::buildProjectile(int projectileKind)
{
	float startX = facingRight ? x + width - 22.0f : x - 20.0f;
	float startY = y + 48.0f;
	if (projectileKind == 1)
	{
		return new EnemyBullet(startX, y + 94.0f, facingRight);
	}
	if (projectileKind == 2)
	{
		float vx = facingRight ? 250.0f : -250.0f;
		return new EnemyGrenadeProjectile(startX, y + 28.0f, vx, -500.0f);
	}
	return new EnemyRocketProjectile(startX, startY, aimedVelocityX(*this, target, 380.0f), aimedVelocityY(*this, target, 380.0f) - 80.0f);
}

const char* IronNokanaBoss::getEnemyName() const
{
	return "Iron Nokana";
}

SeaSatanBoss::SeaSatanBoss(float startX, float startY, bool finalPhaseBoss)
	: Boss(BossType::SeaSatan, finalPhaseBoss ? BiomeType::Merged : BiomeType::Aquatic, finalPhaseBoss)
{
	width = 364.0f * 1.0f;
	height = 227.0f * 1.0f;
	moveSpeed = 58.0f;
	patrolLeftBound = startX - 420.0f;
	patrolRightBound = startX + 420.0f;
	fallbackColor = sf::Color(45, 110, 170);
	body.setSize(sf::Vector2f(width, height));
	body.setFillColor(fallbackColor);
	loadStaticBossTexture("Sprites/Clean/Sea_Satan.png", 1.0f);
	setSpawnPosition(startX, startY);
	velocityX = -moveSpeed;
	updateBossVisual();
}

void SeaSatanBoss::update(float deltaTime, PlayerSoldier& player, EntityManager& entities, LevelManager& level)
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

	if (attackTimer > 0.0f)
	{
		attackTimer -= deltaTime;
	}
	if (minionSpawnTimer > 0.0f)
	{
		minionSpawnTimer -= deltaTime;
	}
	if (attackTimer <= 0.0f)
	{
		attack(player, entities);
		attackTimer = 1.35f;
	}
	updateMinionBatch(entities);
	if (minionSpawnTimer <= 0.0f && !waitingForMinionBatchClear)
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
	if ((std::rand() % 2) == 0)
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
	int count = 2 + std::rand() % 3;
	for (int i = 0; i < count; i += 1)
	{
		EnemyKind kind = (i % 2 == 0) ? ENEMY_SUB_VEHICLE : ENEMY_ZOMBIE;
		Enemy* minion = EnemyFactory::createEnemy(kind, x - 180.0f + i * 95.0f, y + 120.0f, target);
		entities.addEntity(minion);
	}
	waitingForMinionBatchClear = true;
	minionSpawnTimer = 7.0f;
}

void SeaSatanBoss::startRetreat()
{
	retreating = false;
	velocityY = 0.0f;
}

void SeaSatanBoss::updateRetreat(float deltaTime)
{
	(void)deltaTime;
}

Projectile* SeaSatanBoss::buildProjectile(int projectileKind)
{
	if (projectileKind == 0)
	{
		return new EnemyBullet(x - 18.0f, y + 48.0f, false);
	}
	if (projectileKind == 1)
	{
		return new EnemyBullet(x + width, y + 48.0f, true);
	}
	if (projectileKind == 3)
	{
		return new EnemyRocketProjectile(getCenterX(), y, 0.0f, -430.0f);
	}
	return new EnemyRocketProjectile(getCenterX(), getCenterY(), aimedVelocityX(*this, target, 360.0f), aimedVelocityY(*this, target, 360.0f));
}

const char* SeaSatanBoss::getEnemyName() const
{
	return "Sea Satan";
}

HairbusterRibertsBoss::HairbusterRibertsBoss(float startX, float startY, bool finalPhaseBoss)
	: Boss(BossType::HairbusterRiberts, finalPhaseBoss ? BiomeType::Merged : BiomeType::Aerial, finalPhaseBoss)
{
	width = 180.0f * 1.3f;
	height = 105.0f * 1.3f;
	moveSpeed = 82.0f;
	patrolLeftBound = startX - 500.0f;
	patrolRightBound = startX + 500.0f;
	fallbackColor = sf::Color(165, 165, 85);
	body.setSize(sf::Vector2f(width, height));
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

	if (attackTimer > 0.0f)
	{
		attackTimer -= deltaTime;
	}
	if (minionSpawnTimer > 0.0f)
	{
		minionSpawnTimer -= deltaTime;
	}
	if (attackTimer <= 0.0f)
	{
		attack(player, entities);
		attackTimer = 1.45f;
	}
	updateMinionBatch(entities);
	if (minionSpawnTimer <= 0.0f && !waitingForMinionBatchClear)
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
	if ((std::rand() % 2) == 0)
	{
		enqueueProjectile(2);
	}
}

void HairbusterRibertsBoss::spawnMinions(EntityManager& entities)
{
	int count = 2 + std::rand() % 3;
	for (int i = 0; i < count; i += 1)
	{
		EnemyKind kind = (i % 2 == 0) ? ENEMY_FLYING_TARA : ENEMY_MARTIAN;
		Enemy* minion = EnemyFactory::createEnemy(kind, x - 220.0f + i * 110.0f, y + 80.0f, target);
		entities.addEntity(minion);
	}
	waitingForMinionBatchClear = true;
	minionSpawnTimer = 7.5f;
}

void HairbusterRibertsBoss::startRetreat()
{
	retreating = false;
	velocityY = 0.0f;
}

void HairbusterRibertsBoss::updateRetreat(float deltaTime)
{
	(void)deltaTime;
}

Projectile* HairbusterRibertsBoss::buildProjectile(int projectileKind)
{
	if (projectileKind == 1)
	{
		return new EnemyGrenadeProjectile(getCenterX(), y + height - 10.0f, facingRight ? 120.0f : -120.0f, 120.0f);
	}
	if (projectileKind == 2)
	{
		return new EnemyRocketProjectile(getCenterX() + 36.0f, getCenterY(), aimedVelocityX(*this, target, 390.0f) + 70.0f, aimedVelocityY(*this, target, 390.0f));
	}
	return new EnemyRocketProjectile(getCenterX() - 36.0f, getCenterY(), aimedVelocityX(*this, target, 390.0f) - 70.0f, aimedVelocityY(*this, target, 390.0f));
}

const char* HairbusterRibertsBoss::getEnemyName() const
{
	return "Hairbuster Riberts";
}
