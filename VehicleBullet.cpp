#include "VehicleBullet.h"

#include "Constants.h"

bool VehicleBullet::textureLoaded = false;
sf::Texture VehicleBullet::bulletTexture;

VehicleBullet::VehicleBullet(float startX, float startY, bool newFacingRight)
{
	damage = 18;
	lifeTime = 2.0f;
	width = 40.0f;
	height = 35.0f;
	facingRight = newFacingRight;
	spriteScale = 1.0f;
	usingSprite = loadBulletTexture();
	setPosition(startX, startY);

	if (facingRight)
	{
		setVelocity(Constants::PLAYER_BULLET_SPEED, 0.0f);
	}
	else
	{
		setVelocity(-Constants::PLAYER_BULLET_SPEED, 0.0f);
	}

	body.setSize(sf::Vector2f(width, height));
	body.setFillColor(sf::Color(255, 210, 85));

	if (usingSprite)
	{
		bulletSprite.setTexture(bulletTexture);
		bulletSprite.setTextureRect(sf::IntRect(0, 0, 40, 35));
		updateSpritePosition();
	}
}

bool VehicleBullet::loadBulletTexture()
{
	if (textureLoaded)
	{
		return true;
	}

	sf::Image sheetImage;
	if (!sheetImage.loadFromFile("Sprites/Metal Slug.png"))
	{
		return false;
	}

	sf::Color backgroundColor = sheetImage.getPixel(0, 0);
	sf::Image bulletImage;
	bulletImage.create(40, 35, backgroundColor);
	bulletImage.copy(sheetImage, 0, 0, sf::IntRect(7, 17544, 40, 35));
	bulletImage.createMaskFromColor(backgroundColor);

	if (!bulletTexture.loadFromImage(bulletImage))
	{
		return false;
	}

	textureLoaded = true;
	return true;
}

void VehicleBullet::updateSpritePosition()
{
	if (!usingSprite)
	{
		return;
	}

	if (facingRight)
	{
		bulletSprite.setScale(spriteScale, spriteScale);
		bulletSprite.setPosition(x, y);
	}
	else
	{
		bulletSprite.setScale(-spriteScale, spriteScale);
		bulletSprite.setPosition(x + width, y);
	}
}

void VehicleBullet::update(float deltaTime)
{
	Projectile::update(deltaTime);
	updateSpritePosition();

	if (x + width < 0.0f || x > 25000.0f)
	{
		deactivate();
	}
}

void VehicleBullet::draw(sf::RenderWindow& window)
{
	if (!visible)
	{
		return;
	}

	if (usingSprite)
	{
		window.draw(bulletSprite);
	}
	else
	{
		window.draw(body);
	}
}
