#include "EntityManager.h"

#include "AudioManager.h"
#include "Enemy.h"
#include "EnemyVehicle.h"
#include "Collectible.h"
#include "Level.h"
#include "PlayerSoldier.h"
#include "Prisoner.h"
#include "Projectile.h"
#include "ScoreManager.h"
#include "Soldier.h"
#include "Vehicle.h"
#include "EnemyTypes.h"
#include "Boss.h"

#include <cmath>
#include <cstdlib>
#include <ctime>

EntityManager::EntityManager()
{
	pendingScore = 0;
	activeLevel = 0;
	audioManager = 0;
	destroyedFlyingTara = 0;
	destroyedBradley = 0;
	destroyedEnemySub = 0;
	std::srand(static_cast<unsigned int>(std::time(0)));
}

EntityManager::~EntityManager()
{
	clear();
}

void EntityManager::setAudioManager(AudioManager* manager)
{
	audioManager = manager;
}

void EntityManager::addEntity(Entity* entity)
{
	if (entity != 0)
	{
		assignActiveLevel(entity);
		Projectile* projectile = dynamic_cast<Projectile*>(entity);
		if (projectile != 0)
		{
			if (audioManager != 0 && !projectile->isMelee() && !projectile->isExplosive())
			{
				audioManager->playSound(SFX_BULLET_FIRE);
			}
		}
		entities.pushBack(entity);
	}
}

void EntityManager::setActiveLevel(Level* level)
{
	activeLevel = level;
	for (int i = 0; i < entities.getSize(); i += 1)
	{
		assignActiveLevel(entities.get(i));
	}
}

void EntityManager::assignActiveLevel(Entity* entity)
{
	if (entity == 0)
	{
		return;
	}

	Soldier* soldier = dynamic_cast<Soldier*>(entity);
	if (soldier != 0)
	{
		soldier->setActiveLevel(activeLevel);
	}

	Enemy* enemy = dynamic_cast<Enemy*>(entity);
	if (enemy != 0)
	{
		enemy->setActiveLevel(activeLevel);
	}

	Vehicle* vehicle = dynamic_cast<Vehicle*>(entity);
	if (vehicle != 0)
	{
		vehicle->setActiveLevel(activeLevel);
	}

	Projectile* projectile = dynamic_cast<Projectile*>(entity);
	if (projectile != 0)
	{
		projectile->setActiveLevel(activeLevel);
	}

	Collectible* collectible = dynamic_cast<Collectible*>(entity);
	if (collectible != 0)
	{
		collectible->setActiveLevel(activeLevel);
	}

	Prisoner* prisoner = dynamic_cast<Prisoner*>(entity);
	if (prisoner != 0)
	{
		prisoner->setActiveLevel(activeLevel);
	}
}

void EntityManager::updateAll(float deltaTime)
{
	for (int i = 0; i < entities.getSize(); i += 1)
	{
		if (entities.get(i) != 0 && entities.get(i)->isActive())
		{
			entities.get(i)->update(deltaTime);
		}
	}

	int originalSize = entities.getSize();
	for (int i = 0; i < originalSize; i += 1)
	{
		Enemy* enemy = dynamic_cast<Enemy*>(entities.get(i));
		if (enemy != 0 && enemy->isActive())
		{
			Projectile* projectile = enemy->createProjectileIfReady();
			if (projectile != 0)
			{
				addEntity(projectile);
			}

			Entity* spawnedEntity = enemy->createSpawnedEntityIfReady();
			if (spawnedEntity != 0)
			{
				addEntity(spawnedEntity);
			}
		}
	}

	originalSize = entities.getSize();
	for (int i = 0; i < originalSize; i += 1)
	{
		Prisoner* prisoner = dynamic_cast<Prisoner*>(entities.get(i));
		if (prisoner != 0 && prisoner->isActive())
		{
			Collectible* crate = prisoner->createCrateIfReady();
			if (crate != 0)
			{
				addEntity(crate);
			}
		}
	}

	checkProjectileEnemyCollisions();
	checkProjectilePrisonerCollisions();
	checkProjectileVehicleCollisions();
	checkProjectilePlayerCollisions();
	checkEnemyVehicleCollisions();
	checkEnemyPlayerCollisions();
	checkPlayerCollectibleCollisions();
	removeInactive();
}

