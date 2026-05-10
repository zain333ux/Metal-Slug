#include "Prisoner.h"

#include "Collectible.h"
#include "Constants.h"
#include "Level.h"

namespace
{
	void makeStrip(FrameRect* frames, int count, int width, int height)
	{
		int frameWidth = width / count;
		for (int i = 0; i < count; i += 1)
		{
			frames[i] = FrameRect{i * frameWidth, 0, frameWidth, height, 0, 0};
		}
	}
}

Prisoner::Prisoner(float startX, float groundY)
{
	state = PRISONER_STUCK;
	activeLevel = 0;
	spritesLoaded = false;
	crateQueued = false;
	crateCreated = false;
	facingRight = true;
	runSpeed = 190.0f;
	width = 64.0f;
	height = 82.0f;
	setPosition(startX, groundY - height);
	buildAnimations();
}

bool Prisoner::loadMaskedTexture(sf::Texture& target, const char* fileName)
{
	sf::Image image;
	if (!image.loadFromFile(fileName))
	{
		return false;
	}
	image.createMaskFromColor(sf::Color::White);
	image.createMaskFromColor(sf::Color(255, 0, 255));
	return target.loadFromImage(image);
}

void Prisoner::buildAnimations()
{
	if (!loadMaskedTexture(stuckTexture, "Sprites/Clean/prisoner_stuck.png") ||
		!loadMaskedTexture(breakTexture, "Sprites/Clean/prisoner_break.png") ||
		!loadMaskedTexture(dropTexture, "Sprites/Clean/prisoner_drop.png") ||
		!loadMaskedTexture(runTexture, "Sprites/Clean/prisoner_run.png"))
	{
		return;
	}

	static FrameRect stuckFrames[2];
	static FrameRect breakFrames[9];
	static FrameRect dropFrames[11];
	static FrameRect runFrames[8];
	static bool framesReady = false;
	if (!framesReady)
	{
		makeStrip(stuckFrames, 2, 84, 35);
		makeStrip(breakFrames, 9, 378, 35);
		makeStrip(dropFrames, 11, 484, 38);
		makeStrip(runFrames, 8, 336, 41);
		framesReady = true;
	}

	stuckAnim.setTexture(&stuckTexture);
	stuckAnim.setFrames(stuckFrames, 2);
	stuckAnim.setFrameTime(0.16f);
	stuckAnim.setLoop(true);
	stuckAnim.setScale(2.0f, 2.0f);

	breakAnim.setTexture(&breakTexture);
	breakAnim.setFrames(breakFrames, 9);
	breakAnim.setFrameTime(0.08f);
	breakAnim.setLoop(false);
	breakAnim.setScale(2.0f, 2.0f);

	dropAnim.setTexture(&dropTexture);
	dropAnim.setFrames(dropFrames, 11);
	dropAnim.setFrameTime(0.08f);
	dropAnim.setLoop(false);
	dropAnim.setScale(2.0f, 2.0f);

	runAnim.setTexture(&runTexture);
	runAnim.setFrames(runFrames, 8);
	runAnim.setFrameTime(0.07f);
	runAnim.setLoop(true);
	runAnim.setScale(2.0f, 2.0f);

	stuckAnim.reset();
	breakAnim.reset();
	dropAnim.reset();
	runAnim.reset();
	spritesLoaded = true;
}

void Prisoner::setState(PrisonerState newState)
{
	state = newState;
	currentAnimation().reset();
	if (state == PRISONER_RUNNING)
	{
		velocityX = runSpeed;
	}
	else if (state == PRISONER_DROPPING)
	{
		velocityY = -260.0f;
	}
}

SpriteAnimation& Prisoner::currentAnimation()
{
	if (state == PRISONER_BREAKING)
	{
		return breakAnim;
	}
	if (state == PRISONER_DROPPING)
	{
		return dropAnim;
	}
	if (state == PRISONER_RUNNING)
	{
		return runAnim;
	}
	return stuckAnim;
}

void Prisoner::update(float deltaTime)
{
	if (!active)
	{
		return;
	}

	SpriteAnimation& anim = currentAnimation();
	anim.setFacingRight(!facingRight);
	anim.update(deltaTime);

	if (state == PRISONER_BREAKING && anim.isFinished())
	{
		setState(PRISONER_DROPPING);
	}
	else if (state == PRISONER_DROPPING && anim.isFinished())
	{
		setState(PRISONER_RUNNING);
	}

	if (state == PRISONER_DROPPING)
	{
		velocityY += Constants::GRAVITY * deltaTime;
		Entity::update(deltaTime);
	}
	else if (state == PRISONER_RUNNING)
	{
		Entity::update(deltaTime);
		float maxX = activeLevel != 0 ? activeLevel->getWorldWidth() : Constants::WORLD_WIDTH_LEVEL_3;
		if (x > maxX + 180.0f)
		{
			deactivate();
		}
	}

	float groundY = static_cast<float>(Constants::GROUND_Y);
	if (activeLevel != 0)
	{
		groundY = activeLevel->getGroundYAt(x + width * 0.5f);
	}
	if (state == PRISONER_DROPPING)
	{
		if (y + height >= groundY)
		{
			y = groundY - height;
			velocityY = 0.0f;
			crateQueued = true;
		}
	}
	else
	{
		y = groundY - height;
	}
}

void Prisoner::draw(sf::RenderWindow& window)
{
	if (!visible || !active)
	{
		return;
	}

	if (spritesLoaded)
	{
		currentAnimation().setFacingRight(!facingRight);
		currentAnimation().drawAtAnchor(window, x + width * 0.5f, y + height);
	}
	else
	{
		sf::RectangleShape fallback;
		fallback.setPosition(x, y);
		fallback.setSize(sf::Vector2f(width, height));
		fallback.setFillColor(sf::Color(210, 180, 120));
		window.draw(fallback);
	}
}

void Prisoner::setActiveLevel(Level* level)
{
	activeLevel = level;
}

void Prisoner::freePrisoner()
{
	if (!canBeFreed())
	{
		return;
	}
	setState(PRISONER_BREAKING);
}

bool Prisoner::canBeFreed() const
{
	return state == PRISONER_STUCK;
}

Collectible* Prisoner::createCrateIfReady()
{
	if (!crateQueued || crateCreated)
	{
		return 0;
	}

	crateCreated = true;
	float crateX = x + width + 12.0f;
	float crateY = y + height - 72.0f;
	return new Collectible(COLLECTIBLE_SUPPLY_CRATE, crateX, crateY);
}
