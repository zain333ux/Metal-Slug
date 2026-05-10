#pragma once

#include "ProjectileWeapon.h"
#include "Soldier.h"

class EntityManager;

class PlayerSoldier : public Soldier
{
private:
	ProjectileWeapon weapon;
	bool firing;
	float fireAnimationTimer;
	int currentCharacter;
	int lives[4];
	int grenades[4];
	int rockets[4];
	int hmgBullets[4];
	float meleeTimer;
	float grenadeTimer;
	float rocketTimer;
	float damageTimer;
	bool aimingUp;
	bool previousSwitchKey;
	bool previousMeleeKey;
	bool previousGrenadeKey;
	bool previousRocketKey;
	bool marcoSpritesLoaded;
	int marcoTorsoState;
	int marcoLegState;
	const sf::IntRect* marcoTorsoFrames;
	const sf::IntRect* marcoLegFrames;
	int marcoTorsoFrameCount;
	int marcoLegFrameCount;
	int marcoTorsoFrame;
	int marcoLegFrame;
	float marcoTorsoTimer;
	float marcoLegTimer;
	float marcoTorsoFrameDuration;
	float marcoLegFrameDuration;
	sf::Texture marcoIdleTorsoTexture;
	sf::Texture marcoIdleLegsTexture;
	sf::Texture marcoRunTorsoTexture;
	sf::Texture marcoRunLegsTexture;
	sf::Texture marcoFireTexture;
	sf::Texture marcoFaceUpTexture;
	sf::Texture marcoShootUpTexture;
	sf::Sprite marcoTorsoSprite;
	sf::Sprite marcoLegsSprite;

	bool eriSpritesLoaded;
	int eriTorsoState;
	int eriLegState;
	const sf::IntRect* eriTorsoFrames;
	const sf::IntRect* eriLegFrames;
	int eriTorsoFrameCount;
	int eriLegFrameCount;
	int eriTorsoFrame;
	int eriLegFrame;
	float eriTorsoTimer;
	float eriLegTimer;
	float eriTorsoFrameDuration;
	float eriLegFrameDuration;
	sf::Texture eriIdleTorsoTexture;
	sf::Texture eriIdleLegsTexture;
	sf::Texture eriRunTorsoTexture;
	sf::Texture eriRunLegsTexture;
	sf::Texture eriFireTexture;
	sf::Texture eriFaceUpTexture;
	sf::Texture eriShootUpTexture;
	sf::Sprite eriTorsoSprite;
	sf::Sprite eriLegsSprite;

	bool tarmaSpritesLoaded;
	int tarmaTorsoState;
	int tarmaLegState;
	const sf::IntRect* tarmaTorsoFrames;
	const sf::IntRect* tarmaLegFrames;
	int tarmaTorsoFrameCount;
	int tarmaLegFrameCount;
	int tarmaTorsoFrame;
	int tarmaLegFrame;
	float tarmaTorsoTimer;
	float tarmaLegTimer;
	float tarmaTorsoFrameDuration;
	float tarmaLegFrameDuration;
	sf::Texture tarmaIdleTorsoTexture;
	sf::Texture tarmaIdleLegsTexture;
	sf::Texture tarmaRunTorsoTexture;
	sf::Texture tarmaRunLegsTexture;
	sf::Texture tarmaFireTexture;
	sf::Texture tarmaFaceUpTexture;
	sf::Texture tarmaShootUpTexture;
	sf::Sprite tarmaTorsoSprite;
	sf::Sprite tarmaLegsSprite;

