#include "Soldier.h"

#include "Constants.h"
#include "Level.h"


using namespace std;
using namespace sf;

Soldier::Soldier()
{
	width = 52;
	height = 96;
	moveSpeed = Constants::PLAYER_MOVE_SPEED;
	jumpSpeed = Constants::PLAYER_JUMP_SPEED;
	facingRight = true;
	grounded = false;
	inWater = false;
	currentState = Constants::SOLDIER_STATE_IDLE;
	stateTimer = 0;
	movementMaxX = Constants::WORLD_WIDTH_LEVEL_3 + 2200;
	activeLevel = 0;
	usingSprite = false;
	spriteScale = 2;
	frameWidth = Constants::PLAYER_FRAME_SIZE;
	frameHeight = Constants::PLAYER_FRAME_SIZE;
	animationRow = 0;
	animationStartFrame = 0;
	animationFrameCount = 1;
	currentAnimationFrame = 0;
	animationTimer = 0;
	animationFrameDuration = 0.15f;
	maxHealth = 100;
	health = maxHealth;

	body.setSize(Vector2f(width, height));
	body.setFillColor(Color(70, 170, 240));
	body.setOutlineColor(Color::Black);
	body.setOutlineThickness(2);
	body.setPosition(x, y);
	spriteFacesLeft = false;
}

void Soldier::update(float deltaTime)
{
	bool wasInWater = false;
	if (activeLevel != 0)
	{
		wasInWater = activeLevel->isWaterInBounds(x + 4, x + width - 4, y + height * 0.45f, y + height + 10);
	}

	float gravity = Constants::GRAVITY;
	if (wasInWater)
	{
		gravity *= 0.28f;
		velocityX *= 0.82f;
	}
	velocityY += gravity * deltaTime;
	if (wasInWater && velocityY > 150)
	{
		velocityY = 150;
	}

	float previousBottom = y + height;
	Entity::update(deltaTime);

	float landingY = static_cast<float>(Constants::GROUND_Y);
	if (activeLevel != 0)
	{
		landingY = activeLevel->getLandingY(x, x + width, previousBottom, y + height);
	}

	inWater = activeLevel != 0 &&
		activeLevel->isWaterInBounds(x + 4, x + width - 4, y + height * 0.45f, y + height + 10);

	if (y + height >= landingY - 8 && velocityY >= 0)
	{
		y = landingY - height;
		velocityY = 0;
		grounded = true;
	}
	else if (y + height >= landingY)
	{
		y = landingY - height;
		if (velocityY > 0)
		{
			velocityY = 0;
		}
		grounded = true;
	}
	else
	{
		grounded = false;
	}

	if (activeLevel != 0 && y + height > activeLevel->getWorldHeight())
	{
		y = activeLevel->getWorldHeight() - height;
		velocityY = 0;
		grounded = true;
	}

	if (y < 0)
	{
		y = 0;
		if (velocityY < 0)
		{
			velocityY = 0;
		}
	}

	if (x < 0)
	{
		x = 0;
	}

	if (x + width > movementMaxX)
	{
		x = movementMaxX - width;
	}

	updateVisualPosition();
	updateState(deltaTime);
	updateAnimation(deltaTime);
	updateDebugColor();
}

void Soldier::updateAnimation(float deltaTime)
{
	if (!usingSprite || animationFrameCount <= 1)
	{
		return;
	}

	animationTimer += deltaTime;
	if (animationTimer >= animationFrameDuration)
	{
		animationTimer = 0;
		currentAnimationFrame += 1;

		if (currentAnimationFrame >= animationFrameCount)
		{
			currentAnimationFrame = 0;
		}

		setSpriteFrame((animationStartFrame + currentAnimationFrame) * frameWidth, animationRow * frameHeight, frameWidth, frameHeight);
	}
}

void Soldier::updateVisualPosition()
{
	body.setPosition(x, y);

	if (usingSprite)
	{
		bool drawFlipped = !facingRight;
		if (spriteFacesLeft)
		{
			drawFlipped = facingRight;
		}

		if (!drawFlipped)
		{
			sprite.setScale(spriteScale, spriteScale);
		}
		else
		{
			sprite.setScale(-spriteScale, spriteScale);
		}

		FloatRect spriteBounds = sprite.getGlobalBounds();
		float spriteX = x + width / 2 - spriteBounds.width / 2;
		if (!facingRight)
		{
			spriteX = x + width / 2 + spriteBounds.width / 2;
		}

		float spriteY = y + height - spriteBounds.height;
		sprite.setPosition(spriteX, spriteY);
	}
}

