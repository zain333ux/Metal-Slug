#pragma once

#include "DamageableEntity.h"
#include "ProjectileWeapon.h"

class EntityManager;
class Level;
class PlayerSoldier;

class Vehicle : public DamageableEntity
{
protected:
	enum VehicleAnimationKind
	{
		VEHICLE_ANIM_IDLE,
		VEHICLE_ANIM_MOVE,
		VEHICLE_ANIM_SHOOT,
		VEHICLE_ANIM_HIT
	};

	struct VehicleAnimation
	{
		sf::Texture texture;
		int totalWidth;
		int frameHeight;
		int frameCount;
		int frameLefts[20];
		int frameWidths[20];
		int bottomPadding;
		float frameDuration;
		bool loop;
		bool loaded;
	};

	bool occupied;
	bool facingRight;
	bool grounded;
	bool inWater;
	int baseMaxHealth;
	float baseFireCooldown;
	float moveSpeed;
	float movementMaxX;
	float damageFlashTimer;
	Level* activeLevel;
	BasicProjectileWeapon weapon;
	VehicleAnimation normalIdleAnimation;
	VehicleAnimation normalMoveAnimation;
	VehicleAnimation normalShootAnimation;
	VehicleAnimation normalHitAnimation;
	VehicleAnimation uphillIdleAnimation;
	VehicleAnimation uphillMoveAnimation;
	VehicleAnimation uphillShootAnimation;
	VehicleAnimation uphillHitAnimation;
	VehicleAnimation cannonAnimation;
	sf::Sprite vehicleSprite;
	sf::Sprite cannonSprite;
	bool usingSprite;
	bool usingCannonSprite;
	bool usingUphillAnimation;
	float spriteScale;
	float cannonScale;
	int currentAnimationKind;
	int currentAnimationFrame;
	int currentFrameWidth;
	int currentFrameHeight;
	int currentFrameBottomPadding;
	int currentCannonFrame;
	int currentCannonFrameWidth;
	float animationTimer;
	float shootAnimationTimer;
	float hitAnimationTimer;
	float cannonTimer;
	float cannonFrameTimer;
	sf::RectangleShape body;
	sf::RectangleShape cabin;
	sf::RectangleShape turret;
	sf::CircleShape rearWheel;
	sf::CircleShape frontWheel;

	void resetAnimation(VehicleAnimation& animation);
	virtual bool loadVehicleSprites();
	bool loadAnimation(VehicleAnimation& animation, const sf::Image& sheetImage, sf::Color backgroundColor, int x, int y, int totalWidth, int totalHeight, int frameCount, float frameDuration, bool loop);
	void setFrameBounds(VehicleAnimation& animation, int frameIndex, int frameLeft, int frameWidth);
	void setAnimationBottomPadding(VehicleAnimation& animation, int bottomPadding);
	VehicleAnimation& getAnimation(int animationKind, bool uphill);
	void setAnimationFrame(VehicleAnimation& animation, int frameIndex);
	void setCannonFrame(int frameIndex);
	virtual void handleMovementInput();
	virtual void updateAnimation(float deltaTime);
	void updateCannonAnimation(float deltaTime);
	void updateVisualPosition();
	void updateColors();
	void applyDriverBonuses(const PlayerSoldier* driver);

protected:
	void drawHealthBar(sf::RenderWindow& window);

public:
	Vehicle(float startX, float startY);
	virtual ~Vehicle() = 0;

	virtual void update(float deltaTime);
	virtual void draw(sf::RenderWindow& window);
	virtual void handleWeaponInput(EntityManager& entityManager, float deltaTime);
	virtual void takeDamage(int damage) override;

	bool canMount(const PlayerSoldier& player) const;
	virtual void mount(PlayerSoldier& player);
	virtual void dismount(PlayerSoldier& player);
	virtual void ejectPlayer(PlayerSoldier& player);

	void setActiveLevel(Level* level);
	void setMovementMaxX(float maxX);

	bool isOccupied() const;
	bool isFacingRight() const;
	int getMaxHealth() const;
	float getSeatX() const;
	float getSeatY() const;
};

class GroundVehicle : public Vehicle
{
public:
	GroundVehicle(float startX, float startY);
	virtual ~GroundVehicle() = 0;
};

class AerialVehicle : public Vehicle
{
public:
	AerialVehicle(float startX, float startY);
	virtual ~AerialVehicle() = 0;
};

class AquaticVehicle : public Vehicle
{
public:
	AquaticVehicle(float startX, float startY);
	virtual ~AquaticVehicle() = 0;
};

class MetalSlug : public GroundVehicle
{
public:
	MetalSlug(float startX, float startY);
};
