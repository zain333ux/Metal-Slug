#include "PlayerSoldier.h"

#include "Constants.h"
#include "DeveloperMode.h"
#include "EntityManager.h"
#include "GrenadeProjectile.h"
#include "MeleeHitbox.h"
#include "RocketProjectile.h"
#include "TransformationState.h"

#include <SFML/Window/Keyboard.hpp>


using namespace std;
using namespace sf;

static const IntRect MARCO_IDLE_TORSO_FRAMES[] =
{
	IntRect(5, 4, 29, 29), IntRect(39, 4, 29, 29), IntRect(73, 4, 30, 28), IntRect(108, 4, 29, 27)
};

static const IntRect MARCO_IDLE_LEG_FRAMES[] =
{
	IntRect(3, 7, 21, 16)
};

static const IntRect MARCO_RUN_TORSO_FRAMES[] =
{
	IntRect(7, 3, 32, 28), IntRect(44, 3, 30, 29), IntRect(79, 3, 28, 28), IntRect(112, 3, 27, 27),
	IntRect(144, 3, 29, 28), IntRect(178, 3, 30, 28), IntRect(213, 3, 32, 29), IntRect(251, 3, 32, 29),
	IntRect(288, 3, 32, 28), IntRect(325, 3, 31, 27), IntRect(361, 3, 31, 28), IntRect(397, 3, 31, 28)
};

static const IntRect MARCO_RUN_LEG_FRAMES[] =
{
	IntRect(3, 5, 21, 20), IntRect(29, 5, 28, 20), IntRect(62, 5, 31, 16), IntRect(98, 5, 19, 20),
	IntRect(122, 5, 15, 20), IntRect(142, 5, 16, 20), IntRect(163, 5, 21, 20), IntRect(189, 5, 26, 20),
	IntRect(220, 5, 31, 16), IntRect(256, 5, 20, 20), IntRect(281, 5, 15, 20), IntRect(301, 5, 18, 20)
};

static const IntRect MARCO_FIRE_FRAMES[] =
{
	IntRect(3, 3, 50, 23), IntRect(58, 3, 51, 23), IntRect(114, 3, 52, 23)
};

static const IntRect MARCO_FACE_UP_FRAMES[] =
{
	IntRect(0, 0, 34, 32), IntRect(34, 0, 34, 32), IntRect(68, 0, 34, 32), IntRect(102, 0, 34, 32)
};

static const IntRect MARCO_SHOOT_UP_FRAMES[] =
{
	IntRect(0, 0, 27, 72), IntRect(27, 0, 27, 72), IntRect(54, 0, 27, 72), IntRect(81, 0, 27, 72)
};

static const float MARCO_IDLE_TORSO_ORIGIN_X[] = { 10, 10, 10, 10 };
static const float MARCO_IDLE_TORSO_ORIGIN_Y[] = { 25, 25, 25, 25 };
static const float MARCO_RUN_TORSO_ORIGIN_X[] = { 12, 11, 10, 9.5f, 10.5f, 11, 12, 12, 12, 11.5f, 11.5f, 11.5f };
static const float MARCO_RUN_TORSO_ORIGIN_Y[] = { 27, 28, 27, 26, 27, 27, 28, 28, 27, 26, 27, 27 };
static const float MARCO_FIRE_ORIGIN_X[] = { 11, 11, 11 };
static const float MARCO_FIRE_ORIGIN_Y[] = { 24, 24, 24 };
static const float MARCO_FACE_UP_ORIGIN_X[] = { 13, 13, 13, 13 };
static const float MARCO_FACE_UP_ORIGIN_Y[] = { 30, 30, 30, 30 };
static const float MARCO_SHOOT_UP_ORIGIN_X[] = { 13.5f, 13.5f, 13.5f, 13.5f };
static const float MARCO_SHOOT_UP_ORIGIN_Y[] = { 70, 70, 70, 70 };
static const float MARCO_IDLE_LEG_ORIGIN_X[] = { 10 };
static const float MARCO_IDLE_LEG_ORIGIN_Y[] = { 15 };
static const float MARCO_RUN_LEG_ORIGIN_X[] = { 10.5f, 14, 15.5f, 9.5f, 7.5f, 8, 10.5f, 13, 15.5f, 10, 7.5f, 9 };
static const float MARCO_RUN_LEG_ORIGIN_Y[] = { 19, 19, 15, 19, 19, 19, 19, 19, 15, 19, 19, 19 };

// Tarma Animation Constants
static const IntRect TARMA_IDLE_TORSO_FRAMES[] =
{
	IntRect(4, 4, 32, 26), IntRect(40, 4, 32, 26), IntRect(76, 4, 32, 26), IntRect(112, 4, 32, 26)
};

static const IntRect TARMA_IDLE_LEG_FRAMES[] =
{
	IntRect(3, 2, 22, 18)
};

static const IntRect TARMA_RUN_TORSO_FRAMES[] =
{
	IntRect(1,   3, 32, 28), IntRect(43,  2, 30, 29), IntRect(82,  2, 28, 28), IntRect(119, 2, 27, 27),
	IntRect(154, 2, 29, 28), IntRect(192, 2, 30, 28), IntRect(232, 3, 32, 28), IntRect(273, 2, 32, 29),
	IntRect(313, 2, 32, 28), IntRect(353, 3, 31, 27), IntRect(392, 3, 31, 28), IntRect(430, 3, 31, 28)
};

