#include "EntityManager.h"

#include "Enemy.h"
#include "PlayerSoldier.h"
#include "Projectile.h"

EntityManager::EntityManager()
{
	pendingScore = 0;
}

EntityManager::~EntityManager()
{
	clear();
}

void EntityManager::addEntity(Entity* entity)
{
	if (entity != 0)
	{
		entities.pushBack(entity);
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

	checkProjectileEnemyCollisions();
	checkEnemyPlayerCollisions();
	removeInactive();
}

void EntityManager::checkProjectileEnemyCollisions()
{
	for (int i = 0; i < entities.getSize(); i += 1)
	{
		Projectile* projectile = dynamic_cast<Projectile*>(entities.get(i));
		if (projectile == 0 || !projectile->isActive())
		{
			continue;
		}

		for (int j = 0; j < entities.getSize(); j += 1)
		{
			Enemy* enemy = dynamic_cast<Enemy*>(entities.get(j));
			if (enemy == 0 || !enemy->isActive())
			{
				continue;
			}

			if (projectile->getBounds().intersects(enemy->getBounds()))
			{
				enemy->takeDamage(projectile->getDamage());
				if (enemy->isDead())
				{
					pendingScore += enemy->getScoreValue();
				}
				projectile->deactivate();
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
