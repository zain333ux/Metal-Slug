#include "VehicleBullet.h"

#include "Constants.h"


using namespace std;
using namespace sf;

bool VehicleBullet::textureLoaded = false;
Texture VehicleBullet::bulletTexture;

VehicleBullet::VehicleBullet(float startX, float startY, bool newFacingRight)
{
	damage = 18;
	lifeTime = 2;
	width = 40;
	height = 35;
	facingRight = newFacingRight;
	spriteScale = 1;
	usingSprite = loadBulletTexture();
	setPosition(startX, startY);

	if (facingRight)
	{
		setVelocity(Constants::PLAYER_BULLET_SPEED, 0);
	}
	else
	{
		setVelocity(-Constants::PLAYER_BULLET_SPEED, 0);
	}

	body.setSize(Vector2f(width, height));
	body.setFillColor(Color(255, 210, 85));

	if (usingSprite)
	{
		bulletSprite.setTexture(bulletTexture);
		bulletSprite.setTextureRect(IntRect(0, 0, 40, 35));
		updateSpritePosition();
	}
}

bool VehicleBullet::loadBulletTexture()
{
	if (textureLoaded)
	{
		return true;
	}

	Image sheetImage;
	if (!sheetImage.loadFromFile("25I-0881_25I-0807_Assets/Sprites/Metal Slug.png"))
	{
		return false;
	}

	Color backgroundColor = sheetImage.getPixel(0, 0);
	Image bulletImage;
	bulletImage.create(40, 35, backgroundColor);
	bulletImage.copy(sheetImage, 0, 0, IntRect(7, 17544, 40, 35));
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

	if (x + width < 0 || x > 25000)
	{
		deactivate();
	}
}

void VehicleBullet::draw(RenderWindow& window)
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