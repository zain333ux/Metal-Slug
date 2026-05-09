#include "PlayerSoldier.h"

#include "Constants.h"
#include "DeveloperMode.h"
#include "EntityManager.h"
#include "GrenadeProjectile.h"
#include "MeleeHitbox.h"
#include "RocketProjectile.h"

#include <SFML/Window/Keyboard.hpp>

static const sf::IntRect MARCO_IDLE_TORSO_FRAMES[] =
{
	sf::IntRect(5, 4, 29, 29), sf::IntRect(39, 4, 29, 29), sf::IntRect(73, 4, 30, 28), sf::IntRect(108, 4, 29, 27)
};

static const sf::IntRect MARCO_IDLE_LEG_FRAMES[] =
{
	sf::IntRect(3, 7, 21, 16)
};

static const sf::IntRect MARCO_RUN_TORSO_FRAMES[] =
{
	sf::IntRect(7, 3, 32, 28), sf::IntRect(44, 3, 30, 29), sf::IntRect(79, 3, 28, 28), sf::IntRect(112, 3, 27, 27),
	sf::IntRect(144, 3, 29, 28), sf::IntRect(178, 3, 30, 28), sf::IntRect(213, 3, 32, 29), sf::IntRect(251, 3, 32, 29),
	sf::IntRect(288, 3, 32, 28), sf::IntRect(325, 3, 31, 27), sf::IntRect(361, 3, 31, 28), sf::IntRect(397, 3, 31, 28)
};

static const sf::IntRect MARCO_RUN_LEG_FRAMES[] =
{
	sf::IntRect(3, 5, 21, 20), sf::IntRect(29, 5, 28, 20), sf::IntRect(62, 5, 31, 16), sf::IntRect(98, 5, 19, 20),
	sf::IntRect(122, 5, 15, 20), sf::IntRect(142, 5, 16, 20), sf::IntRect(163, 5, 21, 20), sf::IntRect(189, 5, 26, 20),
	sf::IntRect(220, 5, 31, 16), sf::IntRect(256, 5, 20, 20), sf::IntRect(281, 5, 15, 20), sf::IntRect(301, 5, 18, 20)
};

static const sf::IntRect MARCO_FIRE_FRAMES[] =
{
	sf::IntRect(3, 3, 50, 23), sf::IntRect(58, 3, 51, 23), sf::IntRect(114, 3, 52, 23)
};

static const sf::IntRect MARCO_FACE_UP_FRAMES[] =
{
	sf::IntRect(0, 0, 34, 32), sf::IntRect(34, 0, 34, 32), sf::IntRect(68, 0, 34, 32), sf::IntRect(102, 0, 34, 32)
};

static const sf::IntRect MARCO_SHOOT_UP_FRAMES[] =
{
	sf::IntRect(0, 0, 27, 72), sf::IntRect(27, 0, 27, 72), sf::IntRect(54, 0, 27, 72), sf::IntRect(81, 0, 27, 72)
};

static const float MARCO_IDLE_TORSO_ORIGIN_X[] = { 10.0f, 10.0f, 10.0f, 10.0f };
static const float MARCO_IDLE_TORSO_ORIGIN_Y[] = { 25.0f, 25.0f, 25.0f, 25.0f };
static const float MARCO_RUN_TORSO_ORIGIN_X[] = { 12.0f, 11.0f, 10.0f, 9.5f, 10.5f, 11.0f, 12.0f, 12.0f, 12.0f, 11.5f, 11.5f, 11.5f };
static const float MARCO_RUN_TORSO_ORIGIN_Y[] = { 27.0f, 28.0f, 27.0f, 26.0f, 27.0f, 27.0f, 28.0f, 28.0f, 27.0f, 26.0f, 27.0f, 27.0f };
static const float MARCO_FIRE_ORIGIN_X[] = { 11.0f, 11.0f, 11.0f };
static const float MARCO_FIRE_ORIGIN_Y[] = { 24.0f, 24.0f, 24.0f };
static const float MARCO_FACE_UP_ORIGIN_X[] = { 13.0f, 13.0f, 13.0f, 13.0f };
static const float MARCO_FACE_UP_ORIGIN_Y[] = { 30.0f, 30.0f, 30.0f, 30.0f };
static const float MARCO_SHOOT_UP_ORIGIN_X[] = { 13.5f, 13.5f, 13.5f, 13.5f };
static const float MARCO_SHOOT_UP_ORIGIN_Y[] = { 70.0f, 70.0f, 70.0f, 70.0f };
static const float MARCO_IDLE_LEG_ORIGIN_X[] = { 10.0f };
static const float MARCO_IDLE_LEG_ORIGIN_Y[] = { 15.0f };
static const float MARCO_RUN_LEG_ORIGIN_X[] = { 10.5f, 14.0f, 15.5f, 9.5f, 7.5f, 8.0f, 10.5f, 13.0f, 15.5f, 10.0f, 7.5f, 9.0f };
static const float MARCO_RUN_LEG_ORIGIN_Y[] = { 19.0f, 19.0f, 15.0f, 19.0f, 19.0f, 19.0f, 19.0f, 19.0f, 15.0f, 19.0f, 19.0f, 19.0f };

