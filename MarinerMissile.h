#pragma once

#include "BallisticProjectile.h"

enum MarinerMissileType
{
    MARINER_MISSILE_HORIZONTAL,
    MARINER_MISSILE_VERTICAL,
    MARINER_MISSILE_SURFACE
};

class MarinerMissile : public BallisticProjectile
{
private:
    MarinerMissileType missileType;

public:
    MarinerMissile(float startX, float startY, bool facingRight, MarinerMissileType type);

    virtual void update(float deltaTime) override;
};
