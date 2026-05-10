#pragma once

#include "DamageableEntity.h"

class Level;
class TransformationState;
class Weapon;

class Soldier : public DamageableEntity
{
protected:
	float moveSpeed;
	float jumpSpeed;
	bool facingRight;
	bool grounded;
	int currentState;
	float stateTimer;
	float movementMaxX;
	Level* activeLevel;
	bool inWater;
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
	TransformationState* transformation;
	Weapon* activeWeapon;

	void updateState(float deltaTime);
	void updateDebugColor();
	void updateVisualPosition();
	void updateAnimation(float deltaTime);

public:
	Soldier();
	virtual ~Soldier() = 0;

	virtual void update(float deltaTime);
	virtual void draw(sf::RenderWindow& window) override;

	void moveLeft();
	void moveRight();
	void stopMoving();
	void jump();
	bool loadSpriteSheet(const char* fileName);
	void setSpriteFrame(int left, int top, int frameWidth, int frameHeight);
	void setSpriteScale(float scale);
	void playAnimation(int row, int frameCount, float frameDuration);
	void playAnimation(int row, int startFrame, int frameCount, float frameDuration);
	void setMovementMaxX(float maxX);
	void setActiveLevel(Level* level);
	void setTransformation(TransformationState* newTransformation);
	void setWeapon(Weapon* weapon);
	Weapon* getWeapon() const;

	bool isFacingRight() const;
	bool isGrounded() const;
};
