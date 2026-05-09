#pragma once



#include "EnemyVehicle.h"

#include "SpriteAnimation.h"



/// Aquatic submarine: move / direction-change turn / FallingApart death, torpedo uses Enemy submarine bullet sprite.

class EnemySub : public AquaticVehicleEnemy

{

private:

	float verticalSpeed;

	float damageFlashTimer;



	bool dyingPlaying;

	bool turnPlaying;



	SpriteAnimation moveAnim;

	SpriteAnimation turnAnim;

	SpriteAnimation deathAnim;

	bool subSpritesLoaded;



	void buildSpritesIfNeeded();



protected:

	virtual void updateAI() override;



public:

	EnemySub();



	virtual void update(float deltaTime) override;

	virtual Projectile* attack() override;

	virtual Projectile* createProjectileIfReady() override;

	virtual void draw(sf::RenderWindow& window) override;

	virtual void takeDamage(int damage) override;

	virtual bool applyProjectileHit(Projectile& projectile) override;

	virtual bool canDealContactDamage() const override;



	virtual const char* getType() const override;

	virtual EnemyVehicleType getVehicleType() const override;

	virtual const char* getEnemyName() const override;

};


