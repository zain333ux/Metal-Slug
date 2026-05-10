#include "Submarine.h"

#include "Constants.h"
#include "DeveloperMode.h"
#include "EntityManager.h"
#include "Level.h"
#include "MarinerMissile.h"
#include "VehicleTextureManager.h"

#include <SFML/Window/Keyboard.hpp>
<<<<<<< HEAD
#include <cmath>


using namespace std;
using namespace sf;

// --- MarinerMissile ---
MarinerMissile::MarinerMissile(float startX, float startY, bool facingRight, MarinerMissileType type)
{
	missileType = type;
	damage = 30;
	lifeTime = 3;
	width = 32;
	height = 16;
	explosive = true;
	blastRadius = 120;
	setPosition(startX, startY);

	float speed = 500;

	switch (missileType)
	{
	case MARINER_MISSILE_HORIZONTAL:
		if (facingRight)
			setVelocity(speed, 0);
		else
			setVelocity(-speed, 0);
		break;
	case MARINER_MISSILE_VERTICAL:
		setVelocity(0, -speed);
		width = 16;
		height = 32;
		break;
	case MARINER_MISSILE_SURFACE:
		if (facingRight)
			setVelocity(200, 100);
		else
			setVelocity(-200, 100);
		break;
	}

	body.setSize(Vector2f(width, height));
	body.setFillColor(Color(255, 150, 50));
	body.setOutlineColor(Color::Black);
	body.setOutlineThickness(1);
}

void MarinerMissile::update(float deltaTime)
{
	if (missileType == MARINER_MISSILE_SURFACE)
	{
		velocityY -= Constants::GRAVITY * 1.2f * deltaTime;
		if (velocityY < -600)
			velocityY = -600;
	}
	Projectile::update(deltaTime);
}
=======
>>>>>>> 6d77c54b1816a0bae93ea8c1ad344f5e135692cd

// --- SlugMariner ---

SlugMariner::SlugMariner(float startX, float startY) : Vehicle(startX, startY)
{
	usingSprite = false;
	usingCannonSprite = false;

	width = 120;
	height = 70;
	baseMaxHealth = 200;
	maxHealth = baseMaxHealth;
	health = maxHealth;
	moveSpeed = Constants::PLAYER_MOVE_SPEED * 1.2f;
	verticalSpeed = 250;
	baseFireCooldown = 0.5f;
	weapon.setCooldown(baseFireCooldown);

	horizontalAmmo = 3;
	verticalAmmo = 3;
	surfaceAmmo = 3;

	marinSpritesLoaded = buildMarinerSprite();

	body.setFillColor(Color::Transparent);
	body.setOutlineColor(Color::Transparent);
	cabin.setFillColor(Color::Transparent);
	cabin.setOutlineColor(Color::Transparent);
	turret.setFillColor(Color::Transparent);
	turret.setOutlineColor(Color::Transparent);
	rearWheel.setFillColor(Color::Transparent);
	rearWheel.setOutlineColor(Color::Transparent);
	frontWheel.setFillColor(Color::Transparent);
	frontWheel.setOutlineColor(Color::Transparent);
}

bool SlugMariner::loadVehicleSprites()
{
	marinSpritesLoaded = buildMarinerSprite();
	return false;
}

bool SlugMariner::buildMarinerSprite()
{
	const Texture* tex = VehicleTextureManager::instance().getTexture(VehicleTextureId::SlugMarinerIdle);
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
	velocityX = 0;
	velocityY = 0;

	if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
	{
		velocityX = -moveSpeed;
		facingRight = false;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
	{
		velocityX = moveSpeed;
		facingRight = true;
	}

	if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))
	{
		velocityY = -verticalSpeed;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S))
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
		velocityX = 0;
		velocityY = 30;
	}

	if (damageFlashTimer > 0)
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
			velocityY = 0;
		}

		float waterSurfaceY = activeLevel->getWaterSurfaceYAt(x + width * 0.5f);
		if (y < waterSurfaceY - 20)
		{
			y = waterSurfaceY - 20;
			if (velocityY < 0)
				velocityY = 0;
		}

		if (x < 0)
			x = 0;
		if (x + width > movementMaxX)
			x = movementMaxX - width;
	}

	if (marinSpritesLoaded)
	{
		const Color flash(255, 130, 130);
		marinIdle.setTint(damageFlashTimer > 0 ? flash : Color::White);
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

		if (Keyboard::isKeyPressed(Keyboard::J) || Keyboard::isKeyPressed(Keyboard::LControl))
		{
			if (horizontalAmmo > 0)
			{
				float bulletX = facingRight ? x + width : x - 32;
				float bulletY = y + height * 0.5f;
				entityManager.addEntity(new MarinerMissile(bulletX, bulletY, facingRight, MARINER_MISSILE_HORIZONTAL));
				horizontalAmmo -= 1;
				fired = true;
			}
		}
		else if (Keyboard::isKeyPressed(Keyboard::K) || Keyboard::isKeyPressed(Keyboard::LShift))
		{
			if (verticalAmmo > 0)
			{
				float bulletX = x + width * 0.5f - 8;
				float bulletY = y - 32;
				entityManager.addEntity(new MarinerMissile(bulletX, bulletY, facingRight, MARINER_MISSILE_VERTICAL));
				verticalAmmo -= 1;
				fired = true;
			}
		}
		else if (Keyboard::isKeyPressed(Keyboard::L) || Keyboard::isKeyPressed(Keyboard::Space))
		{
			if (surfaceAmmo > 0)
			{
				float bulletX = facingRight ? x + width : x - 32;
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

void SlugMariner::draw(RenderWindow& window)
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