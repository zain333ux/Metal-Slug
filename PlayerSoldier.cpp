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

// Tarma Animation Constants
static const sf::IntRect TARMA_IDLE_TORSO_FRAMES[] =
{
	sf::IntRect(4, 4, 32, 26), sf::IntRect(40, 4, 32, 26), sf::IntRect(76, 4, 32, 26), sf::IntRect(112, 4, 32, 26)
};

static const sf::IntRect TARMA_IDLE_LEG_FRAMES[] =
{
	sf::IntRect(3, 2, 22, 18)
};

static const sf::IntRect TARMA_RUN_TORSO_FRAMES[] =
{
	sf::IntRect(1,   3, 32, 28), sf::IntRect(43,  2, 30, 29), sf::IntRect(82,  2, 28, 28), sf::IntRect(119, 2, 27, 27),
	sf::IntRect(154, 2, 29, 28), sf::IntRect(192, 2, 30, 28), sf::IntRect(232, 3, 32, 28), sf::IntRect(273, 2, 32, 29),
	sf::IntRect(313, 2, 32, 28), sf::IntRect(353, 3, 31, 27), sf::IntRect(392, 3, 31, 28), sf::IntRect(430, 3, 31, 28)
};

static const sf::IntRect TARMA_RUN_LEG_FRAMES[] =
{
	sf::IntRect(4,   3, 21, 20), sf::IntRect(40,  3, 28, 20), sf::IntRect(78,  3, 31, 16), sf::IntRect(120, 3, 19, 20),
	sf::IntRect(157, 3, 15, 20), sf::IntRect(194, 3, 16, 20), sf::IntRect(232, 3, 21, 20), sf::IntRect(268, 3, 26, 20),
	sf::IntRect(310, 3, 31, 16), sf::IntRect(353, 3, 20, 20), sf::IntRect(394, 3, 15, 20), sf::IntRect(430, 3, 18, 20)
};

static const sf::IntRect TARMA_FIRE_FRAMES[] =
{
	sf::IntRect(4, 3, 50, 23), sf::IntRect(60, 3, 51, 23), sf::IntRect(116, 3, 52, 23)
};

static const sf::IntRect TARMA_FACE_UP_FRAMES[] =
{
	sf::IntRect(0, 0, 33, 31), sf::IntRect(33, 0, 33, 31), sf::IntRect(66, 0, 33, 31), sf::IntRect(99, 0, 33, 31)
};

static const sf::IntRect TARMA_SHOOT_UP_FRAMES[] =
{
	sf::IntRect(0, 0, 29, 69), sf::IntRect(29, 0, 29, 69), sf::IntRect(58, 0, 29, 69), sf::IntRect(87, 0, 29, 69)
};

static const float TARMA_IDLE_TORSO_ORIGIN_X[] = { 7.0f, 6.0f, 6.0f, 7.0f };
static const float TARMA_IDLE_TORSO_ORIGIN_Y[] = { 22.5f, 22.5f, 22.5f, 22.5f };
static const float TARMA_RUN_TORSO_ORIGIN_X[] = { 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f, 11.0f };
static const float TARMA_RUN_TORSO_ORIGIN_Y[] = { 25.0f, 25.0f, 25.0f, 25.0f, 25.0f, 25.0f, 25.0f, 25.0f, 25.0f, 25.0f, 25.0f, 25.0f };
static const float TARMA_FIRE_ORIGIN_X[] = { 9.0f, 9.0f, 9.0f };
static const float TARMA_FIRE_ORIGIN_Y[] = { 22.5f, 22.5f, 22.5f };
static const float TARMA_FACE_UP_ORIGIN_X[] = { 11.0f, 12.0f, 13.0f, 14.0f };
static const float TARMA_FACE_UP_ORIGIN_Y[] = { 29.0f, 29.0f, 29.0f, 29.0f };
static const float TARMA_SHOOT_UP_ORIGIN_X[] = { 14.5f, 14.5f, 14.5f, 14.5f };
static const float TARMA_SHOOT_UP_ORIGIN_Y[] = { 67.0f, 67.0f, 67.0f, 67.0f };
static const float TARMA_IDLE_LEG_ORIGIN_X[] = { 10.0f };
static const float TARMA_IDLE_LEG_ORIGIN_Y[] = { 15.0f };
static const float TARMA_RUN_LEG_ORIGIN_X[] = { 13.0f, 13.0f, 13.0f, 13.0f, 13.0f, 13.0f, 13.0f, 13.0f, 13.0f, 13.0f, 13.0f, 13.0f };
static const float TARMA_RUN_LEG_ORIGIN_Y[] = { 17.0f, 17.0f, 17.0f, 17.0f, 17.0f, 17.0f, 17.0f, 17.0f, 17.0f, 17.0f, 17.0f, 17.0f };

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

