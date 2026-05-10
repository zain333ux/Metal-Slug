#include "BallisticProjectile.h"

#include "AudioManager.h"
#include "Constants.h"


using namespace std;
using namespace sf;

BallisticProjectile::BallisticProjectile(float startX, float startY, float startVelocityX, float startVelocityY,
	int newDamage, float newBlastRadius, float newGravityScale, bool playerOwnedProjectile)
{
	damage = newDamage;
	lifeTime = 3;
	width = 18;
	height = 18;
	explosive = newBlastRadius > 0;
	blastRadius = newBlastRadius;
	gravityScale = newGravityScale;
	exploded = false;
	explosionTimer = 0;
	setPlayerOwned(playerOwnedProjectile);
	setPosition(startX, startY);
	setVelocity(startVelocityX, startVelocityY);

	body.setSize(Vector2f(width, height));
	body.setFillColor(Color(230, 130, 75));
	body.setOutlineColor(Color::Black);
	body.setOutlineThickness(1);
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
	velocityX = 0;
	velocityY = 0;

	float centerX = getCenterX();
	float centerY = getCenterY();
	width = blastRadius * 2;
	height = blastRadius * 2;
	setPosition(centerX - blastRadius, centerY - blastRadius);
	body.setSize(Vector2f(width, height));
	body.setPosition(x, y);
	visible = false;
}

void BallisticProjectile::update(float deltaTime)
{
	if (exploded)
	{
		explosionTimer -= deltaTime;
		if (explosionTimer <= 0)
		{
			deactivate();
		}
		body.setPosition(x, y);
		return;
	}

	lifeTime -= deltaTime;
	if (lifeTime <= 0)
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