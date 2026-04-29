#pragma once

#include "DamageableEntity.h"

class Soldier : public DamageableEntity
{
protected:
	float moveSpeed;
	float jumpSpeed;
	bool facingRight;
	bool grounded;
	int currentState;
	float stateTimer;
	sf::RectangleShape body;
	sf::Texture texture;
	bool usingSprite;
	float spriteScale;
	int frameWidth;
	int frameHeight;
	int animationRow;
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
	void playAnimation(int row, int frameCount, float frameDuration);

	int getCurrentState() const;
	bool isFacingRight() const;
	bool isGrounded() const;
};