// Fio — frame rects from Sprites/Clean/Fio_*.png (tight alpha bounds); pivots at foot mid-x / bottom row
static const sf::IntRect FIO_IDLE_TORSO_FRAMES[] =
{
	sf::IntRect(3, 4, 28, 26), sf::IntRect(36, 4, 28, 26), sf::IntRect(69, 4, 28, 26), sf::IntRect(102, 4, 28, 26)
};

static const sf::IntRect FIO_IDLE_LEG_FRAMES[] =
{
	sf::IntRect(6, 4, 20, 24)
};

static const sf::IntRect FIO_RUN_TORSO_FRAMES[] =
{
	sf::IntRect(3, 3, 27, 26), sf::IntRect(35, 4, 27, 26), sf::IntRect(67, 5, 27, 26), sf::IntRect(99, 4, 27, 26),
	sf::IntRect(131, 3, 27, 26), sf::IntRect(163, 3, 28, 26), sf::IntRect(196, 4, 28, 26), sf::IntRect(229, 5, 28, 26),
	sf::IntRect(262, 5, 29, 26), sf::IntRect(296, 6, 28, 26), sf::IntRect(329, 4, 26, 27), sf::IntRect(360, 4, 27, 27)
};

static const sf::IntRect FIO_RUN_LEG_FRAMES[] =
{
	sf::IntRect(3, 5, 22, 26), sf::IntRect(30, 5, 25, 25), sf::IntRect(60, 6, 27, 23), sf::IntRect(92, 5, 25, 26),
	sf::IntRect(122, 5, 17, 26), sf::IntRect(144, 6, 18, 25), sf::IntRect(167, 6, 21, 25), sf::IntRect(193, 6, 26, 23),
	sf::IntRect(224, 6, 30, 25), sf::IntRect(259, 6, 24, 25), sf::IntRect(288, 5, 18, 26), sf::IntRect(311, 5, 20, 26)
};

static const sf::IntRect FIO_FIRE_FRAMES[] =
{
	sf::IntRect(3, 4, 48, 19), sf::IntRect(56, 4, 58, 19), sf::IntRect(114, 3, 49, 20)
};

static const sf::IntRect FIO_FACE_UP_FRAMES[] =
{
	sf::IntRect(3, 4, 24, 23), sf::IntRect(32, 4, 24, 23), sf::IntRect(61, 4, 25, 23), sf::IntRect(91, 4, 25, 23)
};

static const sf::IntRect FIO_SHOOT_UP_FRAMES[] =
{
	sf::IntRect(8, 10, 22, 59), sf::IntRect(32, 7, 22, 62), sf::IntRect(51, 5, 27, 64), sf::IntRect(75, 36, 28, 33)
};

