#pragma once

#include "SpriteAnimation.h"
#include "Vehicle.h"

/// Player aerial slug: real sprite idle / shoot / land / takeoff; missiles use SpriteAnimation projectile.
class SlugFlyer : public Vehicle
{
private:
	enum SlugFlyerVisualPhase
	{
		V_PHASE_FLIGHT_IDLE,
		V_PHASE_SHOOT_LOOP,
		V_PHASE_LAND_PLAY,
		V_PHASE_GROUND_HOVER,
		V_PHASE_TAKEOFF_PLAY
	};

	float verticalSpeed;
	int missilesRemaining;
	bool previousMissileKey;
	bool slugSpritesLoaded;
	float worldTopBound;
	float worldBottomBound;

	SlugFlyerVisualPhase vizPhase;
	SpriteAnimation animIdle;
	SpriteAnimation animShoot;
	SpriteAnimation animLand;
	bool slugVisualUsingShootFrames;

	bool buildSlugAnimations();

	void refreshSlugAnimTint(float deltaTime);
	void syncSlugVisualState(float deltaTime, bool fireHeld, bool landIntent, bool takeoffIntent, bool groundedBand);

protected:
	virtual bool loadVehicleSprites() override;

public:
	SlugFlyer(float startX, float startY);

	virtual void handleMovementInput() override;
	virtual void update(float deltaTime) override;
	virtual void handleWeaponInput(EntityManager& entityManager, float deltaTime) override;
	virtual void draw(sf::RenderWindow& window) override;
};
