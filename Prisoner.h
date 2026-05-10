#pragma once

#include "Entity.h"
#include "SpriteAnimation.h"

class Level;
class Collectible;

class Prisoner : public Entity
{
private:
	enum PrisonerState
	{
		PRISONER_STUCK,
		PRISONER_BREAKING,
		PRISONER_DROPPING,
		PRISONER_RUNNING
	};

	PrisonerState state;
	Level* activeLevel;
	sf::Texture stuckTexture;
	sf::Texture breakTexture;
	sf::Texture dropTexture;
	sf::Texture runTexture;
	SpriteAnimation stuckAnim;
	SpriteAnimation breakAnim;
	SpriteAnimation dropAnim;
	SpriteAnimation runAnim;
	bool spritesLoaded;
	bool crateQueued;
	bool crateCreated;
	bool facingRight;
	float runSpeed;

	bool loadMaskedTexture(sf::Texture& target, const char* fileName);
	void buildAnimations();
	void setState(PrisonerState newState);
	SpriteAnimation& currentAnimation();

public:
	Prisoner(float startX, float groundY);

	void update(float deltaTime) override;
	void draw(sf::RenderWindow& window) override;
	void setActiveLevel(Level* level);
	void freePrisoner();
	bool canBeFreed() const;
	Collectible* createCrateIfReady();
};
