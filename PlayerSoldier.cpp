#include "PlayerSoldier.h"

#include "Constants.h"
#include "DeveloperMode.h"
#include "EntityManager.h"
#include "GrenadeProjectile.h"
#include "MeleeHitbox.h"
#include "RocketProjectile.h"

#include <SFML/Window/Keyboard.hpp>

PlayerSoldier::PlayerSoldier()
{
	firing = false;
	fireAnimationTimer = 0.0f;
	currentCharacter = 0;
	for (int i = 0; i < 4; i += 1)
	{
		lives[i] = 2;
		grenades[i] = 10;
		rockets[i] = 4;
	}
	meleeTimer = 0.0f;
	grenadeTimer = 0.0f;
	rocketTimer = 0.0f;
	damageTimer = 0.0f;
	aimingUp = false;
	previousSwitchKey = false;
	previousMeleeKey = false;
	previousGrenadeKey = false;
	previousRocketKey = false;
	ridingVehicle = false;
	pilotHiddenWhileInsideVehicle = false;
	setPosition(120.0f, 500.0f);
	setSpriteScale(2.2f);
	if (loadSpriteSheet("Sprites/Clean/player_marco_sheet.png"))
	{
		setSpriteFrame(0, 0, 96, 96);
		playAnimation(Constants::PLAYER_ANIM_IDLE, 4, 0.18f);
	}
	applyCharacterStats();
}

void PlayerSoldier::takeDamage(int damage)
{
	if (DeveloperMode::isEnabled())
	{
		return;
	}

	if (damage <= 0 || isDead() || ridingVehicle || damageTimer > 0.0f)
	{
		return;
	}

	health -= damage;
	damageTimer = 0.8f;
	if (health <= 0)
	{
		health = 0;
		if (lives[currentCharacter] > 0)
		{
			lives[currentCharacter] -= 1;
		}
	}
}

void PlayerSoldier::respawn()
{
	health = maxHealth;
	setPosition(120.0f, 500.0f);
	setVelocity(0.0f, 0.0f);
	facingRight = true;
	firing = false;
	fireAnimationTimer = 0.0f;
	aimingUp = false;
	damageTimer = 0.0f;
	setRidingVehicle(false);
	setPilotHiddenForVehicle(false);
	playAnimation(Constants::PLAYER_ANIM_IDLE, 4, 0.18f);
}

void PlayerSoldier::setPilotHiddenForVehicle(bool hide)
{
	pilotHiddenWhileInsideVehicle = hide;
	if (hide && ridingVehicle)
	{
		visible = false;
	}
}

void PlayerSoldier::draw(sf::RenderWindow& window)
{
	if (!visible || (pilotHiddenWhileInsideVehicle && ridingVehicle))
	{
		return;
	}

	Soldier::draw(window);
}

void PlayerSoldier::applyCharacterStats()
{
	moveSpeed = Constants::PLAYER_MOVE_SPEED;
	maxHealth = 100;

	if (currentCharacter == 0)
	{
		weapon.setCooldown(Constants::PLAYER_FIRE_COOLDOWN * 0.75f);
	}
	else if (currentCharacter == 1)
	{
		moveSpeed = Constants::PLAYER_MOVE_SPEED * 0.8f;
		maxHealth = 80;
		weapon.setCooldown(Constants::PLAYER_FIRE_COOLDOWN);
	}
	else if (currentCharacter == 2)
	{
		weapon.setCooldown(Constants::PLAYER_FIRE_COOLDOWN * 1.2f);
	}
	else
	{
		weapon.setCooldown(Constants::PLAYER_FIRE_COOLDOWN * 0.9f);
	}

	if (health > maxHealth)
	{
		health = maxHealth;
	}
}

void PlayerSoldier::switchCharacter()
{
	int startCharacter = currentCharacter;
	for (int i = 0; i < 4; i += 1)
	{
		currentCharacter = (currentCharacter + 1) % 4;
		if (lives[currentCharacter] > 0)
		{
			applyCharacterStats();
			if (isDead())
			{
				respawn();
			}
			return;
		}
	}

	currentCharacter = startCharacter;
	applyCharacterStats();
}

