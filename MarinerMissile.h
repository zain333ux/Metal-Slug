#pragma once

#include "Projectile.h"

enum MarinerMissileType
{
    MARINER_MISSILE_HORIZONTAL,
    MARINER_MISSILE_VERTICAL,
    MARINER_MISSILE_SURFACE // Reverse projectile motion
};

class MarinerMissile : public Projectile
{
private:
    MarinerMissileType missileType;

public:
    MarinerMissile(float startX, float startY, bool facingRight, MarinerMissileType type);

    virtual void update(float deltaTime) override;
};
