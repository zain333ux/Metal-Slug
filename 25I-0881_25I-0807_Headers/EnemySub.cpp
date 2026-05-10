#include "EnemySub.h"



#include "Level.h"

#include "PlayerSoldier.h"

#include "VehicleSpecialProjectiles.h"

#include "VehicleTextureManager.h"



#include <cmath>


using namespace std;
using namespace sf;



namespace

{

	void distributeStrip(int sheetW, int sheetH, int frameCount, int refW, FrameRect* out)

	{

		int xAcc = 0;

		const int baseW = sheetW / frameCount;

		const int rem = sheetW - baseW * frameCount;

		for (int i = 0; i < frameCount; i += 1)

		{

			const int fw = baseW + (i < rem ? 1 : 0);

			const int ox = (refW - fw) / 2;

			out[i] = FrameRect{xAcc, 0, fw, sheetH, ox, 0};

			xAcc += fw;

		}

	}

} // namespace



EnemySub::EnemySub()

{

	maxHealth = 7;

	health = maxHealth;

	scoreValue = 320;

	contactDamage = 16;

	width = 134;

	height = 70;

	moveSpeed = 92;

	verticalSpeed = 36;

	attackCooldown = 1.9f;

	canMoveInAir = true;

	fallbackColor = Color(78, 130, 170);

	body.setSize(Vector2f(width, height));

	usingSprite = false;



	damageFlashTimer = 0;



	dyingPlaying = false;

	turnPlaying = false;

	subSpritesLoaded = false;



	buildSpritesIfNeeded();

}



void EnemySub::buildSpritesIfNeeded()

{

	if (subSpritesLoaded)

	{

		return;

	}



	const Texture* moveTex = VehicleTextureManager::instance().getTexture(VehicleTextureId::EnemySubMove);

	const Texture* turnTex = VehicleTextureManager::instance().getTexture(VehicleTextureId::EnemySubTurn);

	const Texture* deathTex = VehicleTextureManager::instance().getTexture(VehicleTextureId::EnemySubDeath);



	static FrameRect moveRects[7];

	static FrameRect turnRects[10];

	static FrameRect deathRects[6];



	if (moveTex == nullptr || turnTex == nullptr || deathTex == nullptr)

	{

		return;

	}



	const Vector2u mz = moveTex->getSize();

	const int refMove = mz.x / 7 + 1;

	distributeStrip(static_cast<int>(mz.x), static_cast<int>(mz.y), 7, refMove, moveRects);

	moveAnim.setTexture(moveTex);

	moveAnim.setFrames(moveRects, 7);

	moveAnim.setFrameTime(0.09f);

	moveAnim.setLoop(true);

	moveAnim.setScale(1.42f, 1.42f);

	moveAnim.reset();



	const Vector2u tz = turnTex->getSize();

	const int refTurn = tz.x / 10 + 2;

	distributeStrip(static_cast<int>(tz.x), static_cast<int>(tz.y), 10, refTurn, turnRects);

	turnAnim.setTexture(turnTex);

	turnAnim.setFrames(turnRects, 10);

	turnAnim.setFrameTime(0.055f);

	turnAnim.setLoop(false);

	turnAnim.setPlayBackward(false);

	turnAnim.setScale(1.42f, 1.42f);



	const Vector2u dz = deathTex->getSize();

	const int refDeath = dz.x / 6 + 2;

	distributeStrip(static_cast<int>(dz.x), static_cast<int>(dz.y), 6, refDeath, deathRects);

	deathAnim.setTexture(deathTex);

	deathAnim.setFrames(deathRects, 6);

	deathAnim.setFrameTime(0.1f);

	deathAnim.setLoop(false);

	deathAnim.setScale(1.42f, 1.42f);

	deathAnim.reset();



	subSpritesLoaded = true;

}



void EnemySub::updateAI()

