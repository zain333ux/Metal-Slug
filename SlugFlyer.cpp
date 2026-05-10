#include "SlugFlyer.h"

#include "Constants.h"
#include "DeveloperMode.h"
#include "EntityManager.h"
#include "Level.h"
#include "VehicleBullet.h"
#include "VehicleSpecialProjectiles.h"
#include "VehicleTextureManager.h"

#include <SFML/Window/Keyboard.hpp>


using namespace std;
using namespace sf;

namespace
{
	bool pushSlugStrip(FrameRect* out, int& count, const int* widths, int n, int sheetH, int refW)
	{
		int xAcc = 0;
		for (int i = 0; i < n; i += 1)
		{
			int ww = widths[i];
			int ox = (refW - ww) / 2;
			out[count] = FrameRect{xAcc, 0, ww, sheetH, ox, 0};
			count += 1;
			xAcc += ww;
		}
		return xAcc > 0;
	}
} // namespace

SlugFlyer::SlugFlyer(float startX, float startY) : Vehicle(startX, startY)
{
	usingSprite = false;
	usingCannonSprite = false;
	width = 130;
	height = 110;
	baseMaxHealth = 140;
	maxHealth = baseMaxHealth;
	health = maxHealth;
	moveSpeed = Constants::PLAYER_MOVE_SPEED * 1.15f;
	verticalSpeed = 290;
	baseFireCooldown = 0.48f;
	weapon.setCooldown(baseFireCooldown);
	missilesRemaining = 4;
	previousMissileKey = false;
	slugVisualUsingShootFrames = false;
	vizPhase = V_PHASE_FLIGHT_IDLE;

	slugSpritesLoaded = buildSlugAnimations();

	worldTopBound = 70;
	worldBottomBound = static_cast<float>(Constants::GROUND_Y) - 70;

	body.setFillColor(Color(70, 120, 155));
	cabin.setFillColor(Color(120, 175, 210));
	turret.setFillColor(Color(62, 92, 120));
	rearWheel.setFillColor(Color(70, 120, 155));
	frontWheel.setFillColor(Color(70, 120, 155));

	updateVisualPosition();
	updateColors();
}

bool SlugFlyer::loadVehicleSprites()
{
	slugSpritesLoaded = buildSlugAnimations();
	return false;
}

bool SlugFlyer::buildSlugAnimations()
{
	const Texture* idleTex = VehicleTextureManager::instance().getTexture(VehicleTextureId::SlugFlyerIdle);
	static FrameRect idleFr[5];
	static bool idleBuilt = false;
	if (!idleBuilt && idleTex != nullptr)
	{
		static const int w[] = {83, 83, 83, 83, 82};
		int c = 0;
		pushSlugStrip(idleFr, c, w, 5, 52, 83);
		idleBuilt = true;
	}

	const Texture* shootTex = VehicleTextureManager::instance().getTexture(VehicleTextureId::SlugFlyerShoot);
	static FrameRect shootFr[5];
	static bool shootBuilt = false;
	if (!shootBuilt && shootTex != nullptr)
	{
		static const int w[] = {81, 80, 80, 80, 80};
		int c = 0;
		pushSlugStrip(shootFr, c, w, 5, 58, 81);
		shootBuilt = true;
	}

	const Texture* landTex = VehicleTextureManager::instance().getTexture(VehicleTextureId::SlugFlyerLand);
	static FrameRect landFr[6];
	static bool landBuilt = false;
	if (!landBuilt && landTex != nullptr)
	{
		static const int w[] = {81, 81, 81, 81, 81, 81};
		int c = 0;
		pushSlugStrip(landFr, c, w, 6, 86, 81);
		landBuilt = true;
	}

	if (idleTex != nullptr && idleBuilt)
	{
		animIdle.setTexture(idleTex);
		animIdle.setFrames(idleFr, 5);
		animIdle.setFrameTime(0.1f);
		animIdle.setLoop(true);
		animIdle.setScale(2.05f, 2.05f);
		animIdle.reset();
	}
	else
	{
		return false;
	}

	if (shootTex != nullptr && shootBuilt)
	{
		animShoot.setTexture(shootTex);
		animShoot.setFrames(shootFr, 5);
		animShoot.setFrameTime(0.08f);
		animShoot.setLoop(true);
		animShoot.setScale(2.05f, 2.05f);
		animShoot.reset();
	}
	else
	{
		return false;
	}

	if (landTex != nullptr && landBuilt)
	{
		animLand.setTexture(landTex);
		animLand.setFrames(landFr, 6);
		animLand.setFrameTime(0.085f);
		animLand.setLoop(false);
		animLand.setPlayBackward(false);
		animLand.setScale(2.05f, 2.05f);
		animLand.reset();
	}
	else
	{
		return false;
	}

	return true;
}

