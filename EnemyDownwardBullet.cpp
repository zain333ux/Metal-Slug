#include "EnemyDownwardBullet.h"

#include "Constants.h"
#include "Level.h"

using namespace std;
using namespace sf;

static const IntRect MARTIAN_POD_PROJECTILE_FRAMES[] =
{
	IntRect(20, 67, 60, 60),
	IntRect(92, 67, 60, 60),
	IntRect(164, 67, 60, 60),
	IntRect(236, 67, 60, 60)
};

EnemyDownwardBullet::EnemyDownwardBullet(float startX, float startY, Level* level)
{
	damage = 12;
	lifeTime = 2.2f;
	width = 30;
	height = 30;
	activeLevel = level;
	setPlayerOwned(false);
	setPosition(startX, startY);
	setVelocity(0, 520);
	spriteLoaded = false;
	animationTimer = 0;
	currentFrame = 0;

	body.setSize(Vector2f(width, height));
	body.setFillColor(Color::Transparent);
	body.setOutlineThickness(0);

	Image image;
	if (image.loadFromFile("25I-0881_25I-0807_Assets/Sprites/Clean/MartianPod_Bullet.png") ||
		image.loadFromFile("25I-0881_25I-0807_Assets/Sprites/Clean/MartianPod_bullet.png"))
	{
		image.createMaskFromColor(Color(255, 0, 255));
		image.createMaskFromColor(Color::White);
		if (bulletTexture.loadFromImage(image))
		{
			bulletSprite.setTexture(bulletTexture);
			bulletSprite.setColor(Color::White);
			bulletSprite.setScale(0.72f, 0.72f);
			bulletSprite.setTextureRect(MARTIAN_POD_PROJECTILE_FRAMES[0]);
			spriteLoaded = true;
		}
	}
}

void EnemyDownwardBullet::update(float deltaTime)
{
	float previousBottom = y + height;
	Projectile::update(deltaTime);
	if (!isActive())
	{
		return;
	}

	float landingY = static_cast<float>(Constants::GROUND_Y);
	if (activeLevel != 0)
	{
		landingY = activeLevel->getLandingY(x, x + width, previousBottom, y + height);
	}

	if (y + height >= landingY)
	{
		deactivate();
	}

	if (spriteLoaded)
	{
		animationTimer += deltaTime;
		if (animationTimer >= 0.06f)
		{
			animationTimer = 0;
			currentFrame = (currentFrame + 1) % FRAME_COUNT;
			bulletSprite.setTextureRect(MARTIAN_POD_PROJECTILE_FRAMES[currentFrame]);
		}

		FloatRect bounds = bulletSprite.getGlobalBounds();
		bulletSprite.setPosition(x + width * 0.5f - bounds.width * 0.5f, y + height * 0.5f - bounds.height * 0.5f);
	}
}

void EnemyDownwardBullet::draw(RenderWindow& window)
{
	if (!visible)
	{
		return;
	}

	if (spriteLoaded)
	{
		window.draw(bulletSprite);
	}
	else
	{
		Projectile::draw(window);
	}
}