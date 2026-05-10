#include "FlyingTara.h"



#include "Level.h"

#include "PlayerSoldier.h"

#include "VehicleSpecialProjectiles.h"

#include "VehicleTextureManager.h"



#include <cmath>


using namespace std;
using namespace sf;



FlyingTara::FlyingTara()

{

	maxHealth = 8;

	health = maxHealth;

	scoreValue = 220;

	contactDamage = 12;

	width = 82;

	height = 58;

	moveSpeed = 105;

	canMoveInAir = true;

	attackCooldown = 1.9f;

	hoverY = y;

	fallbackColor = Color(195, 105, 230);

	body.setSize(Vector2f(width, height));

	usingSprite = false;



	visibleKind = FlyingTaraAnimKind::Idle;

	taraSpritesLoaded = false;

	damageFlashTimer = 0;

	lastVerticalVelocity = 0;



	buildSprites();

}



void FlyingTara::buildSprites()

{

	const Texture* idleTex = VehicleTextureManager::instance().getTexture(VehicleTextureId::TaraIdle);

	const Texture* mdTex = VehicleTextureManager::instance().getTexture(VehicleTextureId::TaraMoveDown);

	const Texture* muTex = VehicleTextureManager::instance().getTexture(VehicleTextureId::TaraMoveUp);



	taraSpritesLoaded = false;



	if (idleTex == nullptr || mdTex == nullptr || muTex == nullptr)

	{

		return;

	}



	static FrameRect idleR = FrameRect{0, 0, 80, 38, 0, 0};

	animIdle.setTexture(idleTex);

	animIdle.setFrames(&idleR, 1);

	animIdle.setFrameTime(0.25f);

	animIdle.setLoop(true);

	animIdle.setScale(1.62f, 1.62f);

	animIdle.reset();



	static const int muW[] = {82, 82};

	static FrameRect muRects[2];

	muRects[0] = FrameRect{0, 0, muW[0], 43, (82 - muW[0]) / 2, 0};

	muRects[1] = FrameRect{muW[0], 0, muW[1], 43, (82 - muW[1]) / 2, 0};



	animUp.setTexture(muTex);

	animUp.setFrames(muRects, 2);

	animUp.setFrameTime(0.12f);

	animUp.setLoop(true);

	animUp.setScale(1.62f, 1.62f);

	animUp.reset();



	static const int mdW[] = {82, 83};

	static FrameRect mdRects[2];

	mdRects[0] = FrameRect{0, 0, mdW[0], 44, (83 - mdW[0]) / 2, 0};

	mdRects[1] = FrameRect{mdW[0], 0, mdW[1], 44, (83 - mdW[1]) / 2, 0};



	animDown.setTexture(mdTex);

	animDown.setFrames(mdRects, 2);

	animDown.setFrameTime(0.12f);

	animDown.setLoop(true);

	animDown.setScale(1.62f, 1.62f);

	animDown.reset();



	taraSpritesLoaded = true;

}



void FlyingTara::updateAI()

{

	velocityY = 0;



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



	float wave = sin(x * 0.014f) * 20;

	float targetY = hoverY + wave;

	velocityY = (targetY - y) * 2.2f;

	if (velocityY > 95)

	{

		velocityY = 95;

	}

	if (velocityY < -95)

	{

		velocityY = -95;

	}



	if (target != 0 && target->isActive() && !target->isDead())

	{

		float alignX = abs(target->getCenterX() - getCenterX());

		bool playerBelow = target->getCenterY() > getCenterY();

		if (alignX <= 34 && playerBelow && attackTimer <= 0)

		{

			queuedAttack = true;

			attackTimer = attackCooldown;

		}

	}

}



void FlyingTara::update(float deltaTime)

{

	buildSprites();



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



	updateAI();

	lastVerticalVelocity = velocityY;



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

		float top = 90;

		float bottom = activeLevel->getMainGroundYAt(getCenterX()) - height - 26;

		if (bottom < top + 20)

		{

			bottom = top + 20;

		}

		if (y < top)

		{

			y = top;

		}

		if (y > bottom)

		{

			y = bottom;

		}

	}



	body.setPosition(x, y);



	if (!taraSpritesLoaded)

	{

		body.setFillColor(fallbackColor);

		if (damageFlashTimer > 0)

		{

			body.setFillColor(Color(255, 155, 180));

		}

		return;

	}



	const float vy = lastVerticalVelocity;



	if (abs(vy) >= 40)

	{

		visibleKind = vy > 0 ? FlyingTaraAnimKind::MoveDown : FlyingTaraAnimKind::MoveUp;

	}

	else

	{

		visibleKind = FlyingTaraAnimKind::Idle;

	}



	SpriteAnimation* activeAnim = nullptr;

	switch (visibleKind)

	{

	case FlyingTaraAnimKind::Idle:

		activeAnim = &animIdle;

		break;

	case FlyingTaraAnimKind::MoveUp:

		activeAnim = &animUp;

		break;

	case FlyingTaraAnimKind::MoveDown:

	default:

		activeAnim = &animDown;

		break;

	}



	if (damageFlashTimer > 0)

	{

		const Color flash(255, 165, 195);

		animIdle.setTint(flash);

		animUp.setTint(flash);

		animDown.setTint(flash);

	}

	else

	{

		const Color normal = Color::White;

		animIdle.setTint(normal);

		animUp.setTint(normal);

		animDown.setTint(normal);

	}



	activeAnim->setFacingRight(facingRight);

	activeAnim->update(deltaTime);



	body.setFillColor(Color::Transparent);

}



Projectile* FlyingTara::attack()

{

	float bombX = x + width * 0.62f;

	float bombY = y + height - 10;

	float throwX = velocityX * 0.35f;

	return new FlyingTaraBombProjectile(bombX, bombY, throwX, 130);

}



void FlyingTara::draw(RenderWindow& window)

{

	if (!visible)

	{

		return;

	}



	if (!taraSpritesLoaded)

	{

		Enemy::draw(window);

		return;

	}



	SpriteAnimation* activeAnim = nullptr;

	switch (visibleKind)

	{

	case FlyingTaraAnimKind::Idle:

		activeAnim = &animIdle;

		break;

	case FlyingTaraAnimKind::MoveUp:

		activeAnim = &animUp;

		break;

	case FlyingTaraAnimKind::MoveDown:

	default:

		activeAnim = &animDown;

		break;

	}



	activeAnim->setFacingRight(facingRight);

	activeAnim->drawAtAnchor(window, x + width * 0.5f, y + height);



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



const char* FlyingTara::getType() const

{

	return "FlyingTara";

}



EnemyVehicleType FlyingTara::getVehicleType() const

{

	return ENEMY_VEHICLE_FLYING_TARA;

}



const char* FlyingTara::getEnemyName() const

{

	return "Flying Tara";

}