static const IntRect TARMA_RUN_LEG_FRAMES[] =
{
	IntRect(4,   3, 21, 20), IntRect(40,  3, 28, 20), IntRect(78,  3, 31, 16), IntRect(120, 3, 19, 20),
	IntRect(157, 3, 15, 20), IntRect(194, 3, 16, 20), IntRect(232, 3, 21, 20), IntRect(268, 3, 26, 20),
	IntRect(310, 3, 31, 16), IntRect(353, 3, 20, 20), IntRect(394, 3, 15, 20), IntRect(430, 3, 18, 20)
};

static const IntRect TARMA_FIRE_FRAMES[] =
{
	IntRect(4, 3, 50, 23), IntRect(60, 3, 51, 23), IntRect(116, 3, 52, 23)
};

static const IntRect TARMA_FACE_UP_FRAMES[] =
{
	IntRect(0, 0, 33, 31), IntRect(33, 0, 33, 31), IntRect(66, 0, 33, 31), IntRect(99, 0, 33, 31)
};

static const IntRect TARMA_SHOOT_UP_FRAMES[] =
{
	IntRect(0, 0, 29, 69), IntRect(29, 0, 29, 69), IntRect(58, 0, 29, 69), IntRect(87, 0, 29, 69)
};

static const float TARMA_IDLE_TORSO_ORIGIN_X[] = { 7, 6, 6, 7 };
static const float TARMA_IDLE_TORSO_ORIGIN_Y[] = { 22.5f, 22.5f, 22.5f, 22.5f };
static const float TARMA_RUN_TORSO_ORIGIN_X[] = { 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11 };
static const float TARMA_RUN_TORSO_ORIGIN_Y[] = { 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25 };
static const float TARMA_FIRE_ORIGIN_X[] = { 9, 9, 9 };
static const float TARMA_FIRE_ORIGIN_Y[] = { 22.5f, 22.5f, 22.5f };
static const float TARMA_FACE_UP_ORIGIN_X[] = { 11, 12, 13, 14 };
static const float TARMA_FACE_UP_ORIGIN_Y[] = { 29, 29, 29, 29 };
static const float TARMA_SHOOT_UP_ORIGIN_X[] = { 14.5f, 14.5f, 14.5f, 14.5f };
static const float TARMA_SHOOT_UP_ORIGIN_Y[] = { 67, 67, 67, 67 };
static const float TARMA_IDLE_LEG_ORIGIN_X[] = { 10 };
static const float TARMA_IDLE_LEG_ORIGIN_Y[] = { 15 };
static const float TARMA_RUN_LEG_ORIGIN_X[] = { 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13 };
static const float TARMA_RUN_LEG_ORIGIN_Y[] = { 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17 };

// Eri Animation Constants
static const IntRect ERI_IDLE_TORSO_FRAMES[] =
{
	IntRect(5, 4, 29, 29), IntRect(39, 4, 29, 29), IntRect(73, 4, 30, 28), IntRect(108, 4, 29, 27)
};

static const IntRect ERI_IDLE_LEG_FRAMES[] =
{
	IntRect(2, 7, 21, 21)
};

static const IntRect ERI_RUN_TORSO_FRAMES[] =
{
	IntRect(5, 3, 32, 28), IntRect(40, 3, 30, 29), IntRect(75, 3, 28, 28), IntRect(110, 3, 27, 27),
	IntRect(145, 3, 29, 28), IntRect(180, 3, 30, 28), IntRect(215, 3, 32, 29), IntRect(250, 3, 32, 29),
	IntRect(285, 3, 32, 28), IntRect(320, 3, 31, 27), IntRect(355, 3, 31, 28), IntRect(390, 3, 31, 28)
};

static const IntRect ERI_RUN_LEG_FRAMES[] =
{
	IntRect(4,   3, 24, 25), IntRect(32,  2, 18, 26), IntRect(55,  2, 20, 26), IntRect(80,  2, 22, 26),
	IntRect(109, 2, 25, 25), IntRect(142, 3, 27, 23), IntRect(174, 2, 25, 26), IntRect(203, 2, 17, 26),
	IntRect(226, 3, 18, 25), IntRect(250, 3, 21, 25), IntRect(277, 4, 26, 23), IntRect(308, 4, 30, 25)
};

static const IntRect ERI_FIRE_FRAMES[] =
{
	IntRect(3, 3, 50, 23), IntRect(61, 3, 51, 23), IntRect(119, 3, 52, 23)
};

static const IntRect ERI_FACE_UP_FRAMES[] =
{
	IntRect(2, 0, 36, 32), IntRect(36, 0, 36, 32), IntRect(72, 0, 36, 32), IntRect(108, 0, 38, 32)
};

static const IntRect ERI_SHOOT_UP_FRAMES[] =
{
	IntRect(0, 0, 27, 72), IntRect(28, 0, 27, 72), IntRect(56, 0, 27, 72), IntRect(84, 0, 27, 72)
};

