#include "Collectible.h"

#include "Constants.h"
#include "Level.h"
#include "PlayerSoldier.h"

#include <cstdlib>

Collectible::Collectible(CollectibleKind newKind, float newX, float newY)
{
	kind = newKind;
	activeLevel = 0;
	velocityX = 0.0f;
	velocityY = 0.0f;
	spriteLoaded = false;
	animationFrames = 0;
	animationFrameCount = 1;
	animationFrameIndex = 0;
	animationTimer = 0.0f;
	animationFrameDuration = 0.12f;
	lifeTime = 12.0f;

	width = 32.0f;
	height = 32.0f;
	setPosition(newX, newY);
	fallbackBody.setOutlineColor(sf::Color::Black);
	fallbackBody.setOutlineThickness(1.0f);

	if (kind == COLLECTIBLE_FRUIT)
	{
		loadMaskedTexture("Sprites/Clean/fruit_item.png");
	}
	else if (kind == COLLECTIBLE_TURKEY)
	{
		loadMaskedTexture("Sprites/Clean/turkey_item.png");
		static const sf::IntRect TURKEY_FRAMES[] =
		{
			sf::IntRect(1, 2, 26, 32), sf::IntRect(32, 4, 26, 30), sf::IntRect(63, 3, 26, 31),
			sf::IntRect(95, 3, 26, 31), sf::IntRect(126, 2, 26, 32), sf::IntRect(157, 2, 26, 32),
			sf::IntRect(188, 3, 26, 31), sf::IntRect(218, 3, 26, 31), sf::IntRect(249, 3, 27, 31),
			sf::IntRect(281, 3, 28, 31), sf::IntRect(314, 4, 28, 30)
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
		static const sf::IntRect CRATE_FRAMES[] =
		{
			sf::IntRect(3, 6, 32, 28),  
			sf::IntRect(40, 5, 32, 29),  
			sf::IntRect(77, 4, 32, 30),  
			sf::IntRect(114, 2, 32, 32),  
			sf::IntRect(152, 3, 32, 31),   
			sf::IntRect(190, 5, 32, 29),   
			sf::IntRect(228, 6, 32, 28),   
		};
		animationFrames = CRATE_FRAMES;
		animationFrameCount = 6;
		animationFrameDuration = 0.12f;
		width = 88.0f;
		height = 72.0f;
	}

	if (!spriteLoaded)
	{
		fallbackBody.setSize(sf::Vector2f(width, height));
		if (kind == COLLECTIBLE_FRUIT)
		{
			fallbackBody.setFillColor(sf::Color(245, 90, 80));
		}
		else if (kind == COLLECTIBLE_TURKEY)
		{
			fallbackBody.setFillColor(sf::Color(210, 140, 80));
		}
		else if (kind == COLLECTIBLE_ROCKET_ITEM || kind == COLLECTIBLE_HMG_ITEM)
		{
			fallbackBody.setFillColor(sf::Color(170, 170, 210));
		}
		else
		{
			fallbackBody.setFillColor(sf::Color(170, 110, 60));
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
	if (lifeTime <= 0.0f)
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
		velocityY = 0.0f;
	}

	if (animationFrames != 0 && animationFrameCount > 1)
	{
		animationTimer += deltaTime;
		if (animationTimer >= animationFrameDuration)
		{
			animationTimer = 0.0f;
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
		if (std::rand() % 2 == 0)
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

void Collectible::draw(sf::RenderWindow& window)
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

CollectibleKind Collectible::getKind() const
{
	return kind;
}

bool Collectible::loadMaskedTexture(const char* fileName)
{
	sf::Image image;
	if (!image.loadFromFile(fileName))
	{
		spriteLoaded = false;
		return false;
	}
	image.createMaskFromColor(sf::Color::White);
	image.createMaskFromColor(sf::Color(255, 0, 255));
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

	float spriteScale = 2.0f;
	itemSprite.setScale(spriteScale, spriteScale);
	sf::FloatRect bounds = itemSprite.getLocalBounds();
	if (animationFrames != 0 && animationFrameCount > 1)
	{
		itemSprite.setPosition(x + width * 0.5f - bounds.width * spriteScale * 0.5f,
			y + height - bounds.height * spriteScale);
		fallbackBody.setSize(sf::Vector2f(width, height));
		return;
	}

	if (bounds.height > 0.0f)
	{
		width = bounds.width * spriteScale;
		height = bounds.height * spriteScale;
	}
	itemSprite.setPosition(x, y);
	fallbackBody.setSize(sf::Vector2f(width, height));
}
