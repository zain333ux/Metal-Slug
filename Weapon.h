#pragma once

class EntityManager;

class Weapon
{
protected:
	int damage;
	float cooldown;
	float cooldownTimer;

public:
	Weapon();
	virtual ~Weapon();

	virtual void update(float deltaTime);
	virtual void fire(EntityManager& entityManager, float startX, float startY, bool facingRight) = 0;

	bool canFire() const;
};