static const float ERI_IDLE_TORSO_ORIGIN_X[] = { 10, 11, 13, 12 };
static const float ERI_IDLE_TORSO_ORIGIN_Y[] = { 20, 20, 20, 20 };
static const float ERI_RUN_TORSO_ORIGIN_X[] = { 12, 11, 10, 9.5f, 10.5f, 11, 12, 12, 12, 11.5f, 11.5f, 11.5f };
static const float ERI_RUN_TORSO_ORIGIN_Y[] = { 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25 };
static const float ERI_FIRE_ORIGIN_X[] = { 11, 11, 11 };
static const float ERI_FIRE_ORIGIN_Y[] = { 21, 21, 21 };
static const float ERI_FACE_UP_ORIGIN_X[] = { 14, 18, 19, 20 };
static const float ERI_FACE_UP_ORIGIN_Y[] = { 27, 27, 27, 27 };
static const float ERI_SHOOT_UP_ORIGIN_X[] = { 13.5f, 13.5f, 13.5f, 13.5f };
static const float ERI_SHOOT_UP_ORIGIN_Y[] = { 66, 66, 66, 66 };
static const float ERI_IDLE_LEG_ORIGIN_X[] = { 10 };
static const float ERI_IDLE_LEG_ORIGIN_Y[] = { 15 };
static const float ERI_RUN_LEG_ORIGIN_X[] = { 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13 };
static const float ERI_RUN_LEG_ORIGIN_Y[] = { 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22 };

// Fio — frame rects from Sprites/Clean/Fio_*.png (tight alpha bounds); pivots at foot mid-x / bottom row
static const IntRect FIO_IDLE_TORSO_FRAMES[] =
{
	IntRect(3, 4, 28, 26), IntRect(36, 4, 28, 26), IntRect(69, 4, 28, 26), IntRect(102, 4, 28, 26)
};

static const IntRect FIO_IDLE_LEG_FRAMES[] =
{
	IntRect(6, 4, 20, 24)
};

static const IntRect FIO_RUN_TORSO_FRAMES[] =
{
	IntRect(3, 3, 27, 26), IntRect(35, 4, 27, 26), IntRect(67, 5, 27, 26), IntRect(99, 4, 27, 26),
	IntRect(131, 3, 27, 26), IntRect(163, 3, 28, 26), IntRect(196, 4, 28, 26), IntRect(229, 5, 28, 26),
	IntRect(262, 5, 29, 26), IntRect(296, 6, 28, 26), IntRect(329, 4, 26, 27), IntRect(360, 4, 27, 27)
};

static const IntRect FIO_RUN_LEG_FRAMES[] =
{
	IntRect(3, 5, 22, 26), IntRect(30, 5, 25, 25), IntRect(60, 6, 27, 23), IntRect(92, 5, 25, 26),
	IntRect(122, 5, 17, 26), IntRect(144, 6, 18, 25), IntRect(167, 6, 21, 25), IntRect(193, 6, 26, 23),
	IntRect(224, 6, 30, 25), IntRect(259, 6, 24, 25), IntRect(288, 5, 18, 26), IntRect(311, 5, 20, 26)
};

static const IntRect FIO_FIRE_FRAMES[] =
{
	IntRect(3, 4, 48, 19), IntRect(56, 4, 58, 19), IntRect(114, 3, 49, 20)
};

static const IntRect FIO_FACE_UP_FRAMES[] =
{
	IntRect(3, 4, 24, 23), IntRect(32, 4, 24, 23), IntRect(61, 4, 25, 23), IntRect(91, 4, 25, 23)
};

static const IntRect FIO_SHOOT_UP_FRAMES[] =
{
	IntRect(8, 10, 22, 59), IntRect(32, 7, 22, 62), IntRect(51, 5, 27, 64), IntRect(75, 36, 28, 33)
};

static const float FIO_IDLE_TORSO_ORIGIN_X[] = { 11.5f, 11.5f, 11.5f, 11.5f };
static const float FIO_IDLE_TORSO_ORIGIN_Y[] = { 26, 26, 26, 26 };
static const float FIO_RUN_TORSO_ORIGIN_X[] = { 13, 13, 13, 13, 13, 13.5f, 13.5f, 13.5f, 14, 13.5f, 12.5f, 13 };
static const float FIO_RUN_TORSO_ORIGIN_Y[] = { 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 27, 27 };
static const float FIO_FIRE_ORIGIN_X[] = { 10, 11, 10 };
static const float FIO_FIRE_ORIGIN_Y[] = { 23, 23, 23 };
static const float FIO_FACE_UP_ORIGIN_X[] = { 8.5f, 8.5f, 8.5f, 8.5f };
static const float FIO_FACE_UP_ORIGIN_Y[] = { 26, 26, 26, 26 };
static const float FIO_SHOOT_UP_ORIGIN_X[] = { 9, 9, 10, 11 };
static const float FIO_SHOOT_UP_ORIGIN_Y[] = { 64, 65, 66, 66 };
static const float FIO_IDLE_LEG_ORIGIN_X[] = { 9.5f };
static const float FIO_IDLE_LEG_ORIGIN_Y[] = { 24 };
static const float FIO_RUN_LEG_ORIGIN_X[] = { 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 };
static const float FIO_RUN_LEG_ORIGIN_Y[] = { 26, 25, 23, 26, 26, 25, 25, 23, 25, 25, 26, 26 };

static const int PLAYER_FORM_NORMAL = 0;
static const int PLAYER_FORM_ZOMBIE_EMERGE = 1;
static const int PLAYER_FORM_ZOMBIE = 2;
static const int PLAYER_FORM_MUMMY = 3;

