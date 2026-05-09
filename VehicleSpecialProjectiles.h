#pragma once

#include "BallisticProjectile.h"
#include "Projectile.h"
#include "SpriteAnimation.h"

/// Bradley arcing shell: forward rocket animation while rising, reversed while falling.
class BradleyArcMissileProjectile : public BallisticProjectile
{
private:
	SpriteAnimation rocketAnim;
	bool lastAscending;

public:
	BradleyArcMissileProjectile(float startX, float startY, float vx, float vy);

	virtual void update(float deltaTime) override;
	virtual void draw(sf::RenderWindow& window) override;

private:
	void syncAnimationDirection();
};

/// Slug Flyer player rocket: launch/flame loop in flight, smoke once on impact.
class SlugFlyerRocketProjectile : public Projectile
{
private:
	enum Phase
	{
		PHASE_FLY,
		PHASE_SMOKE
	};

	Phase phase;
	SpriteAnimation flyAnim;
	SpriteAnimation smokeAnim;
	bool deferUsed;

public:
	SlugFlyerRocketProjectile(float startX, float startY, bool facingRight, bool airborne);

	virtual void update(float deltaTime) override;
	virtual void draw(sf::RenderWindow& window) override;
	virtual bool deferProjectileDeactivateAfterEnemyHit() override;
	virtual bool shouldIgnorePlayerProjectileVsEnemyChecks() const override;

private:
	void beginSmokeAtImpact();
};

/// Flying Tara dropped ordnance (uses FlyingTaraRocket sprite).
class FlyingTaraBombProjectile : public BallisticProjectile
{
private:
	SpriteAnimation bombAnim;
	float visualScale;

public:
	FlyingTaraBombProjectile(float startX, float startY, float throwVelocityX, float throwVelocityY);

	virtual void draw(sf::RenderWindow& window) override;
};

/// Enemy Sub homing torpedo (straight path, sprite from EnemySubmarineBullet).
class EnemySubTorpedoProjectile : public BallisticProjectile
{
private:
	SpriteAnimation bulletAnim;
	float visualScale;

public:
	EnemySubTorpedoProjectile(float startX, float startY, float velocityX, float velocityY);

	virtual void draw(sf::RenderWindow& window) override;
};