void SlugFlyer::handleMovementInput()
{
	velocityX = 0;

	if (occupied && vizPhase != V_PHASE_LAND_PLAY && vizPhase != V_PHASE_TAKEOFF_PLAY)
	{
		if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
		{
			velocityX = -moveSpeed;
			facingRight = false;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
		{
			velocityX = moveSpeed;
			facingRight = true;
		}
	}

	if (occupied && vizPhase != V_PHASE_LAND_PLAY && vizPhase != V_PHASE_TAKEOFF_PLAY && vizPhase != V_PHASE_GROUND_HOVER)
	{
		velocityY = 0;
		if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))
		{
			velocityY = -verticalSpeed;
		}
		if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S))
		{
			velocityY = verticalSpeed;
		}
	}
	else if (!occupied || vizPhase == V_PHASE_GROUND_HOVER || vizPhase == V_PHASE_LAND_PLAY || vizPhase == V_PHASE_TAKEOFF_PLAY)
	{
		velocityY = 0;
		if (occupied && vizPhase == V_PHASE_GROUND_HOVER &&
			(Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A)))
		{
			velocityX = -moveSpeed;
			facingRight = false;
		}
		if (occupied && vizPhase == V_PHASE_GROUND_HOVER &&
			(Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)))
		{
			velocityX = moveSpeed;
			facingRight = true;
		}
	}
}

void SlugFlyer::syncSlugVisualState(float deltaTime, bool fireHeld, bool landIntent, bool takeoffIntent, bool groundedNearFloor)
{
	switch (vizPhase)
	{
	case V_PHASE_LAND_PLAY:
		animLand.setPlayBackward(false);
		animLand.update(deltaTime);
		if (animLand.isFinished())
		{
			vizPhase = V_PHASE_GROUND_HOVER;
			animIdle.reset();
			animIdle.setPlayBackward(false);
		}
		return;
	case V_PHASE_TAKEOFF_PLAY:
		animLand.setPlayBackward(true);
		animLand.update(deltaTime);
		if (animLand.isFinished())
		{
			vizPhase = V_PHASE_FLIGHT_IDLE;
			animIdle.reset();
			animIdle.setPlayBackward(false);
		}
		return;
	default:
		break;
	}

	bool canAttemptLand =
		(vizPhase == V_PHASE_FLIGHT_IDLE || vizPhase == V_PHASE_SHOOT_LOOP) && landIntent && groundedNearFloor;
	if (canAttemptLand)
	{
		vizPhase = V_PHASE_LAND_PLAY;
		animLand.setPlayBackward(false);
		animLand.reset();
		animLand.update(0);
		return;
	}

	if (vizPhase == V_PHASE_GROUND_HOVER && takeoffIntent)
	{
		vizPhase = V_PHASE_TAKEOFF_PLAY;
		animLand.setPlayBackward(true);
		animLand.reset();
		animLand.update(0);
		return;
	}

	if (vizPhase == V_PHASE_GROUND_HOVER)
	{
		slugVisualUsingShootFrames = fireHeld && animShoot.getFrameCount() > 0;
		if (slugVisualUsingShootFrames)
		{
			animShoot.setFacingRight(facingRight);
			animShoot.update(deltaTime);
		}
		else
		{
			animIdle.setFacingRight(facingRight);
			animIdle.update(deltaTime);
		}
		return;
	}

	if (vizPhase == V_PHASE_FLIGHT_IDLE && fireHeld && animShoot.getFrameCount() > 0)
	{
		vizPhase = V_PHASE_SHOOT_LOOP;
		animShoot.reset();
	}
	else if (vizPhase == V_PHASE_SHOOT_LOOP && !fireHeld)
	{
		vizPhase = V_PHASE_FLIGHT_IDLE;
		animIdle.reset();
	}

	slugVisualUsingShootFrames =
		vizPhase == V_PHASE_SHOOT_LOOP && animShoot.getFrameCount() > 0;
	if (slugVisualUsingShootFrames)
	{
		animShoot.setFacingRight(facingRight);
		animShoot.update(deltaTime);
	}
	else
	{
		animIdle.setFacingRight(facingRight);
		animIdle.update(deltaTime);
	}
}

void SlugFlyer::refreshSlugAnimTint(float deltaTime)
{
	(void)deltaTime;
	const Color flash(255, 130, 130);
	const Color normal = Color::White;

	if (!slugSpritesLoaded)
	{
		return;
	}

	if (damageFlashTimer > 0)
	{
		animIdle.setTint(flash);
		animShoot.setTint(flash);
		animLand.setTint(flash);
	}
	else
	{
		animIdle.setTint(normal);
		animShoot.setTint(normal);
		animLand.setTint(normal);
	}
}

