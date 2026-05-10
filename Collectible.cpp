#include "Collectible.h"

#include "Constants.h"
#include "Level.h"
#include "PlayerSoldier.h"

#include <cstdlib>

using namespace std;
using namespace sf;

Collectible::Collectible(CollectibleKind newKind, float newX, float newY)
{
	kind = newKind;
	activeLevel = 0;
	velocityX = 0;
	velocityY = 0;
	spriteLoaded = false;
	animationFrames = 0;
	animationFrameCount = 1;
	animationFrameIndex = 0;
	animationTimer = 0;
	animationFrameDuration = 0.12f;
	lifeTime = 12;

	width = 32;
	height = 32;
	setPosition(newX, newY);
	fallbackBody.setOutlineColor(Color::Black);
	fallbackBody.setOutlineThickness(1);

	if (kind == COLLECTIBLE_FRUIT)
	{
		loadMaskedTexture("Sprites/Clean/fruit_item.png");
	}
	else if (kind == COLLECTIBLE_TURKEY)
	{
		loadMaskedTexture("Sprites/Clean/turkey_item.png");
		static const IntRect TURKEY_FRAMES[] =
		{
			IntRect(1, 2, 26, 32), IntRect(32, 4, 26, 30), IntRect(63, 3, 26, 31),
			IntRect(95, 3, 26, 31), IntRect(126, 2, 26, 32), IntRect(157, 2, 26, 32),
			IntRect(188, 3, 26, 31), IntRect(218, 3, 26, 31), IntRect(249, 3, 27, 31),
			IntRect(281, 3, 28, 31), IntRect(314, 4, 28, 30)
		};
		animationFrames = TURKEY_FRAMES;
		animationFrameCount = 11;
		animationFrameDuration = 0.08f;
	}
	else if (kind == COLLECTIBLE_ROCKET_ITEM)
	{
		loadMaskedTexture("Sprites/Clean/rocket_item.png");
	}
	else if (kind == COLLECTIBLE_HMG_ITEM)
	{
		loadMaskedTexture("Sprites/Clean/HMG_item.png");
	}
	else
	{
		loadMaskedTexture("Sprites/Clean/crate.png");
		static const IntRect CRATE_FRAMES[] =
		{
			IntRect(3, 6, 32, 28),  
			IntRect(40, 5, 32, 29),  
			IntRect(77, 4, 32, 30),  
			IntRect(114, 2, 32, 32),  
			IntRect(152, 3, 32, 31),   
			IntRect(190, 5, 32, 29),   
			IntRect(228, 6, 32, 28),   
		};
		animationFrames = CRATE_FRAMES;
		animationFrameCount = 6;
		animationFrameDuration = 0.12f;
		width = 88;
		height = 72;
	}

	if (!spriteLoaded)
	{
		fallbackBody.setSize(Vector2f(width, height));
		if (kind == COLLECTIBLE_FRUIT)
		{
			fallbackBody.setFillColor(Color(245, 90, 80));
		}
		else if (kind == COLLECTIBLE_TURKEY)
		{
			fallbackBody.setFillColor(Color(210, 140, 80));
		}
		else if (kind == COLLECTIBLE_ROCKET_ITEM || kind == COLLECTIBLE_HMG_ITEM)
		{
			fallbackBody.setFillColor(Color(170, 170, 210));
		}
		else
		{
			fallbackBody.setFillColor(Color(170, 110, 60));
		}
	}

	if (spriteLoaded && animationFrames != 0)
	{
		itemSprite.setTextureRect(animationFrames[0]);
	}
	updateVisualPosition();
}

void Collectible::update(float deltaTime)
{
	if (!active)
	{
		return;
	}
	lifeTime -= deltaTime;
	if (lifeTime <= 0)
	{
		deactivate();
		return;
	}

	float previousBottom = y + height;
	velocityY += Constants::GRAVITY * deltaTime;
	Entity::update(deltaTime);

	float landingY = static_cast<float>(Constants::GROUND_Y);
	if (activeLevel != 0)
	{
		landingY = activeLevel->getLandingY(x, x + width, previousBottom, y + height);
	}
	if (y + height >= landingY)
	{
		y = landingY - height;
		velocityY = 0;
	}

	if (animationFrames != 0 && animationFrameCount > 1)
	{
		animationTimer += deltaTime;
		if (animationTimer >= animationFrameDuration)
		{
			animationTimer = 0;
			animationFrameIndex += 1;
			if (animationFrameIndex >= animationFrameCount)
			{
				animationFrameIndex = 0;
			}
			itemSprite.setTextureRect(animationFrames[animationFrameIndex]);
		}
	}

	updateVisualPosition();
}

void Collectible::apply(PlayerSoldier& player)
{
	if (kind == COLLECTIBLE_FRUIT)
	{
		player.heal(20);
	}
	else if (kind == COLLECTIBLE_TURKEY)
	{
		player.heal(30);
	}
	else if (kind == COLLECTIBLE_ROCKET_ITEM)
	{
		player.addRocketAmmo(4);
	}
	else if (kind == COLLECTIBLE_HMG_ITEM)
	{
		player.addHmgAmmo(100);
	}
	else //crate
	{
		player.addGrenades(5);
		player.heal(30);
		if (rand() % 2 == 0)
		{
			player.addRocketAmmo(4);
		}
		else
		{
			player.addHmgAmmo(100);
		}
	}

	deactivate();
}

void Collectible::draw(RenderWindow& window)
{
	if (!visible || !active)
	{
		return;
	}

	if (spriteLoaded)
	{
		window.draw(itemSprite);
	}
	else
	{
		fallbackBody.setPosition(x, y);
		window.draw(fallbackBody);
	}
}

void Collectible::setActiveLevel(Level* level)
{
	activeLevel = level;
}

bool Collectible::loadMaskedTexture(const char* fileName)
{
	Image image;
	if (!image.loadFromFile(fileName))
	{
		spriteLoaded = false;
		return false;
	}
	image.createMaskFromColor(Color::White);
	image.createMaskFromColor(Color(255, 0, 255));
	if (!texture.loadFromImage(image))
	{
		spriteLoaded = false;
		return false;
	}
	itemSprite.setTexture(texture, true);
	spriteLoaded = true;
	return true;
}

void Collectible::updateVisualPosition()
{
	if (!spriteLoaded)
	{
		return;
	}

	float spriteScale = 2;
	itemSprite.setScale(spriteScale, spriteScale);
	FloatRect bounds = itemSprite.getLocalBounds();
	if (animationFrames != 0 && animationFrameCount > 1)
	{
		itemSprite.setPosition(x + width * 0.5f - bounds.width * spriteScale * 0.5f,
			y + height - bounds.height * spriteScale);
		fallbackBody.setSize(Vector2f(width, height));
		return;
	}

	if (bounds.height > 0)
	{
		width = bounds.width * spriteScale;
		height = bounds.height * spriteScale;
	}
	itemSprite.setPosition(x, y);
	fallbackBody.setSize(Vector2f(width, height));
}