void Soldier::updateState(float deltaTime)
{
	stateTimer += deltaTime;

	int nextState = Constants::SOLDIER_STATE_IDLE;

	if (!grounded && velocityY < 0)
	{
		nextState = Constants::SOLDIER_STATE_JUMPING;
	}
	else if (!grounded && velocityY > 0)
	{
		nextState = Constants::SOLDIER_STATE_FALLING;
	}
	else if (velocityX != 0)
	{
		nextState = Constants::SOLDIER_STATE_RUNNING;
	}

	if (nextState != currentState)
	{
		currentState = nextState;
		stateTimer = 0;
	}
}

void Soldier::updateDebugColor()
{
	if (usingSprite)
	{
		return;
	}

	if (inWater)
	{
		body.setFillColor(Color(70, 150, 230));
	}
	else if (currentState == Constants::SOLDIER_STATE_RUNNING)
	{
		body.setFillColor(Color(80, 210, 120));
	}
	else if (currentState == Constants::SOLDIER_STATE_JUMPING)
	{
		body.setFillColor(Color(240, 210, 80));
	}
	else if (currentState == Constants::SOLDIER_STATE_FALLING)
	{
		body.setFillColor(Color(240, 140, 80));
	}
	else
	{
		body.setFillColor(Color(70, 170, 240));
	}
}

void Soldier::draw(RenderWindow& window)
{
	if (visible)
	{
		if (usingSprite)
		{
			window.draw(sprite);
		}
		else
		{
			window.draw(body);
		}

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
	}
}

void Soldier::moveLeft()
{
	velocityX = -moveSpeed;
	facingRight = false;
}

void Soldier::moveRight()
{
	velocityX = moveSpeed;
	facingRight = true;
}

void Soldier::stopMoving()
{
	velocityX = 0;
}

void Soldier::jump()
{
	if (grounded)
	{
		velocityY = -jumpSpeed;
		grounded = false;
	}
	else if (inWater)
	{
		velocityY = -jumpSpeed * 0.55f;
		inWater = false;
	}
}

bool Soldier::loadSpriteSheet(const char* fileName)
{
	if (!texture.loadFromFile(fileName))
	{
		usingSprite = false;
		return false;
	}

	sprite.setTexture(texture);
	sprite.setScale(spriteScale, spriteScale);
	usingSprite = true;
	updateVisualPosition();
	return true;
}

void Soldier::setSpriteFrame(int left, int top, int frameWidth, int frameHeight)
{
	this->frameWidth = frameWidth;
	this->frameHeight = frameHeight;
	sprite.setTextureRect(IntRect(left, top, frameWidth, frameHeight));
	updateVisualPosition();
}

void Soldier::setSpriteScale(float scale)
{
	if (scale <= 0)
	{
		return;
	}

	spriteScale = scale;
	sprite.setScale(spriteScale, spriteScale);
	updateVisualPosition();
}

void Soldier::playAnimation(int row, int frameCount, float frameDuration)
{
	playAnimation(row, 0, frameCount, frameDuration);
}

void Soldier::playAnimation(int row, int startFrame, int frameCount, float frameDuration)
{
	if (frameCount < 1)
	{
		frameCount = 1;
	}

	if (frameDuration <= 0)
	{
		frameDuration = 0.15f;
	}

	if (startFrame < 0)
	{
		startFrame = 0;
	}

	if (animationRow == row && animationStartFrame == startFrame && animationFrameCount == frameCount)
	{
		animationFrameDuration = frameDuration;
		return;
	}

	animationRow = row;
	animationStartFrame = startFrame;
	animationFrameCount = frameCount;
	currentAnimationFrame = 0;
	animationTimer = 0;
	animationFrameDuration = frameDuration;
	setSpriteFrame(animationStartFrame * frameWidth, animationRow * frameHeight, frameWidth, frameHeight);
}

void Soldier::setMovementMaxX(float maxX)
{
	if (maxX > width)
	{
		movementMaxX = maxX;
	}
}

void Soldier::setActiveLevel(Level* level)
{
	activeLevel = level;
}

bool Soldier::isFacingRight() const
{
	return facingRight;
}

bool Soldier::isGrounded() const
{
	return grounded;
}

<<<<<<< HEAD
bool Soldier::isInWater() const
{
	return inWater;
}
=======
>>>>>>> 6d77c54b1816a0bae93ea8c1ad344f5e135692cd
