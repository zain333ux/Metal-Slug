#include "MarinerMissile.h"
#include "Constants.h"


using namespace std;
using namespace sf;

MarinerMissile::MarinerMissile(float startX, float startY, bool facingRight, MarinerMissileType type)
{
    missileType = type;
<<<<<<< HEAD
    damage = 30; // Mariner missiles should be powerful
    lifeTime = 3;
    width = 32;
    height = 16;
=======
    damage = 30;
    lifeTime = 3.0f;
    width = 32.0f;
    height = 16.0f;
>>>>>>> 6d77c54b1816a0bae93ea8c1ad344f5e135692cd
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

<<<<<<< HEAD
    body.setSize(Vector2f(width, height));
    body.setFillColor(Color(255, 150, 50)); // Orange-ish for missiles
    body.setOutlineColor(Color::Black);
    body.setOutlineThickness(1);
=======
    body.setSize(sf::Vector2f(width, height));
    body.setFillColor(sf::Color(255, 150, 50));
    body.setOutlineColor(sf::Color::Black);
    body.setOutlineThickness(1.0f);
>>>>>>> 6d77c54b1816a0bae93ea8c1ad344f5e135692cd
}

void MarinerMissile::update(float deltaTime)
{
    if (missileType == MARINER_MISSILE_SURFACE)
    {
        velocityY -= Constants::GRAVITY * 1.2f * deltaTime;
<<<<<<< HEAD
        
        // Cap upward speed
        if (velocityY < -600)
            velocityY = -600;
=======
        if (velocityY < -600.0f)
            velocityY = -600.0f;
>>>>>>> 6d77c54b1816a0bae93ea8c1ad344f5e135692cd
    }

    Projectile::update(deltaTime);
}