void PlayerSoldier::handleInput()
{
	if (isDead())
	{
		stopMoving();
		return;
	}

	if (ridingVehicle)
	{
		stopMoving();
		return;
	}

	stopMoving();
	aimingUp = sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up);

	bool switchKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
	if (switchKey && !previousSwitchKey)
	{
		switchCharacter();
	}
	previousSwitchKey = switchKey;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		moveLeft();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		moveRight();
	}

	if (inWater)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			velocityY = -jumpSpeed * 0.45f;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			velocityY += jumpSpeed * 0.015f;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		jump();
	}
}

void PlayerSoldier::update(float deltaTime)
{
	if (meleeTimer > 0.0f)
	{
		meleeTimer -= deltaTime;
	}
	if (grenadeTimer > 0.0f)
	{
		grenadeTimer -= deltaTime;
	}
	if (rocketTimer > 0.0f)
	{
		rocketTimer -= deltaTime;
	}
	if (damageTimer > 0.0f)
	{
		damageTimer -= deltaTime;
	}

	if (DeveloperMode::isEnabled())
	{
		health = maxHealth;
		refillDemoInventory();
	}

	if (ridingVehicle)
	{
		stopMoving();
		setVelocity(0.0f, 0.0f);
		return;
	}

	handleInput();
	Soldier::update(deltaTime);
	updatePlayerAnimation(deltaTime);

	if (isDead())
	{
		body.setFillColor(sf::Color(90, 90, 90));
	}
}

void PlayerSoldier::updatePlayerAnimation(float deltaTime)
{
	if (aimingUp && currentState != Constants::SOLDIER_STATE_JUMPING && currentState != Constants::SOLDIER_STATE_FALLING)
	{
		if (fireAnimationTimer > 0.0f)
		{
			fireAnimationTimer -= deltaTime;
		}
		firing = false;
		playAnimation(Constants::PLAYER_ANIM_IDLE, 4, 0.18f);
		return;
	}

	if (fireAnimationTimer > 0.0f)
	{
		fireAnimationTimer -= deltaTime;
		playAnimation(Constants::PLAYER_ANIM_FIRE, 3, 0.08f);
		return;
	}

	firing = false;

	if (currentState == Constants::SOLDIER_STATE_JUMPING || currentState == Constants::SOLDIER_STATE_FALLING)
	{
		playAnimation(Constants::PLAYER_ANIM_JUMP, 6, 0.12f);
	}
	else if (currentState == Constants::SOLDIER_STATE_RUNNING)
	{
		playAnimation(Constants::PLAYER_ANIM_RUN, 8, 0.09f);
	}
	else
	{
		playAnimation(Constants::PLAYER_ANIM_IDLE, 4, 0.18f);
	}
}

