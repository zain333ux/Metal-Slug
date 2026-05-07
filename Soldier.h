#pragma once

#include "DamageableEntity.h"

class Level;

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

	void updateState(float deltaTime);
	void updateDebugColor();
	void updateVisualPosition();
	void updateAnimation(float deltaTime);

public:
	Soldier();

	virtual void update(float deltaTime);
	virtual void draw(sf::RenderWindow& window);

	void moveLeft();
	void moveRight();
	void stopMoving();
	void jump();
	bool loadSpriteSheet(const char* fileName);
	void setSpriteFrame(int left, int top, int frameWidth, int frameHeight);
	void setSpriteScale(float scale);
	void setSpriteFacesLeft(bool facesLeft);
	void playAnimation(int row, int frameCount, float frameDuration);
	void playAnimation(int row, int startFrame, int frameCount, float frameDuration);
	void setMovementMaxX(float maxX);
	void setActiveLevel(Level* level);

	int getCurrentState() const;
	bool isFacingRight() const;
	bool isGrounded() const;
};