{

	if (dyingPlaying || turnPlaying)

	{

		stopMoving();

		velocityY = 0;

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



	float desiredY = y;

	if (activeLevel != 0)

	{

		float waterY = activeLevel->getWaterSurfaceYAt(getCenterX());

		desiredY = waterY + 70;

		if (target != 0 && !target->isDead() && target->getCenterY() > waterY + 10)

		{

			desiredY = target->getCenterY() - height * 0.55f;

		}

		float floorY = activeLevel->getMainGroundYAt(getCenterX()) - height - 14;

		if (desiredY > floorY)

		{

			desiredY = floorY;

		}

		if (desiredY < waterY + 20)

		{

			desiredY = waterY + 20;

		}

	}



	float dy = desiredY - y;

	velocityY = dy * 1.8f;

	if (velocityY > verticalSpeed)

	{

		velocityY = verticalSpeed;

	}

	if (velocityY < -verticalSpeed)

	{

		velocityY = -verticalSpeed;

	}



	if (target != 0 && target->isActive() && !target->isDead())

	{

		float dx = target->getCenterX() - getCenterX();

		facingRight = dx > 0;

		if (abs(dx) <= 720 && attackTimer <= 0)

		{

			queuedAttack = true;

			attackTimer = attackCooldown;

		}

	}

}



void EnemySub::takeDamage(int damage)

{

	if (dyingPlaying)

	{

		return;

	}



	if (damage <= 0)

	{

		return;

	}



	health -= damage;

	damageFlashTimer = 0.16f;



	if (health <= 0)

	{

		health = 0;

		dyingPlaying = true;

		queuedAttack = false;

		turnPlaying = false;

		velocityX = 0;

		velocityY = 0;

		deathAnim.reset();

		deathAnim.setFinished(false);

	}

}



bool EnemySub::applyProjectileHit(Projectile& projectile)

{

	if (dyingPlaying)

	{

		return false;

	}



	return Enemy::applyProjectileHit(projectile);

}



bool EnemySub::canDealContactDamage() const

{

	if (dyingPlaying || health <= 0)

	{

		return false;

	}



	return Enemy::canDealContactDamage();

}



Projectile* EnemySub::createProjectileIfReady()

{

	if (dyingPlaying || turnPlaying)

	{

		return 0;

	}



	return EnemyVehicle::createProjectileIfReady();

}



Projectile* EnemySub::attack()

{

	float spawnX = facingRight ? x + width + 10 : x - 44;

	float spawnY = y + height * 0.55f;

	return new EnemyStraightRocketProjectile(spawnX, spawnY, facingRight);

}



void EnemySub::update(float deltaTime)

{

	buildSpritesIfNeeded();



	if (damageFlashTimer > 0)

	{

		damageFlashTimer -= deltaTime;

	}



	if (contactDamageTimer > 0)

	{

		contactDamageTimer -= deltaTime;

	}

	if (attackTimer > 0)

	{

		attackTimer -= deltaTime;

	}



	if (dyingPlaying)

	{

		deathAnim.update(deltaTime);

		if (deathAnim.isFinished())

		{

			deactivate();

		}

		body.setPosition(x, y);

		return;

	}



	const int patrolSnap = patrolDirection;



	updateAI();



	Entity::update(deltaTime);



	if (x < 0)

	{

		x = 0;

		patrolDirection = 1;

	}

	if (x + width > movementMaxX)

	{

		x = movementMaxX - width;

		patrolDirection = -1;

	}



	if (activeLevel != 0)

	{

		float waterY = activeLevel->getWaterSurfaceYAt(getCenterX());

		float minY = waterY + 18;

		float maxY = activeLevel->getMainGroundYAt(getCenterX()) - height - 12;

		if (maxY < minY + 10)

		{

			maxY = minY + 10;

		}

		if (y < minY)

		{

			y = minY;

		}

		if (y > maxY)

		{

			y = maxY;

		}

	}



	bool dirChangedOverall = patrolDirection != patrolSnap;

	if (!turnPlaying && dirChangedOverall)

	{

		turnPlaying = true;

		queuedAttack = false;

		velocityX = 0;

		velocityY = 0;

		turnAnim.reset();

		turnAnim.setFinished(false);

	}



	if (turnPlaying)

	{

		velocityX = 0;

		velocityY = 0;

		turnAnim.update(deltaTime);



		if (turnAnim.isFinished())

		{

			turnPlaying = false;

			facingRight = patrolDirection > 0;

			turnAnim.reset();

			turnAnim.setFinished(false);

			moveAnim.reset();

		}



		body.setPosition(x, y);

		return;

	}



	moveAnim.setFacingRight(!facingRight);

	moveAnim.update(deltaTime);



	body.setPosition(x, y);

}



void EnemySub::draw(RenderWindow& window)

{

	if (!visible)

	{

		return;

	}



	if (!subSpritesLoaded)

	{

		Enemy::draw(window);

		return;

	}



	const Color flashTint(255, 155, 155);

	const Color neutral(255, 255, 255);

	bool flash = damageFlashTimer > 0;

	const Color moveTint = (!dyingPlaying && flash) ? flashTint : neutral;



	if (dyingPlaying)

	{

		deathAnim.setTint(flash ? flashTint : neutral);

		deathAnim.setFacingRight(!facingRight);

		deathAnim.drawAtAnchor(window, x + width * 0.5f, y + height);

	}

	else if (turnPlaying)

	{

		turnAnim.setTint(moveTint);

		turnAnim.setFacingRight(!facingRight);

		turnAnim.drawAtAnchor(window, x + width * 0.5f, y + height);

	}

	else

	{

		moveAnim.setTint(moveTint);

		moveAnim.setFacingRight(!facingRight);

		moveAnim.drawAtAnchor(window, x + width * 0.5f, y + height);

	}



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



const char* EnemySub::getType() const

{

	return "EnemySub";

}



EnemyVehicleType EnemySub::getVehicleType() const

{

	return ENEMY_VEHICLE_SUB;

}



const char* EnemySub::getEnemyName() const

{

	return "Enemy Sub";

}