void EntityManager::checkProjectileEnemyCollisions()
{
	for (int i = 0; i < entities.getSize(); i += 1)
	{
		Projectile* projectile = dynamic_cast<Projectile*>(entities.get(i));
		if (projectile == 0 || !projectile->isActive() || !projectile->isPlayerOwned())
		{
			continue;
		}
		if (projectile->shouldIgnorePlayerProjectileVsEnemyChecks())
		{
			continue;
		}

		int killedByThisProjectile = 0;
		bool hitSomething = false;

		for (int j = 0; j < entities.getSize(); j += 1)
		{
			Enemy* enemy = dynamic_cast<Enemy*>(entities.get(j));
			if (enemy == 0 || !enemy->isActive())
			{
				continue;
			}

			if (projectile->getBounds().intersects(enemy->getBounds()))
			{
				hitSomething = true;
				if (projectile->isExplosive())
				{
					float centerX = projectile->getCenterX();
					float centerY = projectile->getCenterY();
					for (int k = 0; k < entities.getSize(); k += 1)
					{
						Enemy* nearbyEnemy = dynamic_cast<Enemy*>(entities.get(k));
						if (nearbyEnemy == 0 || !nearbyEnemy->isActive())
						{
							continue;
						}

						float dx = nearbyEnemy->getCenterX() - centerX;
						float dy = nearbyEnemy->getCenterY() - centerY;
						float distance = std::sqrt(dx * dx + dy * dy);
						if (distance <= projectile->getBlastRadius())
						{
							bool damageApplied = nearbyEnemy->applyProjectileHit(*projectile);
							if (damageApplied && nearbyEnemy->isDead())
							{
								killedByThisProjectile += 1;
								handleEnemyKilled(nearbyEnemy);
							}
						}
					}
				}
				else
				{
					bool damageApplied = enemy->applyProjectileHit(*projectile);
					if (damageApplied && enemy->isDead())
					{
						killedByThisProjectile += 1;
						handleEnemyKilled(enemy);
					}
				}
				if (!projectile->deferProjectileDeactivateAfterEnemyHit())
				{
					projectile->onCollision();
				}
				break;
			}
		}

		if (hitSomething)
		{
			if (projectile->isMelee() && killedByThisProjectile > 0)
			{
				pendingScore += ScoreManager::meleeBonus();
			}
			if (projectile->wasFiredWhileAirborne() && killedByThisProjectile > 0)
			{
				pendingScore += ScoreManager::aerialBonus();
			}
			if (projectile->isExplosive())
			{
				pendingScore += ScoreManager::multiKillBonus(killedByThisProjectile);
			}
		}
	}
}

void EntityManager::checkProjectilePrisonerCollisions()
{
	for (int i = 0; i < entities.getSize(); i += 1)
	{
		Projectile* projectile = dynamic_cast<Projectile*>(entities.get(i));
		if (projectile == 0 || !projectile->isActive() || !projectile->isPlayerOwned())
		{
			continue;
		}
		if (projectile->shouldIgnorePlayerProjectileVsEnemyChecks())
		{
			continue;
		}

		for (int j = 0; j < entities.getSize(); j += 1)
		{
			Prisoner* prisoner = dynamic_cast<Prisoner*>(entities.get(j));
			if (prisoner == 0 || !prisoner->isActive() || !prisoner->canBeFreed())
			{
				continue;
			}

			if (projectile->getBounds().intersects(prisoner->getBounds()))
			{
				prisoner->freePrisoner();
				projectile->onCollision();
				break;
			}
		}
	}
}

void EntityManager::checkProjectileVehicleCollisions()
{
	for (int i = 0; i < entities.getSize(); i += 1)
	{
		Projectile* projectile = dynamic_cast<Projectile*>(entities.get(i));
		if (projectile == 0 || !projectile->isActive() || projectile->isPlayerOwned())
		{
			continue;
		}

		for (int j = 0; j < entities.getSize(); j += 1)
		{
			Vehicle* vehicle = dynamic_cast<Vehicle*>(entities.get(j));
			if (vehicle == 0 || !vehicle->isActive())
			{
				continue;
			}

			if (projectile->getBounds().intersects(vehicle->getBounds()))
			{
				vehicle->takeDamage(projectile->getDamage());
				projectile->onCollision();
				break;
			}
		}
	}
}

void EntityManager::checkProjectilePlayerCollisions()
{
	for (int i = 0; i < entities.getSize(); i += 1)
	{
		Projectile* projectile = dynamic_cast<Projectile*>(entities.get(i));
		if (projectile == 0 || !projectile->isActive() || projectile->isPlayerOwned())
		{
			continue;
		}

		for (int j = 0; j < entities.getSize(); j += 1)
		{
			PlayerSoldier* player = dynamic_cast<PlayerSoldier*>(entities.get(j));
			if (player == 0 || !player->isActive() || player->isDead() || player->isRidingVehicle())
			{
				continue;
			}

			if (projectile->getBounds().intersects(player->getBounds()))
			{
				player->takeDamage(projectile->getDamage());
				projectile->onCollision();
				break;
			}
		}
	}
}