// Eri Animation Constants
static const sf::IntRect ERI_IDLE_TORSO_FRAMES[] =
{
	sf::IntRect(5, 4, 29, 29), sf::IntRect(39, 4, 29, 29), sf::IntRect(73, 4, 30, 28), sf::IntRect(108, 4, 29, 27)
};

static const sf::IntRect ERI_IDLE_LEG_FRAMES[] =
{
	sf::IntRect(2, 7, 21, 21)
};

static const sf::IntRect ERI_RUN_TORSO_FRAMES[] =
{
	sf::IntRect(5, 3, 32, 28), sf::IntRect(40, 3, 30, 29), sf::IntRect(75, 3, 28, 28), sf::IntRect(110, 3, 27, 27),
	sf::IntRect(145, 3, 29, 28), sf::IntRect(180, 3, 30, 28), sf::IntRect(215, 3, 32, 29), sf::IntRect(250, 3, 32, 29),
	sf::IntRect(285, 3, 32, 28), sf::IntRect(320, 3, 31, 27), sf::IntRect(355, 3, 31, 28), sf::IntRect(390, 3, 31, 28)
};

static const sf::IntRect ERI_RUN_LEG_FRAMES[] =
{
	sf::IntRect(4,   3, 24, 25), sf::IntRect(32,  2, 18, 26), sf::IntRect(55,  2, 20, 26), sf::IntRect(80,  2, 22, 26),
	sf::IntRect(109, 2, 25, 25), sf::IntRect(142, 3, 27, 23), sf::IntRect(174, 2, 25, 26), sf::IntRect(203, 2, 17, 26),
	sf::IntRect(226, 3, 18, 25), sf::IntRect(250, 3, 21, 25), sf::IntRect(277, 4, 26, 23), sf::IntRect(308, 4, 30, 25)
};

static const sf::IntRect ERI_FIRE_FRAMES[] =
{
	sf::IntRect(3, 3, 50, 23), sf::IntRect(61, 3, 51, 23), sf::IntRect(119, 3, 52, 23)
};

static const sf::IntRect ERI_FACE_UP_FRAMES[] =
{
	sf::IntRect(2, 0, 36, 32), sf::IntRect(36, 0, 36, 32), sf::IntRect(72, 0, 36, 32), sf::IntRect(108, 0, 38, 32)
};

static const sf::IntRect ERI_SHOOT_UP_FRAMES[] =
{
	sf::IntRect(0, 0, 27, 72), sf::IntRect(28, 0, 27, 72), sf::IntRect(56, 0, 27, 72), sf::IntRect(84, 0, 27, 72)
};

