#include "MeleeHitbox.h"


using namespace std;
using namespace sf;

MeleeHitbox::MeleeHitbox(float startX, float startY, bool facingRight, bool airborne)
{
	damage = 20;
	lifeTime = 0.08f;
	width = 48;
	height = 58;
	melee = true;
	markFiredWhileAirborne(airborne);

	if (!facingRight)
	{
		startX -= width;
	}

	setPosition(startX, startY);
	body.setSize(Vector2f(width, height));
	body.setFillColor(Color(255, 255, 255, 90));
}