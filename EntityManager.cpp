#include "EntityManager.h"

#include "Enemy.h"
#include "Collectible.h"
#include "PlayerSoldier.h"
#include "Projectile.h"
#include "ScoreManager.h"
#include "Soldier.h"

#include <cmath>

EntityManager::EntityManager()
{
	pendingScore = 0;
	activeLevel = 0;
}

EntityManager::~EntityManager()
{
	clear();
}

void EntityManager::addEntity(Entity* entity)
{
	if (entity != 0)
	{
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
		entities.pushBack(entity);
	}
}

void EntityManager::setActiveLevel(Level* level)
{
	activeLevel = level;
	for (int i = 0; i < entities.getSize(); i += 1)
	{
		Soldier* soldier = dynamic_cast<Soldier*>(entities.get(i));
		if (soldier != 0)
		{
			soldier->setActiveLevel(activeLevel);
		}
		Enemy* enemy = dynamic_cast<Enemy*>(entities.get(i));
		if (enemy != 0)
		{
			enemy->setActiveLevel(activeLevel);
		}
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

	checkProjectileEnemyCollisions();
	checkProjectilePlayerCollisions();
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
								pendingScore += nearbyEnemy->getScoreValue();
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
						pendingScore += enemy->getScoreValue();
					}
				}
				projectile->onCollision();
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
			if (player == 0 || !player->isActive() || player->isDead())
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
			if (player == 0 || !player->isActive() || player->isDead())
			{
				continue;
			}

			if (enemy->getBounds().intersects(player->getBounds()) && enemy->canDealContactDamage())
			{
				player->takeDamage(enemy->getContactDamage());
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
			}
		}
	}
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

void EntityManager::removeInactive()
{
	for (int i = 0; i < entities.getSize(); i += 1)
	{
		if (entities.get(i) == 0 || !entities.get(i)->isActive())
		{
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
}