static const float ERI_IDLE_TORSO_ORIGIN_X[] = { 10.0f, 11.0f, 13.0f, 12.0f };
static const float ERI_IDLE_TORSO_ORIGIN_Y[] = { 20.0f, 20.0f, 20.0f, 20.0f };
static const float ERI_RUN_TORSO_ORIGIN_X[] = { 12.0f, 11.0f, 10.0f, 9.5f, 10.5f, 11.0f, 12.0f, 12.0f, 12.0f, 11.5f, 11.5f, 11.5f };
static const float ERI_RUN_TORSO_ORIGIN_Y[] = { 25.0f, 25.0f, 25.0f, 25.0f, 25.0f, 25.0f, 25.0f, 25.0f, 25.0f, 25.0f, 25.0f, 25.0f };
static const float ERI_FIRE_ORIGIN_X[] = { 11.0f, 11.0f, 11.0f };
static const float ERI_FIRE_ORIGIN_Y[] = { 21.0f, 21.0f, 21.0f };
static const float ERI_FACE_UP_ORIGIN_X[] = { 14.0f, 18.0f, 19.0f, 20.0f };
static const float ERI_FACE_UP_ORIGIN_Y[] = { 27.0f, 27.0f, 27.0f, 27.0f };
static const float ERI_SHOOT_UP_ORIGIN_X[] = { 13.5f, 13.5f, 13.5f, 13.5f };
static const float ERI_SHOOT_UP_ORIGIN_Y[] = { 66.0f, 66.0f, 66.0f, 66.0f };
static const float ERI_IDLE_LEG_ORIGIN_X[] = { 10.0f };
static const float ERI_IDLE_LEG_ORIGIN_Y[] = { 15.0f };
static const float ERI_RUN_LEG_ORIGIN_X[] = { 13.0f, 13.0f, 13.0f, 13.0f, 13.0f, 13.0f, 13.0f, 13.0f, 13.0f, 13.0f, 13.0f, 13.0f };
static const float ERI_RUN_LEG_ORIGIN_Y[] = { 22.0f, 22.0f, 22.0f, 22.0f, 22.0f, 22.0f, 22.0f, 22.0f, 22.0f, 22.0f, 22.0f, 22.0f };

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
	marcoSpritesLoaded = false;
	marcoTorsoState = -1;
	marcoLegState = -1;
	marcoTorsoFrames = 0;
	marcoLegFrames = 0;
	marcoTorsoFrameCount = 1;
	marcoLegFrameCount = 1;
	marcoTorsoFrame = 0;
	marcoLegFrame = 0;
	marcoTorsoTimer = 0.0f;
	marcoLegTimer = 0.0f;
	marcoTorsoFrameDuration = 0.12f;
	marcoLegFrameDuration = 0.09f;

	eriSpritesLoaded = false;
	eriTorsoState = -1;
	eriLegState = -1;
	eriTorsoFrames = 0;
	eriLegFrames = 0;
	eriTorsoFrameCount = 1;
	eriLegFrameCount = 1;
	eriTorsoFrame = 0;
	eriLegFrame = 0;
	eriTorsoTimer = 0.0f;
	eriLegTimer = 0.0f;
	eriTorsoFrameDuration = 0.12f;
	eriLegFrameDuration = 0.09f;

	ridingVehicle = false;
	setPosition(120.0f, 500.0f);
	setSpriteScale(2.2f);
	loadMarcoSprites();
	loadEriSprites();
	applyCharacterStats();
}

bool PlayerSoldier::loadMaskedTexture(sf::Texture& targetTexture, const char* fileName)
{
	sf::Image image;
	if (!image.loadFromFile(fileName))
	{
		return false;
	}

	image.createMaskFromColor(sf::Color::White);
	return targetTexture.loadFromImage(image);
}

void PlayerSoldier::loadMarcoSprites()
{
	if (loadMaskedTexture(marcoIdleTorsoTexture, "Sprites/Clean/Marco_idle_torso.png") &&
		loadMaskedTexture(marcoIdleLegsTexture, "Sprites/Clean/Marco_idle_legs.png") &&
		loadMaskedTexture(marcoRunTorsoTexture, "Sprites/Clean/Marco_run_torso.png") &&
		loadMaskedTexture(marcoRunLegsTexture, "Sprites/Clean/Marco_run_legs.png") &&
		loadMaskedTexture(marcoFireTexture, "Sprites/Clean/Marco_fire.png") &&
		loadMaskedTexture(marcoFaceUpTexture, "Sprites/Clean/Marco_faceUp.png") &&
		loadMaskedTexture(marcoShootUpTexture, "Sprites/Clean/Marco_shootUp.png"))
	{
		marcoSpritesLoaded = true;
		usingSprite = false;
		setMarcoTorsoAnimation(0, MARCO_IDLE_TORSO_FRAMES, 4, 0.16f, marcoIdleTorsoTexture);
		setMarcoLegAnimation(0, MARCO_IDLE_LEG_FRAMES, 1, 0.16f, marcoIdleLegsTexture);
	}
}

