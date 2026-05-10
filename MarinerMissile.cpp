#include "MarinerMissile.h"
#include "Constants.h"

MarinerMissile::MarinerMissile(float startX, float startY, bool facingRight, MarinerMissileType type)
{
    missileType = type;
    damage = 30;
    lifeTime = 3.0f;
    width = 32.0f;
    height = 16.0f;
    explosive = true;
    blastRadius = 120.0f;
    setPosition(startX, startY);

    float speed = 500.0f;

    switch (missileType)
    {
    case MARINER_MISSILE_HORIZONTAL:
        if (facingRight)
            setVelocity(speed, 0.0f);
        else
            setVelocity(-speed, 0.0f);
        break;
    case MARINER_MISSILE_VERTICAL:
        setVelocity(0.0f, -speed);
        width = 16.0f;
        height = 32.0f;
        break;
    case MARINER_MISSILE_SURFACE:
        if (facingRight)
            setVelocity(200.0f, 100.0f);
        else
            setVelocity(-200.0f, 100.0f);
        break;
    }

    body.setSize(sf::Vector2f(width, height));
    body.setFillColor(sf::Color(255, 150, 50));
    body.setOutlineColor(sf::Color::Black);
    body.setOutlineThickness(1.0f);
}

void MarinerMissile::update(float deltaTime)
{
    if (missileType == MARINER_MISSILE_SURFACE)
    {
        velocityY -= Constants::GRAVITY * 1.2f * deltaTime;
        if (velocityY < -600.0f)
            velocityY = -600.0f;
    }

    Projectile::update(deltaTime);
}