void EntityManager::checkEnemyVehicleCollisions()
{
	for (int i = 0; i < entities.getSize(); i += 1)
	{
		Enemy* enemy = dynamic_cast<Enemy*>(entities.get(i));
		if (enemy == 0 || !enemy->isActive())
		{
			continue;
		}

		for (int j = 0; j < entities.getSize(); j += 1)
		{
			Vehicle* vehicle = dynamic_cast<Vehicle*>(entities.get(j));
			if (vehicle == 0 || !vehicle->isActive())
			{
				continue;
			}

			if (enemy->getBounds().intersects(vehicle->getBounds()) && enemy->canDealContactDamage())
			{
				vehicle->takeDamage(enemy->getContactDamage());
				enemy->restartContactDamageCooldown();
			}
		}
	}
}

void EntityManager::checkEnemyPlayerCollisions()
{
	for (int i = 0; i < entities.getSize(); i += 1)
	{
		Enemy* enemy = dynamic_cast<Enemy*>(entities.get(i));
		if (enemy == 0 || !enemy->isActive())
		{
			continue;
		}

		for (int j = 0; j < entities.getSize(); j += 1)
		{
			PlayerSoldier* player = dynamic_cast<PlayerSoldier*>(entities.get(j));
			if (player == 0 || !player->isActive() || player->isDead() || player->isRidingVehicle())
			{
				continue;
			}

			if (enemy->getBounds().intersects(player->getBounds()) && enemy->canDealContactDamage())
			{
				if (dynamic_cast<ZombieEnemy*>(enemy) != 0)
				{
					player->zombify();
				}
				else if (dynamic_cast<MummyEnemy*>(enemy) != 0)
				{
					player->mummify();
				}
				else
				{
					player->takeDamage(enemy->getContactDamage());
				}
				enemy->restartContactDamageCooldown();
			}
		}
	}
}

void EntityManager::checkPlayerCollectibleCollisions()
{
	for (int i = 0; i < entities.getSize(); i += 1)
	{
		PlayerSoldier* player = dynamic_cast<PlayerSoldier*>(entities.get(i));
		if (player == 0 || !player->isActive() || player->isDead())
		{
			continue;
		}

		for (int j = 0; j < entities.getSize(); j += 1)
		{
			Collectible* collectible = dynamic_cast<Collectible*>(entities.get(j));
			if (collectible == 0 || !collectible->isActive())
			{
				continue;
			}

			if (player->getBounds().intersects(collectible->getBounds()))
			{
				collectible->apply(*player);
				if (audioManager != 0)
				{
					audioManager->playSound(SFX_COLLECT);
				}
			}
		}
	}
}

void EntityManager::handleEnemyKilled(Enemy* enemy)
{
	if (enemy == 0 || enemy->hasProcessedDeath())
	{
		return;
	}

	enemy->markDeathProcessed();

	pendingScore += enemy->getScoreValue();
	spawnDropForEnemy(*enemy);

	MartianPod* pod = dynamic_cast<MartianPod*>(enemy);
	if (pod != 0)
	{
		return;
	}

	MartianInfantry* infantry = dynamic_cast<MartianInfantry*>(enemy);
	if (infantry == 0)
	{
		enemy->deactivate();
	}
}

void EntityManager::spawnDropForEnemy(const Enemy& enemy)
{
	int roll = std::rand() % 100;
	CollectibleKind dropKind;
	if (roll < 5)
	{
		dropKind = COLLECTIBLE_ROCKET_ITEM;
	}
	else if (roll < 10)
	{
		dropKind = COLLECTIBLE_HMG_ITEM;
	}
	else if (roll < 20)
	{
		dropKind = COLLECTIBLE_TURKEY;
	}
	else if (roll < 40)
	{
		dropKind = COLLECTIBLE_FRUIT;
	}
	else
	{
		return;
	}


	float spawnX = enemy.getCenterX() - 12.0f;
	float spawnY = enemy.getY();
	Collectible* drop = new Collectible(dropKind, spawnX, spawnY);
	addEntity(drop);
}

void EntityManager::drawAll(sf::RenderWindow& window)
{
	for (int i = 0; i < entities.getSize(); i += 1)
	{
		if (entities.get(i) != 0 && entities.get(i)->isActive())
		{
			entities.get(i)->draw(window);
		}
	}
}

int EntityManager::collectPendingScore()
{
	int score = pendingScore;
	pendingScore = 0;
	return score;
}

void EntityManager::addBonusScore(int score)
{
	if (score > 0)
	{
		pendingScore += score;
	}
}

