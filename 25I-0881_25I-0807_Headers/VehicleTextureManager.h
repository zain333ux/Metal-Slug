#pragma once

#include <SFML/Graphics.hpp>

enum class VehicleTextureId
{
	SlugFlyerIdle,
	SlugFlyerLand,
	SlugFlyerRocket,
	SlugFlyerShoot,
	SlugMarinerIdle,
	EnemySubMove,
	EnemySubTurn,
	EnemySubDeath,
	EnemySubBullet,
	BradleyIdle,
	BradleyRocketUp,
	BradleyBullet,
	TaraIdle,
	TaraMoveDown,
	TaraMoveUp,
	TaraRocket,
	COUNT_
};

/// Loads vehicle / vehicle-projectile textures once (Texture::loadFromFile only here or in vehicle ctor one-time static).
class VehicleTextureManager
{
private:
	sf::Texture textures[static_cast<int>(VehicleTextureId::COUNT_)];
	bool loaded[static_cast<int>(VehicleTextureId::COUNT_)];
	bool warned[static_cast<int>(VehicleTextureId::COUNT_)];

	bool tryLoad(int id, const char* path);

public:
	VehicleTextureManager();

	static VehicleTextureManager& instance();

	const sf::Texture* getTexture(VehicleTextureId id);
};
