#include "MarinerMissile.h"
#include "Constants.h"


using namespace std;
using namespace sf;

MarinerMissile::MarinerMissile(float startX, float startY, bool facingRight, MarinerMissileType type)
{
    missileType = type;
    damage = 30;
    lifeTime = 3;
    width = 32;
    height = 16;
    explosive = true;
    blastRadius = 120;
    setPosition(startX, startY);

    float speed = 500;

    switch (missileType)
    {
    case MARINER_MISSILE_HORIZONTAL:
        if (facingRight)
            setVelocity(speed, 0);
        else
            setVelocity(-speed, 0);
        break;
    case MARINER_MISSILE_VERTICAL:
        setVelocity(0, -speed);
        width = 16;
        height = 32;
        break;
    case MARINER_MISSILE_SURFACE:
        if (facingRight)
            setVelocity(200, 100);
        else
            setVelocity(-200, 100);
        break;
    }

    body.setSize(Vector2f(width, height));
    body.setFillColor(Color(255, 150, 50));
    body.setOutlineColor(Color::Black);
    body.setOutlineThickness(1);
}

void MarinerMissile::update(float deltaTime)
{
    if (missileType == MARINER_MISSILE_SURFACE)
    {
        velocityY -= Constants::GRAVITY * 1.2f * deltaTime;
        if (velocityY < -600)
            velocityY = -600;
    }

    Projectile::update(deltaTime);
}