void PlayerSoldier::handleWeaponInput(EntityManager& entityManager, float deltaTime)
{
	weapon.update(deltaTime);

	if (isDead() || ridingVehicle)
	{
		return;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::J))
	{
		if (weapon.canFire())
		{
			firing = true;
			if (!aimingUp)
			{
				fireAnimationTimer = 0.24f;
			}
		}

		if (!aimingUp && (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)))
		{
			facingRight = false;
		}
		else if (!aimingUp && (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)))
		{
			facingRight = true;
		}

		float bulletX = x;
		if (aimingUp)
		{
			bulletX = x + width / 2.0f - 8.0f;
		}
		else if (facingRight)
		{
			bulletX = x + width;
		}

		float bulletY = aimingUp ? y + 6.0f : y + 38.0f;
		weapon.fire(entityManager, bulletX, bulletY, facingRight, aimingUp);
	}

	bool meleeKey = sf::Keyboard::isKeyPressed(sf::Keyboard::K);
	if (meleeKey && !previousMeleeKey && meleeTimer <= 0.0f)
	{
		float hitX = facingRight ? x + width : x;
		MeleeHitbox* hitbox = new MeleeHitbox(hitX, y + 18.0f, facingRight, !grounded);
		entityManager.addEntity(hitbox);
		meleeTimer = Constants::MELEE_COOLDOWN;
		fireAnimationTimer = 0.18f;
	}
	previousMeleeKey = meleeKey;

	bool grenadeKey = sf::Keyboard::isKeyPressed(sf::Keyboard::G);
	if (grenadeKey && !previousGrenadeKey && grenadeTimer <= 0.0f)
	{
		if (grenades[currentCharacter] > 0 || DeveloperMode::isEnabled())
		{
			float grenadeX = facingRight ? x + width : x;
			float grenadeY = y + 30.0f;
			GrenadeProjectile* grenade = new GrenadeProjectile(grenadeX, grenadeY, facingRight, !grounded);
			entityManager.addEntity(grenade);
			if (!DeveloperMode::isEnabled())
			{
				grenades[currentCharacter] -= 1;
			}
			grenadeTimer = Constants::GRENADE_COOLDOWN;
			fireAnimationTimer = 0.24f;
		}
	}
	previousGrenadeKey = grenadeKey;

	bool rocketKey = sf::Keyboard::isKeyPressed(sf::Keyboard::H);
	if (rocketKey && !previousRocketKey && rocketTimer <= 0.0f)
	{
		if (rockets[currentCharacter] > 0 || DeveloperMode::isEnabled())
		{
			float rocketX = facingRight ? x + width : x;
			float rocketY = y + 42.0f;
			RocketProjectile* rocket = new RocketProjectile(rocketX, rocketY, facingRight, !grounded);
			entityManager.addEntity(rocket);
			if (!DeveloperMode::isEnabled())
			{
				rockets[currentCharacter] -= 1;
			}
			rocketTimer = Constants::ROCKET_COOLDOWN;
			fireAnimationTimer = 0.24f;
		}
	}
	previousRocketKey = rocketKey;
}

void PlayerSoldier::refillDemoInventory()
{
	for (int i = 0; i < 4; i += 1)
	{
		grenades[i] = 99;
		rockets[i] = 99;
	}
}

void PlayerSoldier::setRidingVehicle(bool riding)
{
	ridingVehicle = riding;
	visible = !riding;

	if (ridingVehicle)
	{
		stopMoving();
		aimingUp = false;
		firing = false;
		fireAnimationTimer = 0.0f;
	}
}

void PlayerSoldier::handleVehicleDestruction()
{
	if (DeveloperMode::isEnabled() || survivesVehicleDestruction())
	{
		return;
	}

	health = 0;
	if (lives[currentCharacter] > 0)
	{
		lives[currentCharacter] -= 1;
	}
	damageTimer = 0.8f;
}

bool PlayerSoldier::isRidingVehicle() const
{
	return ridingVehicle;
}

float PlayerSoldier::getVehicleFireCooldownMultiplier() const
{
	if (currentCharacter == 1)
	{
		return 0.75f;
	}

	return 1.0f;
}

float PlayerSoldier::getVehicleDurabilityMultiplier() const
{
	if (currentCharacter == 1)
	{
		return 1.2f;
	}

	return 1.0f;
}

bool PlayerSoldier::survivesVehicleDestruction() const
{
	return currentCharacter == 1;
}

const char* PlayerSoldier::getCharacterName() const
{
	if (currentCharacter == 0)
	{
		return "Marco";
	}
	if (currentCharacter == 1)
	{
		return "Tarma";
	}
	if (currentCharacter == 2)
	{
		return "Eri";
	}
	return "Fio";
}

int PlayerSoldier::getLives() const
{
	return lives[currentCharacter];
}

int PlayerSoldier::getGrenades() const
{
	return grenades[currentCharacter];
}

int PlayerSoldier::getRockets() const
{
	return rockets[currentCharacter];
}
