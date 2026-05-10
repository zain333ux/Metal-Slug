#pragma once

#include "DamageableEntity.h"

class Level;
class PlayerSoldier;
class Projectile;
class Entity;

class Enemy : public DamageableEntity
{
protected:
	int scoreValue;
	int contactDamage;
	float stopDistance;
	float contactDamageCooldown;
	float contactDamageTimer;
	float patrolLeft;
	float patrolRight;
	int patrolDirection;
	float detectionRange;
	float attackRange;
	float shootingRange;
	float pistolCooldown;
	float pistolTimer;
	bool pistolEquipped;
	bool queuedShot;
	PlayerSoldier* target;
	sf::Color fallbackColor;
	bool canMoveInAir;
	bool facingRight;
	bool grounded;
	float moveSpeed;
	float movementMaxX;
	Level* activeLevel;
	sf::RectangleShape body;
	sf::Texture texture;
	bool usingSprite;
	bool spriteFacesLeft;
	float spriteScale;
	int frameWidth;
	int frameHeight;
	int animationRow;
	int animationStartFrame;
	int animationFrameCount;
	int currentAnimationFrame;
	float animationTimer;
	float animationFrameDuration;
	bool deathProcessed;

	virtual void updateAI();
	void moveLeft();
	void moveRight();
	void stopMoving();
	bool loadSpriteSheet(const char* fileName);
	bool loadMaskedTexture(sf::Texture& targetTexture, const char* fileName);
	void setSpriteFrame(int left, int top, int newFrameWidth, int newFrameHeight);
	void setSpriteScale(float scale);
	void playAnimation(int row, int frameCount, float frameDuration);
	void playAnimation(int row, int startFrame, int frameCount, float frameDuration);
	void updateVisualPosition();
	void updateAnimation(float deltaTime);

public:
	Enemy();

	virtual void setSpawnPosition(float newX, float newY);
	void setTarget(PlayerSoldier* newTarget);
	void update(float deltaTime);
	void draw(sf::RenderWindow& window);
	void setMovementMaxX(float maxX);
	void setActiveLevel(Level* level);
	virtual Projectile* createProjectileIfReady();
	virtual Entity* createSpawnedEntityIfReady();
	virtual bool applyProjectileHit(Projectile& projectile);
	int getScoreValue() const;
	int getContactDamage() const;
	bool canDealContactDamage() const;
	void restartContactDamageCooldown();
	virtual const char* getEnemyName() const;
	bool isFacingRight() const;
	bool isGrounded() const;
	bool hasSpriteVisual() const;
	bool hasProcessedDeath() const;
	void markDeathProcessed();
};
