#pragma once



#include "EnemyVehicle.h"

#include "SpriteAnimation.h"



enum class FlyingTaraAnimKind

{

	Idle,

	MoveUp,

	MoveDown

};



class FlyingTara : public AerialVehicleEnemy

{

private:

	float hoverY;

	FlyingTaraAnimKind visibleKind;

	SpriteAnimation animIdle;

	SpriteAnimation animUp;

	SpriteAnimation animDown;

	bool taraSpritesLoaded;

	float damageFlashTimer;

	float lastVerticalVelocity;



	void buildSprites();



protected:

	virtual void updateAI() override;



public:

	FlyingTara();



	virtual void update(float deltaTime) override;

	virtual Projectile* attack() override;

	virtual void draw(sf::RenderWindow& window) override;



	virtual const char* getType() const override;

	virtual EnemyVehicleType getVehicleType() const override;

	virtual const char* getEnemyName() const override;

};


