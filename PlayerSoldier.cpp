#include "PlayerSoldier.h"

#include "Constants.h"
#include "EntityManager.h"

#include <SFML/Window/Keyboard.hpp>

PlayerSoldier::PlayerSoldier()
{
	firing = false;
	fireAnimationTimer = 0.0f;
	setPosition(120.0f, 500.0f);
	setSpriteScale(2.2f);
	if (loadSpriteSheet("Sprites/Clean/player_marco_sheet.png"))
	{
		setSpriteFrame(0, 0, 96, 96);
		playAnimation(Constants::PLAYER_ANIM_IDLE, 4, 0.18f);
	}
}

void PlayerSoldier::takeDamage(int damage)
{
	if (damage <= 0 || isDead())
	{
		return;
	}

	health -= damage;
	if (health < 0)
	{
		health = 0;
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
	playAnimation(Constants::PLAYER_ANIM_IDLE, 4, 0.18f);
}

void PlayerSoldier::handleInput()
{
	if (isDead())
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		{
			respawn();
		}

		stopMoving();
		return;
	}

	stopMoving();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		moveLeft();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		moveRight();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		jump();
	}
}

void PlayerSoldier::update(float deltaTime)
{
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

	if (isDead())
	{
		return;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::J))
	{
		if (weapon.canFire())
		{
			firing = true;
			fireAnimationTimer = 0.24f;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			facingRight = false;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			facingRight = true;
		}

		float bulletX = x;
		if (facingRight)
		{
			bulletX = x + width;
		}

		float bulletY = y + 38.0f;
		weapon.fire(entityManager, bulletX, bulletY, facingRight);
	}
}