void PlayerSoldier::loadEriSprites()
{
	if (loadMaskedTexture(eriIdleTorsoTexture, "Sprites/Clean/Eri_idle_torso.png") &&
		loadMaskedTexture(eriIdleLegsTexture, "Sprites/Clean/Eri_idle_legs.png") &&
		loadMaskedTexture(eriRunTorsoTexture, "Sprites/Clean/Eri_run_torso.png") &&
		loadMaskedTexture(eriRunLegsTexture, "Sprites/Clean/Eri_run_legs.png") &&
		loadMaskedTexture(eriFireTexture, "Sprites/Clean/Eri_fire.png") &&
		loadMaskedTexture(eriFaceUpTexture, "Sprites/Clean/Eri_faceUp.png") &&
		loadMaskedTexture(eriShootUpTexture, "Sprites/Clean/Eri_shootUp.png"))
	{
		eriSpritesLoaded = true;
		usingSprite = false;
		setEriTorsoAnimation(0, ERI_IDLE_TORSO_FRAMES, 4, 0.16f, eriIdleTorsoTexture);
		setEriLegAnimation(0, ERI_IDLE_LEG_FRAMES, 1, 0.16f, eriIdleLegsTexture);
	}
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
	marcoTorsoState = -1;
	marcoLegState = -1;
	eriTorsoState = -1;
	eriLegState = -1;
	setRidingVehicle(false);
	playAnimation(Constants::PLAYER_ANIM_IDLE, 4, 0.18f);
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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
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
	if (currentCharacter == 0 && marcoSpritesLoaded)
	{
		updateMarcoLayeredAnimation(deltaTime);
		return;
	}
	else if (currentCharacter == 2 && eriSpritesLoaded)
	{
		updateEriLayeredAnimation(deltaTime);
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
			fireAnimationTimer = 0.24f;
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

void PlayerSoldier::setMarcoTorsoAnimation(int newState, const sf::IntRect* frames, int frameCount, float frameDuration, sf::Texture& texture)
{
	if (marcoTorsoState == newState)
	{
		return;
	}

	marcoTorsoState = newState;
	marcoTorsoFrames = frames;
	marcoTorsoFrameCount = frameCount;
	marcoTorsoFrame = 0;
	marcoTorsoTimer = 0.0f;
	marcoTorsoFrameDuration = frameDuration;
	marcoTorsoSprite.setTexture(texture, true);
	marcoTorsoSprite.setTextureRect(marcoTorsoFrames[0]);
	updateMarcoSpritePositions();
}

void PlayerSoldier::setMarcoLegAnimation(int newState, const sf::IntRect* frames, int frameCount, float frameDuration, sf::Texture& texture)
{
	if (marcoLegState == newState)
	{
		return;
	}

	marcoLegState = newState;
	marcoLegFrames = frames;
	marcoLegFrameCount = frameCount;
	marcoLegFrame = 0;
	marcoLegTimer = 0.0f;
	marcoLegFrameDuration = frameDuration;
	marcoLegsSprite.setTexture(texture, true);
	marcoLegsSprite.setTextureRect(marcoLegFrames[0]);
	updateMarcoSpritePositions();
}

void PlayerSoldier::updateMarcoLayeredAnimation(float deltaTime)
{
	bool running = currentState == Constants::SOLDIER_STATE_RUNNING;
	bool airborne = currentState == Constants::SOLDIER_STATE_JUMPING || currentState == Constants::SOLDIER_STATE_FALLING;

	if (fireAnimationTimer > 0.0f)
	{
		fireAnimationTimer -= deltaTime;
	}
	else
	{
		firing = false;
	}

	if (running && !airborne)
	{
		setMarcoLegAnimation(1, MARCO_RUN_LEG_FRAMES, 12, 0.075f, marcoRunLegsTexture);
	}
	else
	{
		setMarcoLegAnimation(0, MARCO_IDLE_LEG_FRAMES, 1, 0.16f, marcoIdleLegsTexture);
	}

	if (aimingUp && fireAnimationTimer > 0.0f)
	{
		setMarcoTorsoAnimation(4, MARCO_SHOOT_UP_FRAMES, 4, 0.08f, marcoShootUpTexture);
	}
	else if (aimingUp)
	{
		setMarcoTorsoAnimation(3, MARCO_FACE_UP_FRAMES, 4, 0.12f, marcoFaceUpTexture);
	}
	else if (fireAnimationTimer > 0.0f)
	{
		setMarcoTorsoAnimation(2, MARCO_FIRE_FRAMES, 3, 0.08f, marcoFireTexture);
	}
	else if (running && !airborne)
	{
		setMarcoTorsoAnimation(1, MARCO_RUN_TORSO_FRAMES, 12, 0.075f, marcoRunTorsoTexture);
	}
	else
	{
		setMarcoTorsoAnimation(0, MARCO_IDLE_TORSO_FRAMES, 4, 0.16f, marcoIdleTorsoTexture);
	}

	bool syncRunFrames = marcoTorsoState == 1 && marcoLegState == 1;

	if (!syncRunFrames)
	{
		marcoTorsoTimer += deltaTime;
		if (marcoTorsoFrameCount > 1 && marcoTorsoTimer >= marcoTorsoFrameDuration)
		{
			marcoTorsoTimer = 0.0f;
			marcoTorsoFrame += 1;
			if (marcoTorsoFrame >= marcoTorsoFrameCount)
			{
				marcoTorsoFrame = 0;
			}
			marcoTorsoSprite.setTextureRect(marcoTorsoFrames[marcoTorsoFrame]);
		}
	}

	marcoLegTimer += deltaTime;
	if (marcoLegFrameCount > 1 && marcoLegTimer >= marcoLegFrameDuration)
	{
		marcoLegTimer = 0.0f;
		marcoLegFrame += 1;
		if (marcoLegFrame >= marcoLegFrameCount)
		{
			marcoLegFrame = 0;
		}
		marcoLegsSprite.setTextureRect(marcoLegFrames[marcoLegFrame]);
	}

	if (syncRunFrames)
	{
		marcoTorsoFrame = marcoLegFrame;
		marcoTorsoSprite.setTextureRect(marcoTorsoFrames[marcoTorsoFrame]);
	}

	updateMarcoSpritePositions();
}

void PlayerSoldier::updateMarcoSpritePositions()
{
	if (!marcoSpritesLoaded)
	{
		return;
	}

	float scale = 2.45f;
	bool drawFlipped = !facingRight;
	if (drawFlipped)
	{
		marcoLegsSprite.setScale(-scale, scale);
		marcoTorsoSprite.setScale(-scale, scale);
	}
	else
	{
		marcoLegsSprite.setScale(scale, scale);
		marcoTorsoSprite.setScale(scale, scale);
	}

	float centerX = x + width / 2.0f;
	float legOriginX = MARCO_IDLE_LEG_ORIGIN_X[0];
	float legOriginY = MARCO_IDLE_LEG_ORIGIN_Y[0];
	if (marcoLegState == 1)
	{
		legOriginX = MARCO_RUN_LEG_ORIGIN_X[marcoLegFrame];
		legOriginY = MARCO_RUN_LEG_ORIGIN_Y[marcoLegFrame];
	}

	float torsoOriginX = MARCO_IDLE_TORSO_ORIGIN_X[marcoTorsoFrame];
	float torsoOriginY = MARCO_IDLE_TORSO_ORIGIN_Y[marcoTorsoFrame];
	if (marcoTorsoState == 1)
	{
		torsoOriginX = MARCO_RUN_TORSO_ORIGIN_X[marcoTorsoFrame];
		torsoOriginY = MARCO_RUN_TORSO_ORIGIN_Y[marcoTorsoFrame];
	}
	else if (marcoTorsoState == 2)
	{
		torsoOriginX = MARCO_FIRE_ORIGIN_X[marcoTorsoFrame];
		torsoOriginY = MARCO_FIRE_ORIGIN_Y[marcoTorsoFrame];
	}
	else if (marcoTorsoState == 3)
	{
		torsoOriginX = MARCO_FACE_UP_ORIGIN_X[marcoTorsoFrame];
		torsoOriginY = MARCO_FACE_UP_ORIGIN_Y[marcoTorsoFrame];
	}
	else if (marcoTorsoState == 4)
	{
		torsoOriginX = MARCO_SHOOT_UP_ORIGIN_X[marcoTorsoFrame];
		torsoOriginY = MARCO_SHOOT_UP_ORIGIN_Y[marcoTorsoFrame];
	}

	marcoLegsSprite.setOrigin(legOriginX, legOriginY);
	marcoTorsoSprite.setOrigin(torsoOriginX, torsoOriginY);

	float legAnchorY = y + height;
	float torsoOffsetY = 12.0f;
	marcoLegsSprite.setPosition(centerX, legAnchorY);
	marcoTorsoSprite.setPosition(centerX, legAnchorY - torsoOffsetY * scale);
}

void PlayerSoldier::setEriTorsoAnimation(int newState, const sf::IntRect* frames, int frameCount, float frameDuration, sf::Texture& texture)
{
	if (eriTorsoState == newState)
	{
		return;
	}

	eriTorsoState = newState;
	eriTorsoFrames = frames;
	eriTorsoFrameCount = frameCount;
	eriTorsoFrame = 0;
	eriTorsoTimer = 0.0f;
	eriTorsoFrameDuration = frameDuration;
	eriTorsoSprite.setTexture(texture, true);
	eriTorsoSprite.setTextureRect(eriTorsoFrames[0]);
	updateEriSpritePositions();
}

void PlayerSoldier::setEriLegAnimation(int newState, const sf::IntRect* frames, int frameCount, float frameDuration, sf::Texture& texture)
{
	if (eriLegState == newState)
	{
		return;
	}

	eriLegState = newState;
	eriLegFrames = frames;
	eriLegFrameCount = frameCount;
	eriLegFrame = 0;
	eriLegTimer = 0.0f;
	eriLegFrameDuration = frameDuration;
	eriLegsSprite.setTexture(texture, true);
	eriLegsSprite.setTextureRect(eriLegFrames[0]);
	updateEriSpritePositions();
}

void PlayerSoldier::updateEriLayeredAnimation(float deltaTime)
{
	bool running = currentState == Constants::SOLDIER_STATE_RUNNING;
	bool airborne = currentState == Constants::SOLDIER_STATE_JUMPING || currentState == Constants::SOLDIER_STATE_FALLING;

	if (fireAnimationTimer > 0.0f)
	{
		fireAnimationTimer -= deltaTime;
	}
	else
	{
		firing = false;
	}

	if (running && !airborne)
	{
		setEriLegAnimation(1, ERI_RUN_LEG_FRAMES, 12, 0.075f, eriRunLegsTexture);
	}
	else
	{
		setEriLegAnimation(0, ERI_IDLE_LEG_FRAMES, 1, 0.16f, eriIdleLegsTexture);
	}

	if (aimingUp && fireAnimationTimer > 0.0f)
	{
		setEriTorsoAnimation(4, ERI_SHOOT_UP_FRAMES, 4, 0.08f, eriShootUpTexture);
	}
	else if (aimingUp)
	{
		setEriTorsoAnimation(3, ERI_FACE_UP_FRAMES, 4, 0.12f, eriFaceUpTexture);
	}
	else if (fireAnimationTimer > 0.0f)
	{
		setEriTorsoAnimation(2, ERI_FIRE_FRAMES, 3, 0.08f, eriFireTexture);
	}
	else if (running && !airborne)
	{
		setEriTorsoAnimation(1, ERI_RUN_TORSO_FRAMES, 12, 0.075f, eriRunTorsoTexture);
	}
	else
	{
		setEriTorsoAnimation(0, ERI_IDLE_TORSO_FRAMES, 4, 0.16f, eriIdleTorsoTexture);
	}

	bool syncRunFrames = eriTorsoState == 1 && eriLegState == 1;

	if (!syncRunFrames)
	{
		eriTorsoTimer += deltaTime;
		if (eriTorsoFrameCount > 1 && eriTorsoTimer >= eriTorsoFrameDuration)
		{
			eriTorsoTimer = 0.0f;
			eriTorsoFrame += 1;
			if (eriTorsoFrame >= eriTorsoFrameCount)
			{
				eriTorsoFrame = 0;
			}
			eriTorsoSprite.setTextureRect(eriTorsoFrames[eriTorsoFrame]);
		}
	}

	eriLegTimer += deltaTime;
	if (eriLegFrameCount > 1 && eriLegTimer >= eriLegFrameDuration)
	{
		eriLegTimer = 0.0f;
		eriLegFrame += 1;
		if (eriLegFrame >= eriLegFrameCount)
		{
			eriLegFrame = 0;
		}
		eriLegsSprite.setTextureRect(eriLegFrames[eriLegFrame]);
	}

	if (syncRunFrames)
	{
		eriTorsoFrame = eriLegFrame;
		eriTorsoSprite.setTextureRect(eriTorsoFrames[eriTorsoFrame]);
	}

	updateEriSpritePositions();
}

void PlayerSoldier::updateEriSpritePositions()
{
	if (!eriSpritesLoaded)
	{
		return;
	}

	float scale = 2.45f;
	bool drawFlipped = !facingRight;
	if (drawFlipped)
	{
		eriLegsSprite.setScale(-scale, scale);
		eriTorsoSprite.setScale(-scale, scale);
	}
	else
	{
		eriLegsSprite.setScale(scale, scale);
		eriTorsoSprite.setScale(scale, scale);
	}

	float centerX = x + width / 2.0f;
	float legOriginX = ERI_IDLE_LEG_ORIGIN_X[0];
	float legOriginY = ERI_IDLE_LEG_ORIGIN_Y[0];
	if (eriLegState == 1)
	{
		legOriginX = ERI_RUN_LEG_ORIGIN_X[eriLegFrame];
		legOriginY = ERI_RUN_LEG_ORIGIN_Y[eriLegFrame];
	}

	float torsoOriginX = ERI_IDLE_TORSO_ORIGIN_X[eriTorsoFrame];
	float torsoOriginY = ERI_IDLE_TORSO_ORIGIN_Y[eriTorsoFrame];
	if (eriTorsoState == 1)
	{
		torsoOriginX = ERI_RUN_TORSO_ORIGIN_X[eriTorsoFrame];
		torsoOriginY = ERI_RUN_TORSO_ORIGIN_Y[eriTorsoFrame];
	}
	else if (eriTorsoState == 2)
	{
		torsoOriginX = ERI_FIRE_ORIGIN_X[eriTorsoFrame];
		torsoOriginY = ERI_FIRE_ORIGIN_Y[eriTorsoFrame];
	}
	else if (eriTorsoState == 3)
	{
		torsoOriginX = ERI_FACE_UP_ORIGIN_X[eriTorsoFrame];
		torsoOriginY = ERI_FACE_UP_ORIGIN_Y[eriTorsoFrame];
	}
	else if (eriTorsoState == 4)
	{
		torsoOriginX = ERI_SHOOT_UP_ORIGIN_X[eriTorsoFrame];
		torsoOriginY = ERI_SHOOT_UP_ORIGIN_Y[eriTorsoFrame];
	}

	eriLegsSprite.setOrigin(legOriginX, legOriginY);
	eriTorsoSprite.setOrigin(torsoOriginX, torsoOriginY);

	float legAnchorY = y + height;
	float torsoOffsetY = 12.0f;
	eriLegsSprite.setPosition(centerX, legAnchorY);
	eriTorsoSprite.setPosition(centerX, legAnchorY - torsoOffsetY * scale);
}

void PlayerSoldier::draw(sf::RenderWindow& window)
{
	if (!visible)
	{
		return;
	}

	if (currentCharacter == 0 && marcoSpritesLoaded)
	{
		window.draw(marcoLegsSprite);
		window.draw(marcoTorsoSprite);
	}
	else if (currentCharacter == 2 && eriSpritesLoaded)
	{
		window.draw(eriLegsSprite);
		window.draw(eriTorsoSprite);
	}

	if ((currentCharacter == 0 && marcoSpritesLoaded) || (currentCharacter == 2 && eriSpritesLoaded))
	{
		if (maxHealth > 0)
		{
			float healthRatio = static_cast<float>(health) / static_cast<float>(maxHealth);
			if (healthRatio < 0.0f)
			{
				healthRatio = 0.0f;
			}

			sf::RectangleShape healthBack;
			healthBack.setPosition(x, y - 12.0f);
			healthBack.setSize(sf::Vector2f(width, 6.0f));
			healthBack.setFillColor(sf::Color(80, 20, 20));
			window.draw(healthBack);

			sf::RectangleShape healthFront;
			healthFront.setPosition(x, y - 12.0f);
			healthFront.setSize(sf::Vector2f(width * healthRatio, 6.0f));
			healthFront.setFillColor(sf::Color(60, 220, 90));
			window.draw(healthFront);
		}
		return;
	}

	Soldier::draw(window);
}