void SlugFlyer::update(float deltaTime)
{
	if (DeveloperMode::isEnabled())
	{
		health = maxHealth;
	}
	if (damageFlashTimer > 0)
	{
		damageFlashTimer -= deltaTime;
	}

	if (occupied)
	{
		handleMovementInput();
	}
	else
	{
		velocityX = 0;
		velocityY = 0;
	}

	if (vizPhase != V_PHASE_LAND_PLAY && vizPhase != V_PHASE_TAKEOFF_PLAY)
	{
		Entity::update(deltaTime);

		float worldTop = worldTopBound;
		float worldBottom = worldBottomBound;
		if (activeLevel != 0)
		{
			worldTop = 40;
			float terrainY = activeLevel->getMainGroundYAt(x + width * 0.5f);
			worldBottom = terrainY - 40;
		}
		worldBottomBound = worldBottom;
		worldTopBound = worldTop;
		if (worldBottom < worldTop + height + 10)
		{
			worldBottom = worldTop + height + 10;
			worldBottomBound = worldBottom;
		}

		if (x < 0)
		{
			x = 0;
		}
		if (x + width > movementMaxX)
		{
			x = movementMaxX - width;
		}
		if (y < worldTop)
		{
			y = worldTop;
		}
		if (y + height > worldBottom)
		{
			y = worldBottom - height;
		}
	}
	else
	{
		worldBottomBound =
			activeLevel != 0 ? activeLevel->getMainGroundYAt(x + width * 0.5f) - 40 : static_cast<float>(Constants::GROUND_Y) - 70;
	}

	bool fireHeld = occupied && !isDead() &&
								 (Keyboard::isKeyPressed(Keyboard::LControl) || Keyboard::isKeyPressed(Keyboard::J));

	bool landIntent =
		occupied && (vizPhase != V_PHASE_LAND_PLAY && vizPhase != V_PHASE_TAKEOFF_PLAY) &&
		(Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S));
	bool takeoffIntent =
		occupied && vizPhase == V_PHASE_GROUND_HOVER &&
		(Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W));
	bool groundedNear = (y + height) >= worldBottomBound - 14;

	syncSlugVisualState(deltaTime, fireHeld, landIntent, takeoffIntent, groundedNear);

	refreshSlugAnimTint(deltaTime);
	updateColors();
	updateVisualPosition();
}

void SlugFlyer::handleWeaponInput(EntityManager& entityManager, float deltaTime)
{
	weapon.update(deltaTime);

	if (!occupied || isDead())
	{
		previousMissileKey = Keyboard::isKeyPressed(Keyboard::H);
		return;
	}

	if (vizPhase == V_PHASE_LAND_PLAY || vizPhase == V_PHASE_TAKEOFF_PLAY)
	{
		previousMissileKey = Keyboard::isKeyPressed(Keyboard::H);
		return;
	}

	bool fireBulletKey = Keyboard::isKeyPressed(Keyboard::LControl) || Keyboard::isKeyPressed(Keyboard::J);
	if (fireBulletKey && weapon.canFire())
	{
		float bulletX = facingRight ? x + width + 8 : x - 32;
		float bulletY = y + height * 0.45f;
		VehicleBullet* bullet = new VehicleBullet(bulletX, bulletY, facingRight);
		float slugFlyerBulletSpeed = Constants::PLAYER_BULLET_SPEED * 0.5f;
		bullet->setVelocity(facingRight ? slugFlyerBulletSpeed : -slugFlyerBulletSpeed, 0);
		entityManager.addEntity(bullet);
		weapon.restartCooldown();
	}

	bool missileKey = Keyboard::isKeyPressed(Keyboard::H);
	if (missileKey && !previousMissileKey && missilesRemaining > 0 && vizPhase != V_PHASE_GROUND_HOVER)
	{
		float rocketX = facingRight ? x + width + 6 : x - 38;
		float rocketY = y + height * 0.4f;
		entityManager.addEntity(new SlugFlyerRocketProjectile(rocketX, rocketY, facingRight, true));
		missilesRemaining -= 1;
	}
	previousMissileKey = missileKey;
}

void SlugFlyer::draw(RenderWindow& window)
{
	if (!visible)
	{
		return;
	}

	if (!slugSpritesLoaded)
	{
		window.draw(body);
		window.draw(cabin);
		window.draw(turret);
		window.draw(rearWheel);
		window.draw(frontWheel);
		drawHealthBar(window);
		return;
	}

	SpriteAnimation* drawAnim = nullptr;
	switch (vizPhase)
	{
	case V_PHASE_SHOOT_LOOP:
	case V_PHASE_GROUND_HOVER:
	case V_PHASE_FLIGHT_IDLE:
		drawAnim = slugVisualUsingShootFrames ? static_cast<SpriteAnimation*>(&animShoot) : static_cast<SpriteAnimation*>(&animIdle);
		break;
	case V_PHASE_LAND_PLAY:
	case V_PHASE_TAKEOFF_PLAY:
		drawAnim = static_cast<SpriteAnimation*>(&animLand);
		break;
	default:
		drawAnim = static_cast<SpriteAnimation*>(&animIdle);
		break;
	}

	drawAnim->setFacingRight(facingRight);
	float ax = x + width * 0.5f;
	float by = y + height;
	drawAnim->drawAtAnchor(window, ax, by);
	drawHealthBar(window);
}