static const IntRect PLAYER_MUMMY_IDLE_FRAMES[] =
{
	IntRect(0, 0, 30, 45),   // frame 0
	IntRect(30, 0, 30, 45),   // frame 1
	IntRect(60, 0, 30, 45),   // frame 2
	IntRect(90, 0, 30, 45),   // frame 3
};

PlayerSoldier::PlayerSoldier()
{
	setWeapon(&weapon);
	firing = false;
	fireAnimationTimer = 0;
	currentCharacter = 0;
	for (int i = 0; i < 4; i += 1)
	{
		lives[i] = 2;
		grenades[i] = 10;
		rockets[i] = 0;
		hmgBullets[i] = 0;
	}
	meleeTimer = 0;
	grenadeTimer = 0;
	rocketTimer = 0;
	damageTimer = 0;
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
	marcoTorsoTimer = 0;
	marcoLegTimer = 0;
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
	eriTorsoTimer = 0;
	eriLegTimer = 0;
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
	tarmaTorsoTimer = 0;
	tarmaLegTimer = 0;
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
	fioTorsoTimer = 0;
	fioLegTimer = 0;
	fioTorsoFrameDuration = 0.12f;
	fioLegFrameDuration = 0.09f;

	ridingVehicle = false;
	pilotHiddenWhileInsideVehicle = false;
	transformationState = PLAYER_FORM_NORMAL;
	transformationTimer = 0;
	transformationFrameTimer = 0;
	transformationFrame = 0;
	setPosition(120, 500);
	setSpriteScale(2.2f);
	loadMarcoSprites();
	loadEriSprites();
	loadTarmaSprites();
	loadFioSprites();
	loadTransformationSprites();
	applyCharacterStats();
}

bool PlayerSoldier::loadMaskedTexture(Texture& targetTexture, const char* fileName)
{
	Image image;
	if (!image.loadFromFile(fileName))
	{
		return false;
	}

	image.createMaskFromColor(Color::White);
	image.createMaskFromColor(Color(255, 0, 255));
	return targetTexture.loadFromImage(image);
}

void PlayerSoldier::loadTransformationSprites()
{
	loadMaskedTexture(zombieWalkTexture, "Sprites/Clean/Zombie_walk.png");
	loadMaskedTexture(zombieEmergeTexture, "Sprites/Clean/Zombie_emerge.png");
	loadMaskedTexture(mummyIdleTexture, "Sprites/Clean/MummyWarrior_idle.png");
	loadMaskedTexture(mummyWalkTexture, "Sprites/Clean/MummyWarrior_walk.png");
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

	if (damage <= 0 || isDead() || ridingVehicle || damageTimer > 0)
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
	setPosition(120, 500);
	setVelocity(0, 0);
	facingRight = true;
	firing = false;
	fireAnimationTimer = 0;
	aimingUp = false;
	damageTimer = 0;
	transformationState = PLAYER_FORM_NORMAL;
	setTransformation(new NormalTransformationState());
	transformationTimer = 0;
	transformationFrameTimer = 0;
	transformationFrame = 0;
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
	if (isTransformed())
	{
		aimingUp = false;
		moveSpeed = Constants::PLAYER_MOVE_SPEED * 0.5f;
		if (transformationState == PLAYER_FORM_ZOMBIE_EMERGE)
		{
			return;
		}
		if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
		{
			moveLeft();
		}
		if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
		{
			moveRight();
		}
		return;
	}

	aimingUp = Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up);

	bool switchKey = Keyboard::isKeyPressed(Keyboard::Z);
	if (switchKey && !previousSwitchKey)
	{
		switchCharacter();
	}
	previousSwitchKey = switchKey;

	if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
	{
		moveLeft();
	}

	if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
	{
		moveRight();
	}

	if (inWater)
	{
		if (Keyboard::isKeyPressed(Keyboard::Space) ||
			Keyboard::isKeyPressed(Keyboard::W) ||
			Keyboard::isKeyPressed(Keyboard::Up))
		{
			velocityY = -jumpSpeed * 0.45f;
		}
		else if (Keyboard::isKeyPressed(Keyboard::S) ||
			Keyboard::isKeyPressed(Keyboard::Down))
		{
			velocityY += jumpSpeed * 0.015f;
		}
	}
	else if (Keyboard::isKeyPressed(Keyboard::Space))
	{
		jump();
	}
}

void PlayerSoldier::update(float deltaTime)
{
	if (meleeTimer > 0)
	{
		meleeTimer -= deltaTime;
	}
	if (grenadeTimer > 0)
	{
		grenadeTimer -= deltaTime;
	}
	if (rocketTimer > 0)
	{
		rocketTimer -= deltaTime;
	}
	if (damageTimer > 0)
	{
		damageTimer -= deltaTime;
	}

	if (DeveloperMode::isEnabled())
	{
		health = maxHealth;
		refillDemoInventory();
	}

	updateTransformation(deltaTime);

	if (ridingVehicle)
	{
		stopMoving();
		setVelocity(0, 0);
		return;
	}

	handleInput();
	Soldier::update(deltaTime);
	updatePlayerAnimation(deltaTime);

	if (isDead())
	{
		body.setFillColor(Color(90, 90, 90));
	}
	else if (damageTimer > 0)
	{
		body.setFillColor(Color(255, 90, 90));
	}
}

