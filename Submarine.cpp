#include "Submarine.h"

#include "Constants.h"
#include "DeveloperMode.h"
#include "EntityManager.h"
#include "Level.h"
#include "VehicleTextureManager.h"

#include <SFML/Window/Keyboard.hpp>
#include <cmath>

// --- MarinerMissile ---
MarinerMissile::MarinerMissile(float startX, float startY, bool facingRight, MarinerMissileType type)
{
	missileType = type;
	damage = 30;
	lifeTime = 3.0f;
	width = 32.0f;
	height = 16.0f;
	explosive = true;
	blastRadius = 120.0f;
	setPosition(startX, startY);

	float speed = 500.0f;

	switch (missileType)
	{
	case MARINER_MISSILE_HORIZONTAL:
		if (facingRight)
			setVelocity(speed, 0.0f);
		else
			setVelocity(-speed, 0.0f);
		break;
	case MARINER_MISSILE_VERTICAL:
		setVelocity(0.0f, -speed);
		width = 16.0f;
		height = 32.0f;
		break;
	case MARINER_MISSILE_SURFACE:
		if (facingRight)
			setVelocity(200.0f, 100.0f);
		else
			setVelocity(-200.0f, 100.0f);
		break;
	}

	body.setSize(sf::Vector2f(width, height));
	body.setFillColor(sf::Color(255, 150, 50));
	body.setOutlineColor(sf::Color::Black);
	body.setOutlineThickness(1.0f);
}

void MarinerMissile::update(float deltaTime)
{
	if (missileType == MARINER_MISSILE_SURFACE)
	{
		velocityY -= Constants::GRAVITY * 1.2f * deltaTime;
		if (velocityY < -600.0f)
			velocityY = -600.0f;
	}
	Projectile::update(deltaTime);
}

// --- SlugMariner ---

SlugMariner::SlugMariner(float startX, float startY) : Vehicle(startX, startY)
{
	usingSprite = false;
	usingCannonSprite = false;

	width = 120.0f;
	height = 70.0f;
	baseMaxHealth = 200;
	maxHealth = baseMaxHealth;
	health = maxHealth;
	moveSpeed = Constants::PLAYER_MOVE_SPEED * 1.2f;
	verticalSpeed = 250.0f;
	baseFireCooldown = 0.5f;
	weapon.setCooldown(baseFireCooldown);

	horizontalAmmo = 3;
	verticalAmmo = 3;
	surfaceAmmo = 3;

	marinSpritesLoaded = buildMarinerSprite();

	body.setFillColor(sf::Color::Transparent);
	body.setOutlineColor(sf::Color::Transparent);
	cabin.setFillColor(sf::Color::Transparent);
	cabin.setOutlineColor(sf::Color::Transparent);
	turret.setFillColor(sf::Color::Transparent);
	turret.setOutlineColor(sf::Color::Transparent);
	rearWheel.setFillColor(sf::Color::Transparent);
	rearWheel.setOutlineColor(sf::Color::Transparent);
	frontWheel.setFillColor(sf::Color::Transparent);
	frontWheel.setOutlineColor(sf::Color::Transparent);
}

bool SlugMariner::loadVehicleSprites()
{
	marinSpritesLoaded = buildMarinerSprite();
	return false;
}

bool SlugMariner::buildMarinerSprite()
{
	const sf::Texture* tex = VehicleTextureManager::instance().getTexture(VehicleTextureId::SlugMarinerIdle);
	if (tex == nullptr)
	{
		return false;
	}

	static FrameRect fr = FrameRect{0, 0, 137, 96, 0, 0};
	marinIdle.setTexture(tex);
	marinIdle.setFrames(&fr, 1);
	marinIdle.setFrameTime(0.3f);
	marinIdle.setLoop(true);
	marinIdle.setScale(MARINER_VISUAL_SCALE_X, MARINER_VISUAL_SCALE_Y);
	marinIdle.reset();
	return true;
}

void SlugMariner::takeDamage(int damage)
{
	if (damage <= 0 || isDead() || DeveloperMode::isEnabled())
	{
		return;
	}

	health -= damage;
	damageFlashTimer = 0.18f;

	if (health <= 0)
	{
		health = 0;
		deactivate();
	}
}

void SlugMariner::handleMovementInput()
{
	velocityX = 0.0f;
	velocityY = 0.0f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		velocityX = -moveSpeed;
		facingRight = false;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		velocityX = moveSpeed;
		facingRight = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		velocityY = -verticalSpeed;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		velocityY = verticalSpeed;
	}
}

void SlugMariner::update(float deltaTime)
{
	if (occupied)
	{
		handleMovementInput();
	}
	else
	{
		velocityX = 0.0f;
		velocityY = 30.0f;
	}

	if (damageFlashTimer > 0.0f)
	{
		damageFlashTimer -= deltaTime;
	}

	Entity::update(deltaTime);

	if (activeLevel != 0)
	{
		float groundY = activeLevel->getLandingY(x, x + width, y + height, y + height);
		if (y + height > groundY)
		{
			y = groundY - height;
			velocityY = 0.0f;
		}

		float waterSurfaceY = activeLevel->getWaterSurfaceYAt(x + width * 0.5f);
		if (y < waterSurfaceY - 20.0f)
		{
			y = waterSurfaceY - 20.0f;
			if (velocityY < 0.0f)
				velocityY = 0.0f;
		}

		if (x < 0.0f)
			x = 0.0f;
		if (x + width > movementMaxX)
			x = movementMaxX - width;
	}

	if (marinSpritesLoaded)
	{
		const sf::Color flash(255, 130, 130);
		marinIdle.setTint(damageFlashTimer > 0.0f ? flash : sf::Color::White);
		marinIdle.setFacingRight(facingRight);
		marinIdle.update(deltaTime);
	}
}

void SlugMariner::handleWeaponInput(EntityManager& entityManager, float deltaTime)
{
	weapon.update(deltaTime);

	if (!occupied || isDead())
	{
		return;
	}

	if (weapon.canFire())
	{
		bool fired = false;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::J) || sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
		{
			if (horizontalAmmo > 0)
			{
				float bulletX = facingRight ? x + width : x - 32.0f;
				float bulletY = y + height * 0.5f;
				entityManager.addEntity(new MarinerMissile(bulletX, bulletY, facingRight, MARINER_MISSILE_HORIZONTAL));
				horizontalAmmo -= 1;
				fired = true;
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::K) || sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		{
			if (verticalAmmo > 0)
			{
				float bulletX = x + width * 0.5f - 8.0f;
				float bulletY = y - 32.0f;
				entityManager.addEntity(new MarinerMissile(bulletX, bulletY, facingRight, MARINER_MISSILE_VERTICAL));
				verticalAmmo -= 1;
				fired = true;
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			if (surfaceAmmo > 0)
			{
				float bulletX = facingRight ? x + width : x - 32.0f;
				float bulletY = y + height * 0.5f;
				entityManager.addEntity(new MarinerMissile(bulletX, bulletY, facingRight, MARINER_MISSILE_SURFACE));
				surfaceAmmo -= 1;
				fired = true;
			}
		}

		if (fired)
		{
			weapon.restartCooldown();
		}
	}
}

void SlugMariner::draw(sf::RenderWindow& window)
{
	if (!visible)
	{
		return;
	}

	if (!marinSpritesLoaded)
	{
		Vehicle::draw(window);
		return;
	}

	marinIdle.drawAtAnchor(window, x + width * 0.5f, y + height);
	drawHealthBar(window);
}
