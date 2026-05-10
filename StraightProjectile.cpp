#include "StraightProjectile.h"

#include "Constants.h"


using namespace std;
using namespace sf;

static Texture pistolBulletTexture;
static bool pistolBulletTextureLoaded = false;

static void applyPistolBulletTexture(RectangleShape& body)
{
	if (!pistolBulletTextureLoaded)
	{
		Image image;
		if (image.loadFromFile("Sprites/Clean/Pistol_bullet.png"))
		{
			image.createMaskFromColor(Color::White);
			pistolBulletTextureLoaded = pistolBulletTexture.loadFromImage(image);
		}
	}

	if (pistolBulletTextureLoaded)
	{
		body.setTexture(&pistolBulletTexture);
		body.setFillColor(Color::White);
	}
}

StraightProjectile::StraightProjectile()
{
}

StraightProjectile::StraightProjectile(float startX, float startY, bool facingRight)
	: StraightProjectile(startX, startY, facingRight, false)
{
}

StraightProjectile::StraightProjectile(float startX, float startY, bool facingRight, bool upward)
{
	width = 20;
	height = 20;
	body.setSize(Vector2f(width, height));
	body.setOutlineThickness(0);
	applyPistolBulletTexture(body);
	setPosition(startX, startY);

	if (upward)
	{
		setVelocity(0, -Constants::PLAYER_BULLET_SPEED);
	}
	else if (facingRight)
	{
		setVelocity(Constants::PLAYER_BULLET_SPEED, 0);
	}
	else
	{
		setVelocity(-Constants::PLAYER_BULLET_SPEED, 0);
	}
}

void StraightProjectile::update(float deltaTime)
{
	Projectile::update(deltaTime);

	if (x + width < 0 || x > 25000 || y + height < 0)
	{
		deactivate();
	}
}

StraightProjectile::~StraightProjectile()
{
}

BasicStraightProjectile::BasicStraightProjectile(float startX, float startY, bool facingRight, bool upward)
	: StraightProjectile(startX, startY, facingRight, upward)
{
}