void PlayerSoldier::updatePlayerAnimation(float deltaTime)
{
	if (isTransformed())
	{
		updateTransformationAnimation(deltaTime);
		return;
	}

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

	if (fireAnimationTimer > 0)
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

	if (transformationState == PLAYER_FORM_ZOMBIE_EMERGE)
	{
		return;
	}

	if (isZombieForm())
	{
		if (Keyboard::isKeyPressed(Keyboard::LControl) || Keyboard::isKeyPressed(Keyboard::J))
		{
			if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
			{
				facingRight = false;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
			{
				facingRight = true;
			}
			float bulletX = facingRight ? x + width : x;
			weapon.fire(entityManager, bulletX, y + 38, facingRight, false);
		}
		return;
	}

	if (isMummyForm())
	{
		bool meleeKey = Keyboard::isKeyPressed(Keyboard::K);
		if (meleeKey && !previousMeleeKey && meleeTimer <= 0)
		{
			float hitX = facingRight ? x + width : x;
			MeleeHitbox* hitbox = new MeleeHitbox(hitX, y + 18, facingRight, !grounded);
			entityManager.addEntity(hitbox);
			meleeTimer = Constants::MELEE_COOLDOWN;
		}
		previousMeleeKey = meleeKey;
		previousGrenadeKey = Keyboard::isKeyPressed(Keyboard::G);
		previousRocketKey = Keyboard::isKeyPressed(Keyboard::R);
		return;
	}

	if (Keyboard::isKeyPressed(Keyboard::LControl) || Keyboard::isKeyPressed(Keyboard::J))
	{
		bool canFireWeapon = weapon.canFire();
		if (canFireWeapon)
		{
			firing = true;
			fireAnimationTimer = 0.24f;
		}

		if (!aimingUp && (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A)))
		{
			facingRight = false;
		}
		else if (!aimingUp && (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)))
		{
			facingRight = true;
		}

		float bulletX = x;
		if (aimingUp)
		{
			bulletX = x + width / 2 - 8;
		}
		else if (facingRight)
		{
			bulletX = x + width;
		}

		float bulletY = aimingUp ? y + 6 : y + 38;

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

	bool meleeKey = Keyboard::isKeyPressed(Keyboard::K);
	if (meleeKey && !previousMeleeKey && meleeTimer <= 0)
	{
		float hitX = facingRight ? x + width : x;
		MeleeHitbox* hitbox = new MeleeHitbox(hitX, y + 18, facingRight, !grounded);
		entityManager.addEntity(hitbox);
		meleeTimer = Constants::MELEE_COOLDOWN;
		fireAnimationTimer = 0.18f;
	}
	previousMeleeKey = meleeKey;

	bool grenadeKey = Keyboard::isKeyPressed(Keyboard::G);
	if (grenadeKey && !previousGrenadeKey && grenadeTimer <= 0)
	{
		if (grenades[currentCharacter] > 0 || DeveloperMode::isEnabled())
		{
			float grenadeX = facingRight ? x + width : x;
			float grenadeY = y + 30;
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

void PlayerSoldier::addGrenades(int amount)
{
	if (amount <= 0)
	{
		return;
	}

	grenades[currentCharacter] += amount;
	if (grenades[currentCharacter] > 99)
	{
		grenades[currentCharacter] = 99;
	}
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
		fireAnimationTimer = 0;
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

	return 1;
}

float PlayerSoldier::getVehicleDurabilityMultiplier() const
{
	if (currentCharacter == 1)
	{
		return 1.2f;
	}

	return 1;
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

void PlayerSoldier::zombify()
{
	if (isDead() || ridingVehicle || transformationState == PLAYER_FORM_ZOMBIE || transformationState == PLAYER_FORM_ZOMBIE_EMERGE)
	{
		return;
	}

	transformationState = PLAYER_FORM_ZOMBIE_EMERGE;
	setTransformation(new ZombieTransformationState());
	transformationTimer = 10;
	transformationFrame = 0;
	transformationFrameTimer = 0;
	stopMoving();
	aimingUp = false;
	firing = false;
	fireAnimationTimer = 0;
	transformationSprite.setTexture(zombieEmergeTexture, true);
	transformationSprite.setTextureRect(IntRect(0, 0, 49, 41));
	updateTransformationSpritePosition();
}

void PlayerSoldier::mummify()
{
	if (isDead() || ridingVehicle || transformationState == PLAYER_FORM_MUMMY)
	{
		return;
	}

	transformationState = PLAYER_FORM_MUMMY;
	setTransformation(new MummyTransformationState());
	transformationTimer = 10;
	transformationFrame = 0;
	transformationFrameTimer = 0;
	stopMoving();
	aimingUp = false;
	firing = false;
	fireAnimationTimer = 0;
	transformationSprite.setTexture(mummyIdleTexture, true);
	transformationSprite.setTextureRect(IntRect(0, 0, 40, 45));
	updateTransformationSpritePosition();
}

bool PlayerSoldier::isZombieForm() const
{
	return transformationState == PLAYER_FORM_ZOMBIE || transformationState == PLAYER_FORM_ZOMBIE_EMERGE;
}

bool PlayerSoldier::isMummyForm() const
{
	return transformationState == PLAYER_FORM_MUMMY;
}

bool PlayerSoldier::isTransformed() const
{
	return transformationState != PLAYER_FORM_NORMAL;
}

bool PlayerSoldier::canUseVehicle() const
{
	return !isTransformed();
}

void PlayerSoldier::updateTransformation(float deltaTime)
{
	if (!isTransformed())
	{
		return;
	}

	moveSpeed = Constants::PLAYER_MOVE_SPEED * 0.5f;

	if (transformationState == PLAYER_FORM_ZOMBIE_EMERGE)
	{
		return;
	}

	transformationTimer -= deltaTime;
	if (transformationTimer <= 0)
	{
		transformationState = PLAYER_FORM_NORMAL;
		setTransformation(new NormalTransformationState());
		transformationTimer = 0;
		transformationFrame = 0;
		transformationFrameTimer = 0;
		applyCharacterStats();
	}
}

void PlayerSoldier::updateTransformationAnimation(float deltaTime)
{
	int frameWidthLocal = 45;
	int frameHeightLocal = 41;
	int frameCount = 24;
	float frameDuration = 0.08f;

	if (transformationState == PLAYER_FORM_ZOMBIE_EMERGE)
	{
		frameWidthLocal = 49;
		frameHeightLocal = 41;
		frameCount = 79;
		frameDuration = 0.045f;
		transformationSprite.setTexture(zombieEmergeTexture, true);
	}
	else if (transformationState == PLAYER_FORM_MUMMY)
	{
		if (currentState == Constants::SOLDIER_STATE_RUNNING)
		{
			frameWidthLocal = 36;
			frameHeightLocal = 45;
			frameCount = 18;
			frameDuration = 0.075f;
			transformationSprite.setTexture(mummyWalkTexture, true);
		}
		else
		{
			frameCount = 3;
			frameDuration = 0.16f;
			transformationSprite.setTexture(mummyIdleTexture, true);
		}
	}
	else
	{
		transformationSprite.setTexture(zombieWalkTexture, true);
		if (currentState != Constants::SOLDIER_STATE_RUNNING)
		{
			transformationFrame = 0;
			transformationFrameTimer = 0;
			transformationSprite.setTextureRect(IntRect(0, 0, frameWidthLocal, frameHeightLocal));
			updateTransformationSpritePosition();
			return;
		}
	}

	transformationFrameTimer += deltaTime;
	if (transformationFrameTimer >= frameDuration)
	{
		transformationFrameTimer = 0;
		transformationFrame += 1;
		if (transformationFrame >= frameCount)
		{
			if (transformationState == PLAYER_FORM_ZOMBIE_EMERGE)
			{
				transformationState = PLAYER_FORM_ZOMBIE;
				transformationFrame = 0;
				transformationFrameTimer = 0;
				transformationSprite.setTexture(zombieWalkTexture, true);
				transformationSprite.setTextureRect(IntRect(0, 0, 45, 41));
				updateTransformationSpritePosition();
				return;
			}
			if (transformationState == PLAYER_FORM_MUMMY && currentState != Constants::SOLDIER_STATE_RUNNING)
			{
				transformationFrame = 1;
			}
			else
			{
				transformationFrame = 0;
			}
		}
	}

	if (transformationState == PLAYER_FORM_MUMMY && currentState != Constants::SOLDIER_STATE_RUNNING)
	{
		transformationSprite.setTextureRect(PLAYER_MUMMY_IDLE_FRAMES[transformationFrame]);
	}
	else
	{
		transformationSprite.setTextureRect(IntRect(transformationFrame * frameWidthLocal, 0, frameWidthLocal, frameHeightLocal));
	}
	updateTransformationSpritePosition();
}

void PlayerSoldier::updateTransformationSpritePosition()
{
	float scale = 2.35f;
	bool drawFlipped = !facingRight;
	if (transformationState == PLAYER_FORM_MUMMY)
	{
		drawFlipped = facingRight;
	}
	if (drawFlipped)
	{
		transformationSprite.setScale(-scale, scale);
	}
	else
	{
		transformationSprite.setScale(scale, scale);
	}

	FloatRect bounds = transformationSprite.getGlobalBounds();
	float drawX = x + width / 2 - bounds.width / 2;
	if (drawFlipped)
	{
		drawX = x + width / 2 + bounds.width / 2;
	}
	float drawY = y + height - bounds.height;
	transformationSprite.setPosition(drawX, drawY);
}

void PlayerSoldier::setMarcoTorsoAnimation(int newState, const IntRect* frames, int frameCount, float frameDuration, Texture& texture)
{
	if (marcoTorsoState == newState)
	{
		return;
	}

	marcoTorsoState = newState;
	marcoTorsoFrames = frames;
	marcoTorsoFrameCount = frameCount;
	marcoTorsoFrame = 0;
	marcoTorsoTimer = 0;
	marcoTorsoFrameDuration = frameDuration;
	marcoTorsoSprite.setTexture(texture, true);
	marcoTorsoSprite.setTextureRect(marcoTorsoFrames[0]);
	updateMarcoSpritePositions();
}

void PlayerSoldier::setMarcoLegAnimation(int newState, const IntRect* frames, int frameCount, float frameDuration, Texture& texture)
{
	if (marcoLegState == newState)
	{
		return;
	}

	marcoLegState = newState;
	marcoLegFrames = frames;
	marcoLegFrameCount = frameCount;
	marcoLegFrame = 0;
	marcoLegTimer = 0;
	marcoLegFrameDuration = frameDuration;
	marcoLegsSprite.setTexture(texture, true);
	marcoLegsSprite.setTextureRect(marcoLegFrames[0]);
	updateMarcoSpritePositions();
}

void PlayerSoldier::updateMarcoLayeredAnimation(float deltaTime)
{
	bool running = currentState == Constants::SOLDIER_STATE_RUNNING;
	bool airborne = currentState == Constants::SOLDIER_STATE_JUMPING || currentState == Constants::SOLDIER_STATE_FALLING;

	if (fireAnimationTimer > 0)
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

	if (aimingUp && fireAnimationTimer > 0)
	{
		setMarcoTorsoAnimation(4, MARCO_SHOOT_UP_FRAMES, 4, 0.08f, marcoShootUpTexture);
	}
	else if (aimingUp)
	{
		setMarcoTorsoAnimation(3, MARCO_FACE_UP_FRAMES, 4, 0.12f, marcoFaceUpTexture);
	}
	else if (fireAnimationTimer > 0)
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
			marcoTorsoTimer = 0;
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
		marcoLegTimer = 0;
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

	float centerX = x + width / 2;
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
	float torsoOffsetY = 12;
	marcoLegsSprite.setPosition(centerX, legAnchorY);
	marcoTorsoSprite.setPosition(centerX, legAnchorY - torsoOffsetY * scale);
}

void PlayerSoldier::setEriTorsoAnimation(int newState, const IntRect* frames, int frameCount, float frameDuration, Texture& texture)
{
	if (eriTorsoState == newState)
	{
		return;
	}

	eriTorsoState = newState;
	eriTorsoFrames = frames;
	eriTorsoFrameCount = frameCount;
	eriTorsoFrame = 0;
	eriTorsoTimer = 0;
	eriTorsoFrameDuration = frameDuration;
	eriTorsoSprite.setTexture(texture, true);
	eriTorsoSprite.setTextureRect(eriTorsoFrames[0]);
	updateEriSpritePositions();
}

void PlayerSoldier::setEriLegAnimation(int newState, const IntRect* frames, int frameCount, float frameDuration, Texture& texture)
{
	if (eriLegState == newState)
	{
		return;
	}

	eriLegState = newState;
	eriLegFrames = frames;
	eriLegFrameCount = frameCount;
	eriLegFrame = 0;
	eriLegTimer = 0;
	eriLegFrameDuration = frameDuration;
	eriLegsSprite.setTexture(texture, true);
	eriLegsSprite.setTextureRect(eriLegFrames[0]);
	updateEriSpritePositions();
}

void PlayerSoldier::updateEriLayeredAnimation(float deltaTime)
{
	bool running = currentState == Constants::SOLDIER_STATE_RUNNING;
	bool airborne = currentState == Constants::SOLDIER_STATE_JUMPING || currentState == Constants::SOLDIER_STATE_FALLING;

	if (fireAnimationTimer > 0)
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

	if (aimingUp && fireAnimationTimer > 0)
	{
		setEriTorsoAnimation(4, ERI_SHOOT_UP_FRAMES, 4, 0.08f, eriShootUpTexture);
	}
	else if (aimingUp)
	{
		setEriTorsoAnimation(3, ERI_FACE_UP_FRAMES, 4, 0.12f, eriFaceUpTexture);
	}
	else if (fireAnimationTimer > 0)
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
			eriTorsoTimer = 0;
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
		eriLegTimer = 0;
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

	float centerX = x + width / 2;
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
	float torsoOffsetY = 12;
	eriLegsSprite.setPosition(centerX, legAnchorY);
	eriTorsoSprite.setPosition(centerX, legAnchorY - torsoOffsetY * scale);
}

void PlayerSoldier::setTarmaTorsoAnimation(int newState, const IntRect* frames, int frameCount, float frameDuration, Texture& texture)
{
	if (tarmaTorsoState == newState)
	{
		return;
	}

	tarmaTorsoState = newState;
	tarmaTorsoFrames = frames;
	tarmaTorsoFrameCount = frameCount;
	tarmaTorsoFrame = 0;
	tarmaTorsoTimer = 0;
	tarmaTorsoFrameDuration = frameDuration;
	tarmaTorsoSprite.setTexture(texture, true);
	tarmaTorsoSprite.setTextureRect(tarmaTorsoFrames[0]);
	updateTarmaSpritePositions();
}

void PlayerSoldier::setTarmaLegAnimation(int newState, const IntRect* frames, int frameCount, float frameDuration, Texture& texture)
{
	if (tarmaLegState == newState)
	{
		return;
	}

	tarmaLegState = newState;
	tarmaLegFrames = frames;
	tarmaLegFrameCount = frameCount;
	tarmaLegFrame = 0;
	tarmaLegTimer = 0;
	tarmaLegFrameDuration = frameDuration;
	tarmaLegsSprite.setTexture(texture, true);
	tarmaLegsSprite.setTextureRect(tarmaLegFrames[0]);
	updateTarmaSpritePositions();
}

void PlayerSoldier::updateTarmaLayeredAnimation(float deltaTime)
{
	bool running = currentState == Constants::SOLDIER_STATE_RUNNING;
	bool airborne = currentState == Constants::SOLDIER_STATE_JUMPING || currentState == Constants::SOLDIER_STATE_FALLING;

	if (fireAnimationTimer > 0)
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

	if (aimingUp && fireAnimationTimer > 0)
	{
		setTarmaTorsoAnimation(4, TARMA_SHOOT_UP_FRAMES, 4, 0.08f, tarmaShootUpTexture);
	}
	else if (aimingUp)
	{
		setTarmaTorsoAnimation(3, TARMA_FACE_UP_FRAMES, 4, 0.12f, tarmaFaceUpTexture);
	}
	else if (fireAnimationTimer > 0)
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
			tarmaTorsoTimer = 0;
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
		tarmaLegTimer = 0;
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

	float centerX = x + width / 2;
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
	float torsoOffsetY = 12;
	tarmaLegsSprite.setPosition(centerX, legAnchorY);
	tarmaTorsoSprite.setPosition(centerX, legAnchorY - torsoOffsetY * scale);
}

void PlayerSoldier::setFioTorsoAnimation(int newState, const IntRect* frames, int frameCount, float frameDuration, Texture& texture)
{
	if (fioTorsoState == newState)
	{
		return;
	}

	fioTorsoState = newState;
	fioTorsoFrames = frames;
	fioTorsoFrameCount = frameCount;
	fioTorsoFrame = 0;
	fioTorsoTimer = 0;
	fioTorsoFrameDuration = frameDuration;
	fioTorsoSprite.setTexture(texture, true);
	fioTorsoSprite.setTextureRect(fioTorsoFrames[0]);
	updateFioSpritePositions();
}

void PlayerSoldier::setFioLegAnimation(int newState, const IntRect* frames, int frameCount, float frameDuration, Texture& texture)
{
	if (fioLegState == newState)
	{
		return;
	}

	fioLegState = newState;
	fioLegFrames = frames;
	fioLegFrameCount = frameCount;
	fioLegFrame = 0;
	fioLegTimer = 0;
	fioLegFrameDuration = frameDuration;
	fioLegsSprite.setTexture(texture, true);
	fioLegsSprite.setTextureRect(fioLegFrames[0]);
	updateFioSpritePositions();
}

void PlayerSoldier::updateFioLayeredAnimation(float deltaTime)
{
	bool running = currentState == Constants::SOLDIER_STATE_RUNNING;
	bool airborne = currentState == Constants::SOLDIER_STATE_JUMPING || currentState == Constants::SOLDIER_STATE_FALLING;

	if (fireAnimationTimer > 0)
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

	if (aimingUp && fireAnimationTimer > 0)
	{
		setFioTorsoAnimation(4, FIO_SHOOT_UP_FRAMES, 4, 0.08f, fioShootUpTexture);
	}
	else if (aimingUp)
	{
		setFioTorsoAnimation(3, FIO_FACE_UP_FRAMES, 4, 0.12f, fioFaceUpTexture);
	}
	else if (fireAnimationTimer > 0)
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
			fioTorsoTimer = 0;
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
		fioLegTimer = 0;
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

	float centerX = x + width / 2;
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
	float torsoOffsetY = 12;
	fioLegsSprite.setPosition(centerX, legAnchorY);
	fioTorsoSprite.setPosition(centerX, legAnchorY - torsoOffsetY * scale);
}

void PlayerSoldier::draw(RenderWindow& window)
{
	if (!visible || (pilotHiddenWhileInsideVehicle && ridingVehicle))
	{
		return;
	}

	Color playerTint = damageTimer > 0 ? Color(255, 120, 120) : Color::White;

	if (isTransformed())
	{
		transformationSprite.setColor(playerTint);
		window.draw(transformationSprite);
	}
	else if (currentCharacter == 0 && marcoSpritesLoaded)
	{
		marcoLegsSprite.setColor(playerTint);
		marcoTorsoSprite.setColor(playerTint);
		window.draw(marcoLegsSprite);
		window.draw(marcoTorsoSprite);
	}
	else if (currentCharacter == 2 && eriSpritesLoaded)
	{
		eriLegsSprite.setColor(playerTint);
		eriTorsoSprite.setColor(playerTint);
		window.draw(eriLegsSprite);
		window.draw(eriTorsoSprite);
	}
	else if (currentCharacter == 1 && tarmaSpritesLoaded)
	{
		tarmaLegsSprite.setColor(playerTint);
		tarmaTorsoSprite.setColor(playerTint);
		window.draw(tarmaLegsSprite);
		window.draw(tarmaTorsoSprite);
	}
	else if (currentCharacter == 3 && fioSpritesLoaded)
	{
		fioLegsSprite.setColor(playerTint);
		fioTorsoSprite.setColor(playerTint);
		window.draw(fioLegsSprite);
		window.draw(fioTorsoSprite);
	}

	if (isTransformed() || (currentCharacter == 0 && marcoSpritesLoaded) || (currentCharacter == 2 && eriSpritesLoaded) || (currentCharacter == 1 && tarmaSpritesLoaded) || (currentCharacter == 3 && fioSpritesLoaded))
	{
		if (maxHealth > 0)
		{
			float healthRatio = static_cast<float>(health) / static_cast<float>(maxHealth);
			if (healthRatio < 0)
			{
				healthRatio = 0;
			}

			RectangleShape healthBack;
			healthBack.setPosition(x, y - 12);
			healthBack.setSize(Vector2f(width, 6));
			healthBack.setFillColor(Color(80, 20, 20));
			window.draw(healthBack);

			RectangleShape healthFront;
			healthFront.setPosition(x, y - 12);
			healthFront.setSize(Vector2f(width * healthRatio, 6));
			healthFront.setFillColor(Color(60, 220, 90));
			window.draw(healthFront);
		}
		return;
	}

	Soldier::draw(window);
}
