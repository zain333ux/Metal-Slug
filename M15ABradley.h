#pragma once



#include "EnemyVehicle.h"

#include "SpriteAnimation.h"



class M15ABradley : public GroundVehicleEnemy

{

private:

	float preferredRange;

	bool bradleySpritesLoaded;

	SpriteAnimation idleAnim;



	void buildSprites();



protected:

	virtual void updateAI() override;



public:

	M15ABradley();



	virtual void update(float deltaTime) override;

	virtual void draw(sf::RenderWindow& window) override;



	virtual Projectile* attack() override;

	virtual const char* getType() const override;

	virtual EnemyVehicleType getVehicleType() const override;

	virtual const char* getEnemyName() const override;

};