	bool fioSpritesLoaded;
	int fioTorsoState;
	int fioLegState;
	const sf::IntRect* fioTorsoFrames;
	const sf::IntRect* fioLegFrames;
	int fioTorsoFrameCount;
	int fioLegFrameCount;
	int fioTorsoFrame;
	int fioLegFrame;
	float fioTorsoTimer;
	float fioLegTimer;
	float fioTorsoFrameDuration;
	float fioLegFrameDuration;
	sf::Texture fioIdleTorsoTexture;
	sf::Texture fioIdleLegsTexture;
	sf::Texture fioRunTorsoTexture;
	sf::Texture fioRunLegsTexture;
	sf::Texture fioFireTexture;
	sf::Texture fioFaceUpTexture;
	sf::Texture fioShootUpTexture;
	sf::Sprite fioTorsoSprite;
	sf::Sprite fioLegsSprite;

	bool ridingVehicle;
	bool pilotHiddenWhileInsideVehicle;
	int transformationState;
	float transformationTimer;
	float transformationFrameTimer;
	int transformationFrame;
	sf::Texture zombieWalkTexture;
	sf::Texture zombieEmergeTexture;
	sf::Texture mummyIdleTexture;
	sf::Texture mummyWalkTexture;
	sf::Sprite transformationSprite;

	void handleInput();
	void updatePlayerAnimation(float deltaTime);
	void applyCharacterStats();
	bool loadMaskedTexture(sf::Texture& targetTexture, const char* fileName);
	void loadTransformationSprites();
	void updateTransformation(float deltaTime);
	void updateTransformationAnimation(float deltaTime);
	void updateTransformationSpritePosition();
	void loadMarcoSprites();
	void setMarcoTorsoAnimation(int newState, const sf::IntRect* frames, int frameCount, float frameDuration, sf::Texture& texture);
	void setMarcoLegAnimation(int newState, const sf::IntRect* frames, int frameCount, float frameDuration, sf::Texture& texture);
	void updateMarcoLayeredAnimation(float deltaTime);
	void updateMarcoSpritePositions();

	void loadEriSprites();
	void setEriTorsoAnimation(int newState, const sf::IntRect* frames, int frameCount, float frameDuration, sf::Texture& texture);
	void setEriLegAnimation(int newState, const sf::IntRect* frames, int frameCount, float frameDuration, sf::Texture& texture);
	void updateEriLayeredAnimation(float deltaTime);
	void updateEriSpritePositions();

	void loadTarmaSprites();
	void setTarmaTorsoAnimation(int newState, const sf::IntRect* frames, int frameCount, float frameDuration, sf::Texture& texture);
	void setTarmaLegAnimation(int newState, const sf::IntRect* frames, int frameCount, float frameDuration, sf::Texture& texture);
	void updateTarmaLayeredAnimation(float deltaTime);
	void updateTarmaSpritePositions();

	void loadFioSprites();
	void setFioTorsoAnimation(int newState, const sf::IntRect* frames, int frameCount, float frameDuration, sf::Texture& texture);
	void setFioLegAnimation(int newState, const sf::IntRect* frames, int frameCount, float frameDuration, sf::Texture& texture);
	void updateFioLayeredAnimation(float deltaTime);
	void updateFioSpritePositions();

public:
	PlayerSoldier();

	void takeDamage(int damage);
	void respawn();
	void update(float deltaTime);
	void draw(sf::RenderWindow& window) override;
	void handleWeaponInput(EntityManager& entityManager, float deltaTime);
	void switchCharacter();
	void refillDemoInventory();
	void addGrenades(int amount);
	void addRocketAmmo(int amount);
	void addHmgAmmo(int amount);
	void setRidingVehicle(bool riding);
	void setPilotHiddenForVehicle(bool hide);
	void zombify();
	void mummify();
	bool isZombieForm() const;
	bool isMummyForm() const;
	bool isTransformed() const;
	bool canUseVehicle() const;
	void handleVehicleDestruction();
	bool isRidingVehicle() const;
	float getVehicleFireCooldownMultiplier() const;
	float getVehicleDurabilityMultiplier() const;
	bool survivesVehicleDestruction() const;
	const char* getCharacterName() const;
	int getLives() const;
	int getGrenades() const;
	int getRockets() const;
	int getHmgBullets() const;
};