static const float FIO_IDLE_TORSO_ORIGIN_X[] = { 11.5f, 11.5f, 11.5f, 11.5f };
static const float FIO_IDLE_TORSO_ORIGIN_Y[] = { 26.0f, 26.0f, 26.0f, 26.0f };
static const float FIO_RUN_TORSO_ORIGIN_X[] = { 13.0f, 13.0f, 13.0f, 13.0f, 13.0f, 13.5f, 13.5f, 13.5f, 14.0f, 13.5f, 12.5f, 13.0f };
static const float FIO_RUN_TORSO_ORIGIN_Y[] = { 26.0f, 26.0f, 26.0f, 26.0f, 26.0f, 26.0f, 26.0f, 26.0f, 26.0f, 26.0f, 27.0f, 27.0f };
static const float FIO_FIRE_ORIGIN_X[] = { 10.0f, 11.0f, 10.0f };
static const float FIO_FIRE_ORIGIN_Y[] = { 23.0f, 23.0f, 23.0f };
static const float FIO_FACE_UP_ORIGIN_X[] = { 8.5f, 8.5f, 8.5f, 8.5f };
static const float FIO_FACE_UP_ORIGIN_Y[] = { 26.0f, 26.0f, 26.0f, 26.0f };
static const float FIO_SHOOT_UP_ORIGIN_X[] = { 9.0f, 9.0f, 10.0f, 11.0f };
static const float FIO_SHOOT_UP_ORIGIN_Y[] = { 64.0f, 65.0f, 66.0f, 66.0f };
static const float FIO_IDLE_LEG_ORIGIN_X[] = { 9.5f };
static const float FIO_IDLE_LEG_ORIGIN_Y[] = { 24.0f };
static const float FIO_RUN_LEG_ORIGIN_X[] = { 15.0f, 15.0f, 15.0f, 15.0f, 15.0f, 15.0f, 15.0f, 15.0f, 15.0f, 15.0f, 15.0f, 15.0f };
static const float FIO_RUN_LEG_ORIGIN_Y[] = { 26.0f, 25.0f, 23.0f, 26.0f, 26.0f, 25.0f, 25.0f, 23.0f, 25.0f, 25.0f, 26.0f, 26.0f };