int EntityManager::countActiveEnemies() const
{
	int count = 0;
	for (int i = 0; i < entities.getSize(); i += 1)
	{
		Enemy* enemy = dynamic_cast<Enemy*>(entities.get(i));
		if (enemy != 0 && enemy->isActive())
		{
			count += 1;
		}
	}

	return count;
}

int EntityManager::countActiveNonBossEnemies() const
{
	int count = 0;
	for (int i = 0; i < entities.getSize(); i += 1)
	{
		Enemy* enemy = dynamic_cast<Enemy*>(entities.get(i));
		Boss* boss = dynamic_cast<Boss*>(entities.get(i));
		if (enemy != 0 && boss == 0 && enemy->isActive())
		{
			count += 1;
		}
	}

	return count;
}

PlayerSoldier* EntityManager::getPlayer() const
{
	for (int i = 0; i < entities.getSize(); i += 1)
	{
		PlayerSoldier* player = dynamic_cast<PlayerSoldier*>(entities.get(i));
		if (player != 0)
		{
			return player;
		}
	}

	return 0;
}

Vehicle* EntityManager::getVehicle() const
{
	for (int i = 0; i < entities.getSize(); i += 1)
	{
		Vehicle* vehicle = dynamic_cast<Vehicle*>(entities.get(i));
		if (vehicle != 0 && vehicle->isActive())
		{
			return vehicle;
		}
	}

	return 0;
}

Vehicle* EntityManager::getClosestVehicle(float x, float y) const
{
	Vehicle* closest = 0;
	float minDist = 1e9f;

	for (int i = 0; i < entities.getSize(); i += 1)
	{
		Vehicle* vehicle = dynamic_cast<Vehicle*>(entities.get(i));
		if (vehicle != 0 && vehicle->isActive())
		{
			float dx = vehicle->getCenterX() - x;
			float dy = vehicle->getCenterY() - y;
			float distSq = dx * dx + dy * dy;
			if (distSq < minDist)
			{
				minDist = distSq;
				closest = vehicle;
			}
		}
	}

	return closest;
}

int EntityManager::getDestroyedFlyingTaraCount() const
{
	return destroyedFlyingTara;
}

int EntityManager::getDestroyedBradleyCount() const
{
	return destroyedBradley;
}

int EntityManager::getDestroyedEnemySubCount() const
{
	return destroyedEnemySub;
}

void EntityManager::removeEnemiesBehind(float minimumX)
{
	for (int i = 0; i < entities.getSize(); i += 1)
	{
		Enemy* enemy = dynamic_cast<Enemy*>(entities.get(i));
		if (enemy != 0 && enemy->getX() < minimumX)
		{
			enemy->deactivate();
		}
	}
}

void EntityManager::clearBossPhaseTransientEntities()
{
	for (int i = 0; i < entities.getSize(); i += 1)
	{
		Enemy* enemy = dynamic_cast<Enemy*>(entities.get(i));
		Projectile* projectile = dynamic_cast<Projectile*>(entities.get(i));
		if (enemy != 0 || projectile != 0)
		{
			delete entities.get(i);
			entities.removeAt(i);
			i -= 1;
		}
	}
}

void EntityManager::removeInactive()
{
	for (int i = 0; i < entities.getSize(); i += 1)
	{
		if (entities.get(i) == 0 || !entities.get(i)->isActive())
		{
			EnemyVehicle* enemyVehicle = dynamic_cast<EnemyVehicle*>(entities.get(i));
			if (enemyVehicle != 0 && enemyVehicle->isDead())
			{
				EnemyVehicleType type = enemyVehicle->getVehicleType();
				if (type == ENEMY_VEHICLE_FLYING_TARA)
				{
					destroyedFlyingTara += 1;
				}
				else if (type == ENEMY_VEHICLE_M15A_BRADLEY)
				{
					destroyedBradley += 1;
				}
				else if (type == ENEMY_VEHICLE_SUB)
				{
					destroyedEnemySub += 1;
				}
			}

			Vehicle* vehicle = dynamic_cast<Vehicle*>(entities.get(i));
			if (vehicle != 0 && vehicle->isOccupied())
			{
				PlayerSoldier* player = getPlayer();
				if (player != 0)
				{
					vehicle->ejectPlayer(*player);
				}
			}

			delete entities.get(i);
			entities.removeAt(i);
			i -= 1;
		}
	}
}

void EntityManager::clear()
{
	for (int i = 0; i < entities.getSize(); i += 1)
	{
		delete entities.get(i);
	}

	entities.clear();
	destroyedFlyingTara = 0;
	destroyedBradley = 0;
	destroyedEnemySub = 0;
}
