#include "FlyingTara.h"



#include "Level.h"

#include "PlayerSoldier.h"

#include "VehicleSpecialProjectiles.h"

#include "VehicleTextureManager.h"



#include <cmath>



FlyingTara::FlyingTara()

{

	maxHealth = 8;

	health = maxHealth;

	scoreValue = 220;

	contactDamage = 12;

	width = 82.0f;

	height = 58.0f;

	moveSpeed = 105.0f;

	canMoveInAir = true;

	attackCooldown = 1.9f;

	hoverY = y;

	fallbackColor = sf::Color(195, 105, 230);

	body.setSize(sf::Vector2f(width, height));

	usingSprite = false;



	visibleKind = FlyingTaraAnimKind::Idle;

	taraSpritesLoaded = false;

	damageFlashTimer = 0.0f;

	lastVerticalVelocity = 0.0f;



	buildSprites();

}



void FlyingTara::buildSprites()

{

	const sf::Texture* idleTex = VehicleTextureManager::instance().getTexture(VehicleTextureId::TaraIdle);

	const sf::Texture* mdTex = VehicleTextureManager::instance().getTexture(VehicleTextureId::TaraMoveDown);

	const sf::Texture* muTex = VehicleTextureManager::instance().getTexture(VehicleTextureId::TaraMoveUp);



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

	velocityY = 0.0f;



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



	float wave = std::sin(x * 0.014f) * 20.0f;

	float targetY = hoverY + wave;

	velocityY = (targetY - y) * 2.2f;

	if (velocityY > 95.0f)

	{

		velocityY = 95.0f;

	}

	if (velocityY < -95.0f)

	{

		velocityY = -95.0f;

	}



	if (target != 0 && target->isActive() && !target->isDead())

	{

		float alignX = std::abs(target->getCenterX() - getCenterX());

		bool playerBelow = target->getCenterY() > getCenterY();

		if (alignX <= 34.0f && playerBelow && attackTimer <= 0.0f)

		{

			queuedAttack = true;

			attackTimer = attackCooldown;

		}

	}

}



void FlyingTara::update(float deltaTime)

{

	buildSprites();



	if (damageFlashTimer > 0.0f)

	{

		damageFlashTimer -= deltaTime;

	}

	if (contactDamageTimer > 0.0f)

	{

		contactDamageTimer -= deltaTime;

	}

	if (attackTimer > 0.0f)

	{

		attackTimer -= deltaTime;

	}



	updateAI();

	lastVerticalVelocity = velocityY;



	Entity::update(deltaTime);



	if (x < 0.0f)

	{

		x = 0.0f;

		patrolDirection = 1;

	}



	if (x + width > movementMaxX)

	{

		x = movementMaxX - width;

		patrolDirection = -1;

	}



	if (activeLevel != 0)

	{

		float top = 90.0f;

		float bottom = activeLevel->getMainGroundYAt(getCenterX()) - height - 26.0f;

		if (bottom < top + 20.0f)

		{

			bottom = top + 20.0f;

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

		if (damageFlashTimer > 0.0f)

		{

			body.setFillColor(sf::Color(255, 155, 180));

		}

		return;

	}



	const float vy = lastVerticalVelocity;



	if (std::abs(vy) >= 40.0f)

	{

		visibleKind = vy > 0.0f ? FlyingTaraAnimKind::MoveDown : FlyingTaraAnimKind::MoveUp;

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



	if (damageFlashTimer > 0.0f)

	{

		const sf::Color flash(255, 165, 195);

		animIdle.setTint(flash);

		animUp.setTint(flash);

		animDown.setTint(flash);

	}

	else

	{

		const sf::Color normal = sf::Color::White;

		animIdle.setTint(normal);

		animUp.setTint(normal);

		animDown.setTint(normal);

	}



	activeAnim->setFacingRight(facingRight);

	activeAnim->update(deltaTime);



	body.setFillColor(sf::Color::Transparent);

}



Projectile* FlyingTara::attack()

{

	float bombX = x + width * 0.62f;

	float bombY = y + height - 10.0f;

	float throwX = velocityX * 0.35f;

	return new FlyingTaraBombProjectile(bombX, bombY, throwX, 130.0f);

}



void FlyingTara::draw(sf::RenderWindow& window)

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


