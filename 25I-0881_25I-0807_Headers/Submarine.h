#pragma once

#include "MarinerMissile.h"
#include "SpriteAnimation.h"
#include "Vehicle.h"

/// Player aquatic vehicle (Slug Mariner) — scaled idle sprite only; avoids tank sprite stripping.
class SlugMariner : public AquaticVehicle
{
private:
	static constexpr float MARINER_VISUAL_SCALE_X = 0.65f;
	static constexpr float MARINER_VISUAL_SCALE_Y = 0.65f;

	float verticalSpeed;
	int horizontalAmmo;
	int verticalAmmo;
	int surfaceAmmo;
	bool marinSpritesLoaded;
	SpriteAnimation marinIdle;

	bool buildMarinerSprite();

public:
	SlugMariner(float startX, float startY);

	virtual bool loadVehicleSprites() override;
	virtual void takeDamage(int damage) override;

	virtual void handleMovementInput() override;
	virtual void update(float deltaTime) override;
	virtual void handleWeaponInput(EntityManager& entityManager, float deltaTime) override;

	virtual void draw(sf::RenderWindow& window) override;
};
