#include "BallisticProjectile.h"

#include "AudioManager.h"
#include "Constants.h"

BallisticProjectile::BallisticProjectile(float startX, float startY, float startVelocityX, float startVelocityY,
	int newDamage, float newBlastRadius, float newGravityScale, bool playerOwnedProjectile)
{
	damage = newDamage;
	lifeTime = 3.0f;
	width = 18.0f;
	height = 18.0f;
	explosive = newBlastRadius > 0.0f;
	blastRadius = newBlastRadius;
	gravityScale = newGravityScale;
	exploded = false;
	explosionTimer = 0.0f;
	setPlayerOwned(playerOwnedProjectile);
	setPosition(startX, startY);
	setVelocity(startVelocityX, startVelocityY);

	body.setSize(sf::Vector2f(width, height));
	body.setFillColor(sf::Color(230, 130, 75));
	body.setOutlineColor(sf::Color::Black);
	body.setOutlineThickness(1.0f);
}

void BallisticProjectile::explode()
{
	if (exploded)
	{
		return;
	}

	if (!explosive)
	{
		deactivate();
		return;
	}

	exploded = true;
	AudioManager::playGlobalSound(SFX_EXPLOSION);
	explosionTimer = 0.12f;
	velocityX = 0.0f;
	velocityY = 0.0f;

	float centerX = getCenterX();
	float centerY = getCenterY();
	width = blastRadius * 2.0f;
	height = blastRadius * 2.0f;
	setPosition(centerX - blastRadius, centerY - blastRadius);
	body.setSize(sf::Vector2f(width, height));
	body.setPosition(x, y);
	visible = false;
}

void BallisticProjectile::update(float deltaTime)
{
	if (exploded)
	{
		explosionTimer -= deltaTime;
		if (explosionTimer <= 0.0f)
		{
			deactivate();
		}
		body.setPosition(x, y);
		return;
	}

	lifeTime -= deltaTime;
	if (lifeTime <= 0.0f)
	{
		explode();
		return;
	}

	velocityY += Constants::GRAVITY * gravityScale * deltaTime;
	Entity::update(deltaTime);
	body.setPosition(x, y);

	if (y + height >= Constants::GROUND_Y)
	{
		y = Constants::GROUND_Y - height;
		explode();
	}
}
