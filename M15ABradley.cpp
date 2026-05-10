#include "M15ABradley.h"



#include "Level.h"

#include "PlayerSoldier.h"

#include "VehicleSpecialProjectiles.h"

#include "VehicleTextureManager.h"



#include <cmath>



M15ABradley::M15ABradley()

{

	maxHealth = 7;

	health = maxHealth;

	scoreValue = 320;

	contactDamage = 18;

	width = 128.0f;

	height = 84.0f;

	moveSpeed = 60.0f;

	detectionRange = 980.0f;

	attackCooldown = 2.4f;

	preferredRange = 500.0f;

	fallbackColor = sf::Color(145, 135, 95);

	body.setSize(sf::Vector2f(width, height));

	usingSprite = false;



	bradleySpritesLoaded = false;

	buildSprites();

}



void M15ABradley::buildSprites()

{

	const sf::Texture* idleTex = VehicleTextureManager::instance().getTexture(VehicleTextureId::BradleyIdle);

	if (idleTex == nullptr)

	{

		return;

	}



	static FrameRect rects[2];

	rects[0] = FrameRect{0, 0, 83, 54, 0, 0};

	rects[1] = FrameRect{83, 0, 83, 54, 0, 0};

	idleAnim.setTexture(idleTex);

	idleAnim.setFrames(rects, 2);

	idleAnim.setFrameTime(0.14f);

	idleAnim.setLoop(true);

	idleAnim.setScale(1.58f, 1.58f);

	idleAnim.reset();



	bradleySpritesLoaded = true;

}



void M15ABradley::update(float deltaTime)

{

	buildSprites();

	Enemy::update(deltaTime);



	if (bradleySpritesLoaded)

	{

		body.setFillColor(sf::Color::Transparent);

		idleAnim.setFacingRight(!facingRight);

		idleAnim.update(deltaTime);

	}

}



void M15ABradley::draw(sf::RenderWindow& window)

{

	if (!visible)

	{

		return;

	}



	if (!bradleySpritesLoaded)

	{

		Enemy::draw(window);

		return;

	}



	idleAnim.setFacingRight(!facingRight);

	idleAnim.drawAtAnchor(window, x + width * 0.5f, y + height);



	if (maxHealth > 0)

	{

		float healthRatio = static_cast<float>(health) / static_cast<float>(maxHealth);

		if (healthRatio < 0.0f)

		{

			healthRatio = 0.0f;

		}



		sf::RectangleShape healthBack;

		healthBack.setPosition(x, y - 12.0f);

		healthBack.setSize(sf::Vector2f(width, 6.0f));

		healthBack.setFillColor(sf::Color(80, 20, 20));

		window.draw(healthBack);



		sf::RectangleShape healthFront;

		healthFront.setPosition(x, y - 12.0f);

		healthFront.setSize(sf::Vector2f(width * healthRatio, 6.0f));

		healthFront.setFillColor(sf::Color(60, 220, 90));

		window.draw(healthFront);

	}

}



void M15ABradley::updateAI()

{

	stopMoving();



	if (target == 0 || !target->isActive() || target->isDead())

	{

		return;

	}



	float dx = target->getCenterX() - getCenterX();

	float absDx = std::abs(dx);

	facingRight = dx > 0.0f;



	if (activeLevel != 0)

	{

		float terrainAhead = activeLevel->getMainGroundYAt(x + (facingRight ? width + 20.0f : -20.0f));

		float terrainNow = activeLevel->getMainGroundYAt(getCenterX());

		bool badStep = terrainAhead > terrainNow + 40.0f;

		bool wateryAhead =

			activeLevel->isWaterInBounds(x + (facingRight ? width : -8.0f), x + (facingRight ? width + 10.0f : 8.0f), y + height * 0.5f, y + height);

		if (!badStep && !wateryAhead)

		{

			if (absDx > preferredRange + 120.0f)

			{

				if (dx > 0.0f)

				{

					moveRight();

				}

				else

				{

					moveLeft();

				}

			}

		}

	}



	if (absDx <= detectionRange && attackTimer <= 0.0f)

	{

		queuedAttack = true;

		attackTimer = attackCooldown;

	}

}



Projectile* M15ABradley::attack()

{

	float spawnX = facingRight ? x + width + 8.0f : x - 42.0f;

	float spawnY = y + 30.0f;

	return new EnemyStraightRocketProjectile(spawnX, spawnY, facingRight);

}



const char* M15ABradley::getType() const

{

	return "M15ABradley";

}



EnemyVehicleType M15ABradley::getVehicleType() const

{

	return ENEMY_VEHICLE_M15A_BRADLEY;

}



const char* M15ABradley::getEnemyName() const

{

	return "M-15A Bradley";

}


