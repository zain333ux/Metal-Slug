#include "M15ABradley.h"



#include "Level.h"

#include "PlayerSoldier.h"

#include "VehicleSpecialProjectiles.h"

#include "VehicleTextureManager.h"



#include <cmath>


using namespace std;
using namespace sf;



M15ABradley::M15ABradley()

{

	maxHealth = 7;

	health = maxHealth;

	scoreValue = 320;

	contactDamage = 18;

	width = 128;

	height = 84;

	moveSpeed = 60;

	detectionRange = 980;

	attackCooldown = 2.4f;

	preferredRange = 500;

	fallbackColor = Color(145, 135, 95);

	body.setSize(Vector2f(width, height));

	usingSprite = false;



	bradleySpritesLoaded = false;

	buildSprites();

}



void M15ABradley::buildSprites()

{

	const Texture* idleTex = VehicleTextureManager::instance().getTexture(VehicleTextureId::BradleyIdle);

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

		body.setFillColor(Color::Transparent);

		idleAnim.setFacingRight(!facingRight);

		idleAnim.update(deltaTime);

	}

}



void M15ABradley::draw(RenderWindow& window)

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

		if (healthRatio < 0)

		{

			healthRatio = 0;

		}



		RectangleShape healthBack;

		healthBack.setPosition(x, y - 12);

		healthBack.setSize(Vector2f(width, 6));

		healthBack.setFillColor(Color(80, 20, 20));

		window.draw(healthBack);



		RectangleShape healthFront;

		healthFront.setPosition(x, y - 12);

		healthFront.setSize(Vector2f(width * healthRatio, 6));

		healthFront.setFillColor(Color(60, 220, 90));

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

	float absDx = abs(dx);

	facingRight = dx > 0;



	if (activeLevel != 0)

	{

		float terrainAhead = activeLevel->getMainGroundYAt(x + (facingRight ? width + 20 : -20));

		float terrainNow = activeLevel->getMainGroundYAt(getCenterX());

		bool badStep = terrainAhead > terrainNow + 40;

		bool wateryAhead =

			activeLevel->isWaterInBounds(x + (facingRight ? width : -8), x + (facingRight ? width + 10 : 8), y + height * 0.5f, y + height);

		if (!badStep && !wateryAhead)

		{

			if (absDx > preferredRange + 120)

			{

				if (dx > 0)

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



	if (absDx <= detectionRange && attackTimer <= 0)

	{

		queuedAttack = true;

		attackTimer = attackCooldown;

	}

}



Projectile* M15ABradley::attack()

{

	float spawnX = facingRight ? x + width + 8 : x - 42;

	float spawnY = y + 30;

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

