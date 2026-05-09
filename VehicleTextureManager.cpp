#include "VehicleTextureManager.h"

#include <cstdio>

VehicleTextureManager::VehicleTextureManager()
{
	int n = static_cast<int>(VehicleTextureId::COUNT_);
	for (int i = 0; i < n; i += 1)
	{
		loaded[i] = false;
		warned[i] = false;
	}
}

VehicleTextureManager& VehicleTextureManager::instance()
{
	static VehicleTextureManager manager;
	return manager;
}

bool VehicleTextureManager::tryLoad(int id, const char* path)
{
	if (id < 0 || id >= static_cast<int>(VehicleTextureId::COUNT_))
	{
		return false;
	}

	if (loaded[id])
	{
		return true;
	}

	if (textures[id].loadFromFile(path))
	{
		loaded[id] = true;
		return true;
	}

	if (!warned[id])
	{
		std::printf("[VehicleTextureManager] Failed to load: %s\n", path);
		warned[id] = true;
	}
	return false;
}

const sf::Texture* VehicleTextureManager::getTexture(VehicleTextureId id)
{
	int i = static_cast<int>(id);
	switch (id)
	{
	case VehicleTextureId::SlugFlyerIdle:
		tryLoad(i, "Sprites/Clean/SlugFlyerIdle.png");
		break;
	case VehicleTextureId::SlugFlyerLand:
		tryLoad(i, "Sprites/Clean/SlugFlyerLand.png");
		break;
	case VehicleTextureId::SlugFlyerRocket:
		tryLoad(i, "Sprites/Clean/SlugFlyerRocket.png");
		break;
	case VehicleTextureId::SlugFlyerShoot:
		tryLoad(i, "Sprites/Clean/SlugFlyerShoot.png");
		break;
	case VehicleTextureId::SlugMarinerIdle:
		tryLoad(i, "Sprites/Clean/SlugSumbarineIdle.png");
		break;
	case VehicleTextureId::EnemySubMove:
		tryLoad(i, "Sprites/Clean/EnemySubmarineMove.png");
		break;
	case VehicleTextureId::EnemySubTurn:
		tryLoad(i, "Sprites/Clean/EnemySubmarineTurn.png");
		break;
	case VehicleTextureId::EnemySubDeath:
		tryLoad(i, "Sprites/Clean/EnemySubmarineFallingApart.png");
		break;
	case VehicleTextureId::EnemySubBullet:
		tryLoad(i, "Sprites/Clean/EnemySubmarineBullet.png");
		break;
	case VehicleTextureId::BradleyIdle:
		tryLoad(i, "Sprites/Clean/M-15ABradleyIdle.png");
		break;
	case VehicleTextureId::BradleyRocketUp:
		tryLoad(i, "Sprites/Clean/M-15ABradleyRocketUp.png");
		break;
	case VehicleTextureId::BradleyBullet:
		tryLoad(i, "Sprites/Clean/M-15ABradleyBullet.png");
		break;
	case VehicleTextureId::TaraIdle:
		tryLoad(i, "Sprites/Clean/FlyingTaraIdle.png");
		break;
	case VehicleTextureId::TaraMoveDown:
		tryLoad(i, "Sprites/Clean/FlyingTaraMoveDown.png");
		break;
	case VehicleTextureId::TaraMoveUp:
		tryLoad(i, "Sprites/Clean/FlyingTaraMoveUp.png");
		break;
	case VehicleTextureId::TaraRocket:
		tryLoad(i, "Sprites/Clean/FlyingTaraRocket.png");
		break;
	default:
		break;
	}

	return loaded[i] ? &textures[i] : nullptr;
}