PlayerSoldier::PlayerSoldier()
{
	firing = false;
	fireAnimationTimer = 0.0f;
	currentCharacter = 0;
	for (int i = 0; i < 4; i += 1)
	{
		lives[i] = 2;
		grenades[i] = 10;
		rockets[i] = 0;
		hmgBullets[i] = 0;
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

	tarmaSpritesLoaded = false;
	tarmaTorsoState = -1;
	tarmaLegState = -1;
	tarmaTorsoFrames = 0;
	tarmaLegFrames = 0;
	tarmaTorsoFrameCount = 1;
	tarmaLegFrameCount = 1;
	tarmaTorsoFrame = 0;
	tarmaLegFrame = 0;
	tarmaTorsoTimer = 0.0f;
	tarmaLegTimer = 0.0f;
	tarmaTorsoFrameDuration = 0.12f;
	tarmaLegFrameDuration = 0.09f;

	fioSpritesLoaded = false;
	fioTorsoState = -1;
	fioLegState = -1;
	fioTorsoFrames = 0;
	fioLegFrames = 0;
	fioTorsoFrameCount = 1;
	fioLegFrameCount = 1;
	fioTorsoFrame = 0;
	fioLegFrame = 0;
	fioTorsoTimer = 0.0f;
	fioLegTimer = 0.0f;
	fioTorsoFrameDuration = 0.12f;
	fioLegFrameDuration = 0.09f;

	ridingVehicle = false;
	pilotHiddenWhileInsideVehicle = false;
	setPosition(120.0f, 500.0f);
	setSpriteScale(2.2f);
	loadMarcoSprites();
	loadEriSprites();
	loadTarmaSprites();
	loadFioSprites();
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

void PlayerSoldier::loadTarmaSprites()
{
	if (loadMaskedTexture(tarmaIdleTorsoTexture, "Sprites/Clean/Tarma_idle_torso.png") &&
		loadMaskedTexture(tarmaIdleLegsTexture, "Sprites/Clean/Tarma_idle_legs.png") &&
		loadMaskedTexture(tarmaRunTorsoTexture, "Sprites/Clean/Tarma_run_torso.png") &&
		loadMaskedTexture(tarmaRunLegsTexture, "Sprites/Clean/Tarma_run_legs.png") &&
		loadMaskedTexture(tarmaFireTexture, "Sprites/Clean/Tarma_fire.png") &&
		loadMaskedTexture(tarmaFaceUpTexture, "Sprites/Clean/Tarma_faceUp.png") &&
		loadMaskedTexture(tarmaShootUpTexture, "Sprites/Clean/Tarma_shootUp.png"))
	{
		tarmaSpritesLoaded = true;
		usingSprite = false;
		setTarmaTorsoAnimation(0, TARMA_IDLE_TORSO_FRAMES, 4, 0.16f, tarmaIdleTorsoTexture);
		setTarmaLegAnimation(0, TARMA_IDLE_LEG_FRAMES, 1, 0.16f, tarmaIdleLegsTexture);
	}
}

void PlayerSoldier::loadFioSprites()
{
	if (loadMaskedTexture(fioIdleTorsoTexture, "Sprites/Clean/Fio_idle_torso.png") &&
		loadMaskedTexture(fioIdleLegsTexture, "Sprites/Clean/Fio_idle_legs.png") &&
		loadMaskedTexture(fioRunTorsoTexture, "Sprites/Clean/Fio_run_torso.png") &&
		loadMaskedTexture(fioRunLegsTexture, "Sprites/Clean/Fio_run_legs.png") &&
		loadMaskedTexture(fioFireTexture, "Sprites/Clean/Fio_fire.png") &&
		loadMaskedTexture(fioFaceUpTexture, "Sprites/Clean/Fio_faceUp.png") &&
		loadMaskedTexture(fioShootUpTexture, "Sprites/Clean/Fio_shootUp.png"))
	{
		fioSpritesLoaded = true;
		usingSprite = false;
		setFioTorsoAnimation(0, FIO_IDLE_TORSO_FRAMES, 4, 0.16f, fioIdleTorsoTexture);
		setFioLegAnimation(0, FIO_IDLE_LEG_FRAMES, 1, 0.16f, fioIdleLegsTexture);
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
	tarmaTorsoState = -1;
	tarmaLegState = -1;
	fioTorsoState = -1;
	fioLegState = -1;
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
	else if (currentCharacter == 1 && tarmaSpritesLoaded)
	{
		updateTarmaLayeredAnimation(deltaTime);
		return;
	}
	else if (currentCharacter == 3 && fioSpritesLoaded)
	{
		updateFioLayeredAnimation(deltaTime);
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
		bool canFireWeapon = weapon.canFire();
		if (canFireWeapon)
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

		if (canFireWeapon)
		{
			if (rockets[currentCharacter] > 0 || (DeveloperMode::isEnabled() && currentCharacter == 2))
			{
				RocketProjectile* rocket = new RocketProjectile(bulletX, bulletY, facingRight, aimingUp);
				entityManager.addEntity(rocket);
				if (!DeveloperMode::isEnabled())
				{
					rockets[currentCharacter] -= 1;
				}
				weapon.restartCooldown();
			}
			else if (hmgBullets[currentCharacter] > 0 || (DeveloperMode::isEnabled() && currentCharacter == 1))
			{
				weapon.fire(entityManager, bulletX, bulletY, facingRight, aimingUp);
				if (!DeveloperMode::isEnabled())
				{
					hmgBullets[currentCharacter] -= 1;
				}
				// Double fire rate: set cooldown to half of the normal cooldown
				weapon.setCooldown(Constants::PLAYER_FIRE_COOLDOWN * 0.5f);
				weapon.restartCooldown();
				// Note: cooldown will be reset to character default in applyCharacterStats if they switch, 
				// but for HMG we want it persistent while firing.
			}
			else
			{
				// Ensure cooldown is normal for pistol
				applyCharacterStats(); 
				weapon.fire(entityManager, bulletX, bulletY, facingRight, aimingUp);
			}
		}
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

	previousGrenadeKey = grenadeKey;
}

void PlayerSoldier::refillDemoInventory()
{
	for (int i = 0; i < 4; i += 1)
	{
		grenades[i] = 99;
		rockets[i] = 0;
		hmgBullets[i] = 0;
	}

	// Marco: Pistol (already set above)
	// Tarma (1): HMG
	hmgBullets[1] = 999;
	// Eri (2): Rockets
	rockets[2] = 99;
	// Fio (3): Pistol
}

void PlayerSoldier::addRocketAmmo(int amount)
{
	if (amount <= 0)
	{
		return;
	}

	rockets[currentCharacter] += amount;
	if (rockets[currentCharacter] > 99)
	{
		rockets[currentCharacter] = 99;
	}
}

void PlayerSoldier::addHmgAmmo(int amount)
{
	if (amount <= 0)
	{
		return;
	}

	hmgBullets[currentCharacter] += amount;
	if (hmgBullets[currentCharacter] > 999)
	{
		hmgBullets[currentCharacter] = 999;
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

int PlayerSoldier::getHmgBullets() const
{
	return hmgBullets[currentCharacter];
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

void PlayerSoldier::setTarmaTorsoAnimation(int newState, const sf::IntRect* frames, int frameCount, float frameDuration, sf::Texture& texture)
{
	if (tarmaTorsoState == newState)
	{
		return;
	}

	tarmaTorsoState = newState;
	tarmaTorsoFrames = frames;
	tarmaTorsoFrameCount = frameCount;
	tarmaTorsoFrame = 0;
	tarmaTorsoTimer = 0.0f;
	tarmaTorsoFrameDuration = frameDuration;
	tarmaTorsoSprite.setTexture(texture, true);
	tarmaTorsoSprite.setTextureRect(tarmaTorsoFrames[0]);
	updateTarmaSpritePositions();
}

void PlayerSoldier::setTarmaLegAnimation(int newState, const sf::IntRect* frames, int frameCount, float frameDuration, sf::Texture& texture)
{
	if (tarmaLegState == newState)
	{
		return;
	}

	tarmaLegState = newState;
	tarmaLegFrames = frames;
	tarmaLegFrameCount = frameCount;
	tarmaLegFrame = 0;
	tarmaLegTimer = 0.0f;
	tarmaLegFrameDuration = frameDuration;
	tarmaLegsSprite.setTexture(texture, true);
	tarmaLegsSprite.setTextureRect(tarmaLegFrames[0]);
	updateTarmaSpritePositions();
}

void PlayerSoldier::updateTarmaLayeredAnimation(float deltaTime)
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
		setTarmaLegAnimation(1, TARMA_RUN_LEG_FRAMES, 12, 0.075f, tarmaRunLegsTexture);
	}
	else
	{
		setTarmaLegAnimation(0, TARMA_IDLE_LEG_FRAMES, 1, 0.16f, tarmaIdleLegsTexture);
	}

	if (aimingUp && fireAnimationTimer > 0.0f)
	{
		setTarmaTorsoAnimation(4, TARMA_SHOOT_UP_FRAMES, 4, 0.08f, tarmaShootUpTexture);
	}
	else if (aimingUp)
	{
		setTarmaTorsoAnimation(3, TARMA_FACE_UP_FRAMES, 4, 0.12f, tarmaFaceUpTexture);
	}
	else if (fireAnimationTimer > 0.0f)
	{
		setTarmaTorsoAnimation(2, TARMA_FIRE_FRAMES, 3, 0.08f, tarmaFireTexture);
	}
	else if (running && !airborne)
	{
		setTarmaTorsoAnimation(1, TARMA_RUN_TORSO_FRAMES, 12, 0.075f, tarmaRunTorsoTexture);
	}
	else
	{
		setTarmaTorsoAnimation(0, TARMA_IDLE_TORSO_FRAMES, 4, 0.16f, tarmaIdleTorsoTexture);
	}

	bool syncRunFrames = tarmaTorsoState == 1 && tarmaLegState == 1;

	if (!syncRunFrames)
	{
		tarmaTorsoTimer += deltaTime;
		if (tarmaTorsoFrameCount > 1 && tarmaTorsoTimer >= tarmaTorsoFrameDuration)
		{
			tarmaTorsoTimer = 0.0f;
			tarmaTorsoFrame += 1;
			if (tarmaTorsoFrame >= tarmaTorsoFrameCount)
			{
				tarmaTorsoFrame = 0;
			}
			tarmaTorsoSprite.setTextureRect(tarmaTorsoFrames[tarmaTorsoFrame]);
		}
	}

	tarmaLegTimer += deltaTime;
	if (tarmaLegFrameCount > 1 && tarmaLegTimer >= tarmaLegFrameDuration)
	{
		tarmaLegTimer = 0.0f;
		tarmaLegFrame += 1;
		if (tarmaLegFrame >= tarmaLegFrameCount)
		{
			tarmaLegFrame = 0;
		}
		tarmaLegsSprite.setTextureRect(tarmaLegFrames[tarmaLegFrame]);
	}

	if (syncRunFrames)
	{
		tarmaTorsoFrame = tarmaLegFrame;
		tarmaTorsoSprite.setTextureRect(tarmaTorsoFrames[tarmaTorsoFrame]);
	}

	updateTarmaSpritePositions();
}

void PlayerSoldier::updateTarmaSpritePositions()
{
	if (!tarmaSpritesLoaded)
	{
		return;
	}

	float scale = 2.45f;
	bool drawFlipped = !facingRight;
	if (drawFlipped)
	{
		tarmaLegsSprite.setScale(-scale, scale);
		tarmaTorsoSprite.setScale(-scale, scale);
	}
	else
	{
		tarmaLegsSprite.setScale(scale, scale);
		tarmaTorsoSprite.setScale(scale, scale);
	}

	float centerX = x + width / 2.0f;
	float legOriginX = TARMA_IDLE_LEG_ORIGIN_X[0];
	float legOriginY = TARMA_IDLE_LEG_ORIGIN_Y[0];
	if (tarmaLegState == 1)
	{
		legOriginX = TARMA_RUN_LEG_ORIGIN_X[tarmaLegFrame];
		legOriginY = TARMA_RUN_LEG_ORIGIN_Y[tarmaLegFrame];
	}

	float torsoOriginX = TARMA_IDLE_TORSO_ORIGIN_X[tarmaTorsoFrame];
	float torsoOriginY = TARMA_IDLE_TORSO_ORIGIN_Y[tarmaTorsoFrame];
	if (tarmaTorsoState == 1)
	{
		torsoOriginX = TARMA_RUN_TORSO_ORIGIN_X[tarmaTorsoFrame];
		torsoOriginY = TARMA_RUN_TORSO_ORIGIN_Y[tarmaTorsoFrame];
	}
	else if (tarmaTorsoState == 2)
	{
		torsoOriginX = TARMA_FIRE_ORIGIN_X[tarmaTorsoFrame];
		torsoOriginY = TARMA_FIRE_ORIGIN_Y[tarmaTorsoFrame];
	}
	else if (tarmaTorsoState == 3)
	{
		torsoOriginX = TARMA_FACE_UP_ORIGIN_X[tarmaTorsoFrame];
		torsoOriginY = TARMA_FACE_UP_ORIGIN_Y[tarmaTorsoFrame];
	}
	else if (tarmaTorsoState == 4)
	{
		torsoOriginX = TARMA_SHOOT_UP_ORIGIN_X[tarmaTorsoFrame];
		torsoOriginY = TARMA_SHOOT_UP_ORIGIN_Y[tarmaTorsoFrame];
	}

	tarmaLegsSprite.setOrigin(legOriginX, legOriginY);
	tarmaTorsoSprite.setOrigin(torsoOriginX, torsoOriginY);

	float legAnchorY = y + height;
	float torsoOffsetY = 12.0f;
	tarmaLegsSprite.setPosition(centerX, legAnchorY);
	tarmaTorsoSprite.setPosition(centerX, legAnchorY - torsoOffsetY * scale);
}

void PlayerSoldier::setFioTorsoAnimation(int newState, const sf::IntRect* frames, int frameCount, float frameDuration, sf::Texture& texture)
{
	if (fioTorsoState == newState)
	{
		return;
	}

	fioTorsoState = newState;
	fioTorsoFrames = frames;
	fioTorsoFrameCount = frameCount;
	fioTorsoFrame = 0;
	fioTorsoTimer = 0.0f;
	fioTorsoFrameDuration = frameDuration;
	fioTorsoSprite.setTexture(texture, true);
	fioTorsoSprite.setTextureRect(fioTorsoFrames[0]);
	updateFioSpritePositions();
}

void PlayerSoldier::setFioLegAnimation(int newState, const sf::IntRect* frames, int frameCount, float frameDuration, sf::Texture& texture)
{
	if (fioLegState == newState)
	{
		return;
	}

	fioLegState = newState;
	fioLegFrames = frames;
	fioLegFrameCount = frameCount;
	fioLegFrame = 0;
	fioLegTimer = 0.0f;
	fioLegFrameDuration = frameDuration;
	fioLegsSprite.setTexture(texture, true);
	fioLegsSprite.setTextureRect(fioLegFrames[0]);
	updateFioSpritePositions();
}

void PlayerSoldier::updateFioLayeredAnimation(float deltaTime)
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
		setFioLegAnimation(1, FIO_RUN_LEG_FRAMES, 12, 0.075f, fioRunLegsTexture);
	}
	else
	{
		setFioLegAnimation(0, FIO_IDLE_LEG_FRAMES, 1, 0.16f, fioIdleLegsTexture);
	}

	if (aimingUp && fireAnimationTimer > 0.0f)
	{
		setFioTorsoAnimation(4, FIO_SHOOT_UP_FRAMES, 4, 0.08f, fioShootUpTexture);
	}
	else if (aimingUp)
	{
		setFioTorsoAnimation(3, FIO_FACE_UP_FRAMES, 4, 0.12f, fioFaceUpTexture);
	}
	else if (fireAnimationTimer > 0.0f)
	{
		setFioTorsoAnimation(2, FIO_FIRE_FRAMES, 3, 0.08f, fioFireTexture);
	}
	else if (running && !airborne)
	{
		setFioTorsoAnimation(1, FIO_RUN_TORSO_FRAMES, 12, 0.075f, fioRunTorsoTexture);
	}
	else
	{
		setFioTorsoAnimation(0, FIO_IDLE_TORSO_FRAMES, 4, 0.16f, fioIdleTorsoTexture);
	}

	bool syncRunFrames = fioTorsoState == 1 && fioLegState == 1;

	if (!syncRunFrames)
	{
		fioTorsoTimer += deltaTime;
		if (fioTorsoFrameCount > 1 && fioTorsoTimer >= fioTorsoFrameDuration)
		{
			fioTorsoTimer = 0.0f;
			fioTorsoFrame += 1;
			if (fioTorsoFrame >= fioTorsoFrameCount)
			{
				fioTorsoFrame = 0;
			}
			fioTorsoSprite.setTextureRect(fioTorsoFrames[fioTorsoFrame]);
		}
	}

	fioLegTimer += deltaTime;
	if (fioLegFrameCount > 1 && fioLegTimer >= fioLegFrameDuration)
	{
		fioLegTimer = 0.0f;
		fioLegFrame += 1;
		if (fioLegFrame >= fioLegFrameCount)
		{
			fioLegFrame = 0;
		}
		fioLegsSprite.setTextureRect(fioLegFrames[fioLegFrame]);
	}

	if (syncRunFrames)
	{
		fioTorsoFrame = fioLegFrame;
		fioTorsoSprite.setTextureRect(fioTorsoFrames[fioTorsoFrame]);
	}

	updateFioSpritePositions();
}

void PlayerSoldier::updateFioSpritePositions()
{
	if (!fioSpritesLoaded)
	{
		return;
	}

	float scale = 2.45f;
	bool drawFlipped = !facingRight;
	if (drawFlipped)
	{
		fioLegsSprite.setScale(-scale, scale);
		fioTorsoSprite.setScale(-scale, scale);
	}
	else
	{
		fioLegsSprite.setScale(scale, scale);
		fioTorsoSprite.setScale(scale, scale);
	}

	float centerX = x + width / 2.0f;
	float legOriginX = FIO_IDLE_LEG_ORIGIN_X[0];
	float legOriginY = FIO_IDLE_LEG_ORIGIN_Y[0];
	if (fioLegState == 1)
	{
		legOriginX = FIO_RUN_LEG_ORIGIN_X[fioLegFrame];
		legOriginY = FIO_RUN_LEG_ORIGIN_Y[fioLegFrame];
	}

	float torsoOriginX = FIO_IDLE_TORSO_ORIGIN_X[fioTorsoFrame];
	float torsoOriginY = FIO_IDLE_TORSO_ORIGIN_Y[fioTorsoFrame];
	if (fioTorsoState == 1)
	{
		torsoOriginX = FIO_RUN_TORSO_ORIGIN_X[fioTorsoFrame];
		torsoOriginY = FIO_RUN_TORSO_ORIGIN_Y[fioTorsoFrame];
	}
	else if (fioTorsoState == 2)
	{
		torsoOriginX = FIO_FIRE_ORIGIN_X[fioTorsoFrame];
		torsoOriginY = FIO_FIRE_ORIGIN_Y[fioTorsoFrame];
	}
	else if (fioTorsoState == 3)
	{
		torsoOriginX = FIO_FACE_UP_ORIGIN_X[fioTorsoFrame];
		torsoOriginY = FIO_FACE_UP_ORIGIN_Y[fioTorsoFrame];
	}
	else if (fioTorsoState == 4)
	{
		torsoOriginX = FIO_SHOOT_UP_ORIGIN_X[fioTorsoFrame];
		torsoOriginY = FIO_SHOOT_UP_ORIGIN_Y[fioTorsoFrame];
	}

	fioLegsSprite.setOrigin(legOriginX, legOriginY);
	fioTorsoSprite.setOrigin(torsoOriginX, torsoOriginY);

	float legAnchorY = y + height;
	float torsoOffsetY = 12.0f;
	fioLegsSprite.setPosition(centerX, legAnchorY);
	fioTorsoSprite.setPosition(centerX, legAnchorY - torsoOffsetY * scale);
}

void PlayerSoldier::draw(sf::RenderWindow& window)
{
	if (!visible || (pilotHiddenWhileInsideVehicle && ridingVehicle))
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
	else if (currentCharacter == 1 && tarmaSpritesLoaded)
	{
		window.draw(tarmaLegsSprite);
		window.draw(tarmaTorsoSprite);
	}
	else if (currentCharacter == 3 && fioSpritesLoaded)
	{
		window.draw(fioLegsSprite);
		window.draw(fioTorsoSprite);
	}

	if ((currentCharacter == 0 && marcoSpritesLoaded) || (currentCharacter == 2 && eriSpritesLoaded) || (currentCharacter == 1 && tarmaSpritesLoaded) || (currentCharacter == 3 && fioSpritesLoaded))
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
