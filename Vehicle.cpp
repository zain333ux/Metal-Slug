#include "Vehicle.h"

#include "Constants.h"
#include "DeveloperMode.h"
#include "EntityManager.h"
#include "Level.h"
#include "PlayerSoldier.h"
#include "VehicleBullet.h"

#include <SFML/Window/Keyboard.hpp>
#include <cmath>

Vehicle::Vehicle(float startX, float startY)
{
	width = 110.0f;
	height = 96.0f;
	baseMaxHealth = 160;
	baseFireCooldown = 0.24f;
	maxHealth = baseMaxHealth;
	health = maxHealth;
	occupied = false;
	facingRight = true;
	grounded = false;
	moveSpeed = Constants::PLAYER_MOVE_SPEED * 1.2f;
	movementMaxX = Constants::WORLD_WIDTH_LEVEL_3 + 2200.0f;
	damageFlashTimer = 0.0f;
	activeLevel = 0;
	weapon.setCooldown(baseFireCooldown);

	resetAnimation(normalIdleAnimation);
	resetAnimation(normalMoveAnimation);
	resetAnimation(normalShootAnimation);
	resetAnimation(normalHitAnimation);
	resetAnimation(uphillIdleAnimation);
	resetAnimation(uphillMoveAnimation);
	resetAnimation(uphillShootAnimation);
	resetAnimation(uphillHitAnimation);
	resetAnimation(cannonAnimation);

	usingSprite = false;
	usingCannonSprite = false;
	usingUphillAnimation = false;
	spriteScale = 1.65f;
	cannonScale = 1.35f;
	currentAnimationKind = VEHICLE_ANIM_IDLE;
	currentAnimationFrame = 0;
	currentFrameWidth = 65;
	currentFrameHeight = 59;
	currentFrameBottomPadding = 2;
	currentCannonFrame = 0;
	currentCannonFrameWidth = 53;
	animationTimer = 0.0f;
	shootAnimationTimer = 0.0f;
	hitAnimationTimer = 0.0f;
	cannonTimer = 0.0f;
	cannonFrameTimer = 0.0f;

	body.setSize(sf::Vector2f(width, 44.0f));
	body.setOutlineColor(sf::Color::Black);
	body.setOutlineThickness(2.0f);

	cabin.setSize(sf::Vector2f(54.0f, 34.0f));
	cabin.setOutlineColor(sf::Color::Black);
	cabin.setOutlineThickness(2.0f);

	turret.setSize(sf::Vector2f(58.0f, 12.0f));
	turret.setOutlineColor(sf::Color::Black);
	turret.setOutlineThickness(1.0f);

	rearWheel.setRadius(15.0f);
	rearWheel.setFillColor(sf::Color(35, 35, 35));
	rearWheel.setOutlineColor(sf::Color::Black);
	rearWheel.setOutlineThickness(2.0f);

	frontWheel.setRadius(15.0f);
	frontWheel.setFillColor(sf::Color(35, 35, 35));
	frontWheel.setOutlineColor(sf::Color::Black);
	frontWheel.setOutlineThickness(2.0f);

	loadVehicleSprites();
	setPosition(startX, startY);
	updateVisualPosition();
	updateColors();
}

void Vehicle::resetAnimation(VehicleAnimation& animation)
{
	animation.totalWidth = 0;
	animation.frameHeight = 0;
	animation.frameCount = 0;
	animation.bottomPadding = 2;
	for (int i = 0; i < 20; i += 1)
	{
		animation.frameLefts[i] = 0;
		animation.frameWidths[i] = 1;
	}
	animation.frameDuration = 0.12f;
	animation.loop = true;
	animation.loaded = false;
}

bool Vehicle::loadVehicleSprites()
{
	sf::Image sheetImage;
	if (!sheetImage.loadFromFile("Sprites/Metal Slug.png"))
	{
		usingSprite = false;
		usingCannonSprite = false;
		return false;
	}

	sf::Color backgroundColor = sheetImage.getPixel(0, 0);

	loadAnimation(normalIdleAnimation, sheetImage, backgroundColor, 8, 23, 195, 59, 3, 0.18f, true);
	loadAnimation(normalMoveAnimation, sheetImage, backgroundColor, 263, 881, 354, 58, 5, 0.10f, true);
	loadAnimation(normalShootAnimation, sheetImage, backgroundColor, 330, 442, 249, 55, 4, 0.07f, false);
	loadAnimation(normalHitAnimation, sheetImage, backgroundColor, 10, 1553, 379, 67, 5, 0.07f, false);

	loadAnimation(uphillIdleAnimation, sheetImage, backgroundColor, 10, 2106, 193, 61, 3, 0.18f, true);
	loadAnimation(uphillMoveAnimation, sheetImage, backgroundColor, 280, 2828, 270, 61, 4, 0.10f, true);
	loadAnimation(uphillShootAnimation, sheetImage, backgroundColor, 345, 2540, 351, 64, 5, 0.07f, false);
	loadAnimation(uphillHitAnimation, sheetImage, backgroundColor, 6, 3576, 948, 70, 13, 0.05f, false);

	loadAnimation(cannonAnimation, sheetImage, backgroundColor, 10, 17875, 743, 35, 14, 0.015f, false);

	setFrameBounds(normalIdleAnimation, 0, 2, 60);
	setFrameBounds(normalIdleAnimation, 1, 67, 60);
	setFrameBounds(normalIdleAnimation, 2, 132, 60);

	setFrameBounds(normalMoveAnimation, 0, 5, 60);
	setFrameBounds(normalMoveAnimation, 1, 70, 59);
	setFrameBounds(normalMoveAnimation, 2, 134, 65);
	setFrameBounds(normalMoveAnimation, 3, 204, 69);
	setFrameBounds(normalMoveAnimation, 4, 278, 70);

	setFrameBounds(normalShootAnimation, 0, 1, 58);
	setFrameBounds(normalShootAnimation, 1, 64, 58);
	setFrameBounds(normalShootAnimation, 2, 127, 59);
	setFrameBounds(normalShootAnimation, 3, 191, 58);

	setFrameBounds(normalHitAnimation, 0, 0, 67);
	setFrameBounds(normalHitAnimation, 1, 72, 70);
	setFrameBounds(normalHitAnimation, 2, 147, 75);
	setFrameBounds(normalHitAnimation, 3, 227, 83);
	setFrameBounds(normalHitAnimation, 4, 315, 64);

	setFrameBounds(uphillIdleAnimation, 0, 0, 59);
	setFrameBounds(uphillIdleAnimation, 1, 64, 59);
	setFrameBounds(uphillIdleAnimation, 2, 128, 59);

	setFrameBounds(uphillMoveAnimation, 0, 3, 60);
	setFrameBounds(uphillMoveAnimation, 1, 68, 60);
	setFrameBounds(uphillMoveAnimation, 2, 133, 62);
	setFrameBounds(uphillMoveAnimation, 3, 200, 66);

	setFrameBounds(uphillShootAnimation, 0, 4, 64);
	setFrameBounds(uphillShootAnimation, 1, 73, 65);
	setFrameBounds(uphillShootAnimation, 2, 143, 65);
	setFrameBounds(uphillShootAnimation, 3, 213, 63);
	setFrameBounds(uphillShootAnimation, 4, 281, 63);

	setFrameBounds(uphillHitAnimation, 0, 4, 68);
	setFrameBounds(uphillHitAnimation, 1, 72, 73);
	setFrameBounds(uphillHitAnimation, 2, 145, 73);
	setFrameBounds(uphillHitAnimation, 3, 218, 73);
	setFrameBounds(uphillHitAnimation, 4, 291, 73);
	setFrameBounds(uphillHitAnimation, 5, 364, 73);
	setFrameBounds(uphillHitAnimation, 6, 437, 73);
	setFrameBounds(uphillHitAnimation, 7, 510, 73);
	setFrameBounds(uphillHitAnimation, 8, 583, 73);
	setFrameBounds(uphillHitAnimation, 9, 656, 73);
	setFrameBounds(uphillHitAnimation, 10, 729, 73);
	setFrameBounds(uphillHitAnimation, 11, 802, 73);
	setFrameBounds(uphillHitAnimation, 12, 880, 60);

	setFrameBounds(cannonAnimation, 0, 0, 53);
	setFrameBounds(cannonAnimation, 1, 53, 53);
	setFrameBounds(cannonAnimation, 2, 106, 53);
	setFrameBounds(cannonAnimation, 3, 159, 53);
	setFrameBounds(cannonAnimation, 4, 212, 53);
	setFrameBounds(cannonAnimation, 5, 265, 53);
	setFrameBounds(cannonAnimation, 6, 318, 53);
	setFrameBounds(cannonAnimation, 7, 371, 53);
	setFrameBounds(cannonAnimation, 8, 424, 53);
	setFrameBounds(cannonAnimation, 9, 477, 53);
	setFrameBounds(cannonAnimation, 10, 530, 53);
	setFrameBounds(cannonAnimation, 11, 583, 53);
	setFrameBounds(cannonAnimation, 12, 636, 53);
	setFrameBounds(cannonAnimation, 13, 689, 54);

	setAnimationBottomPadding(normalIdleAnimation, 2);
	setAnimationBottomPadding(normalMoveAnimation, 6);
	setAnimationBottomPadding(normalShootAnimation, 1);
	setAnimationBottomPadding(normalHitAnimation, 4);
	setAnimationBottomPadding(uphillIdleAnimation, 2);
	setAnimationBottomPadding(uphillMoveAnimation, 4);
	setAnimationBottomPadding(uphillShootAnimation, 2);
	setAnimationBottomPadding(uphillHitAnimation, 3);

	usingSprite = normalIdleAnimation.loaded;
	usingCannonSprite = cannonAnimation.loaded;

	if (usingSprite)
	{
		vehicleSprite.setTexture(normalIdleAnimation.texture);
		setAnimationFrame(normalIdleAnimation, 0);
	}
	if (usingCannonSprite)
	{
		cannonSprite.setTexture(cannonAnimation.texture);
		setCannonFrame(0);
	}

	return usingSprite;
}

bool Vehicle::loadAnimation(VehicleAnimation& animation, const sf::Image& sheetImage, sf::Color backgroundColor, int x, int y, int totalWidth, int totalHeight, int frameCount, float frameDuration, bool loop)
{
	if (totalWidth <= 0 || totalHeight <= 0 || frameCount <= 0)
	{
		return false;
	}

	sf::Image animationImage;
	animationImage.create(totalWidth, totalHeight, backgroundColor);
	animationImage.copy(sheetImage, 0, 0, sf::IntRect(x, y, totalWidth, totalHeight));
	animationImage.createMaskFromColor(backgroundColor);

	if (!animation.texture.loadFromImage(animationImage))
	{
		animation.loaded = false;
		return false;
	}

	animation.totalWidth = totalWidth;
	animation.frameHeight = totalHeight;
	animation.frameCount = frameCount;
	for (int i = 0; i < frameCount && i < 20; i += 1)
	{
		int frameLeft = totalWidth * i / frameCount;
		int frameRight = totalWidth * (i + 1) / frameCount;
		animation.frameLefts[i] = frameLeft;
		animation.frameWidths[i] = frameRight - frameLeft;
	}
	animation.frameDuration = frameDuration;
	animation.loop = loop;
	animation.loaded = true;
	return true;
}

void Vehicle::setFrameBounds(VehicleAnimation& animation, int frameIndex, int frameLeft, int frameWidth)
{
	if (frameIndex < 0 || frameIndex >= 20 || frameIndex >= animation.frameCount)
	{
		return;
	}
	if (frameWidth <= 0)
	{
		return;
	}

	animation.frameLefts[frameIndex] = frameLeft;
	animation.frameWidths[frameIndex] = frameWidth;
}

void Vehicle::setAnimationBottomPadding(VehicleAnimation& animation, int bottomPadding)
{
	if (bottomPadding < 0)
	{
		bottomPadding = 0;
	}

	animation.bottomPadding = bottomPadding;
}

void Vehicle::applyDriverBonuses(const PlayerSoldier* driver)
{
	float healthRatio = 1.0f;
	if (maxHealth > 0)
	{
		healthRatio = static_cast<float>(health) / static_cast<float>(maxHealth);
	}

	float durabilityMultiplier = 1.0f;
	float cooldownMultiplier = 1.0f;
	if (driver != 0)
	{
		durabilityMultiplier = driver->getVehicleDurabilityMultiplier();
		cooldownMultiplier = driver->getVehicleFireCooldownMultiplier();
	}

	maxHealth = static_cast<int>(static_cast<float>(baseMaxHealth) * durabilityMultiplier + 0.5f);
	if (maxHealth < 1)
	{
		maxHealth = 1;
	}

	if (health <= 0)
	{
		health = 0;
	}
	else
	{
		health = static_cast<int>(static_cast<float>(maxHealth) * healthRatio + 0.5f);
		if (health < 1)
		{
			health = 1;
		}
		if (health > maxHealth)
		{
			health = maxHealth;
		}
	}

	weapon.setCooldown(baseFireCooldown * cooldownMultiplier);
}

Vehicle::VehicleAnimation& Vehicle::getAnimation(int animationKind, bool uphill)
{
	if (uphill)
	{
		if (animationKind == VEHICLE_ANIM_MOVE && uphillMoveAnimation.loaded)
		{
			return uphillMoveAnimation;
		}
		if (animationKind == VEHICLE_ANIM_SHOOT && uphillShootAnimation.loaded)
		{
			return uphillShootAnimation;
		}
		if (animationKind == VEHICLE_ANIM_HIT && uphillHitAnimation.loaded)
		{
			return uphillHitAnimation;
		}
		if (uphillIdleAnimation.loaded)
		{
			return uphillIdleAnimation;
		}
	}

	if (animationKind == VEHICLE_ANIM_MOVE && normalMoveAnimation.loaded)
	{
		return normalMoveAnimation;
	}
	if (animationKind == VEHICLE_ANIM_SHOOT && normalShootAnimation.loaded)
	{
		return normalShootAnimation;
	}
	if (animationKind == VEHICLE_ANIM_HIT && normalHitAnimation.loaded)
	{
		return normalHitAnimation;
	}

	return normalIdleAnimation;
}

void Vehicle::setAnimationFrame(VehicleAnimation& animation, int frameIndex)
{
	if (!animation.loaded || animation.frameCount <= 0)
	{
		return;
	}

	if (frameIndex < 0)
	{
		frameIndex = 0;
	}
	if (frameIndex >= animation.frameCount)
	{
		if (animation.loop)
		{
			frameIndex = 0;
		}
		else
		{
			frameIndex = animation.frameCount - 1;
		}
	}

	currentAnimationFrame = frameIndex;
	int frameLeft = animation.frameLefts[frameIndex];
	currentFrameWidth = animation.frameWidths[frameIndex];
	currentFrameHeight = animation.frameHeight;
	currentFrameBottomPadding = animation.bottomPadding;
	if (currentFrameWidth <= 0)
	{
		currentFrameWidth = 1;
	}

	vehicleSprite.setTexture(animation.texture);
	vehicleSprite.setTextureRect(sf::IntRect(frameLeft, 0, currentFrameWidth, currentFrameHeight));
}

void Vehicle::setCannonFrame(int frameIndex)
{
	if (!cannonAnimation.loaded || cannonAnimation.frameCount <= 0)
	{
		return;
	}

	if (frameIndex < 0)
	{
		frameIndex = 0;
	}
	if (frameIndex >= cannonAnimation.frameCount)
	{
		frameIndex = cannonAnimation.frameCount - 1;
	}

	currentCannonFrame = frameIndex;
	int frameLeft = cannonAnimation.frameLefts[frameIndex];
	currentCannonFrameWidth = cannonAnimation.frameWidths[frameIndex];
	if (currentCannonFrameWidth <= 0)
	{
		currentCannonFrameWidth = 1;
	}

	cannonSprite.setTexture(cannonAnimation.texture);
	cannonSprite.setTextureRect(sf::IntRect(frameLeft, 0, currentCannonFrameWidth, cannonAnimation.frameHeight));
}

void Vehicle::handleMovementInput()
{
	velocityX = 0.0f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		velocityX = -moveSpeed;
		facingRight = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		velocityX = moveSpeed;
		facingRight = true;
	}
}

void Vehicle::update(float deltaTime)
{
	if (DeveloperMode::isEnabled())
	{
		health = maxHealth;
	}

	if (damageFlashTimer > 0.0f)
	{
		damageFlashTimer -= deltaTime;
	}
	if (shootAnimationTimer > 0.0f)
	{
		shootAnimationTimer -= deltaTime;
	}
	if (hitAnimationTimer > 0.0f)
	{
		hitAnimationTimer -= deltaTime;
	}

	if (occupied)
	{
		handleMovementInput();
		usingUphillAnimation = sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
	}
	else
	{
		velocityX = 0.0f;
		usingUphillAnimation = false;
	}

	velocityY += Constants::GRAVITY * deltaTime;

	float previousBottom = y + height;
	Entity::update(deltaTime);

	float landingY = static_cast<float>(Constants::GROUND_Y);
	if (activeLevel != 0)
	{
		landingY = activeLevel->getLandingY(x, x + width, previousBottom, y + height);
	}

	if (y + height >= landingY)
	{
		y = landingY - height;
		velocityY = 0.0f;
		grounded = true;
	}
	else
	{
		grounded = false;
	}

	if (x < 0.0f)
	{
		x = 0.0f;
	}

	if (x + width > movementMaxX)
	{
		x = movementMaxX - width;
	}

	updateAnimation(deltaTime);
	updateCannonAnimation(deltaTime);
	updateVisualPosition();
	updateColors();
}

void Vehicle::updateAnimation(float deltaTime)
{
	if (!usingSprite)
	{
		return;
	}

	int nextAnimationKind = VEHICLE_ANIM_IDLE;
	if (hitAnimationTimer > 0.0f)
	{
		nextAnimationKind = VEHICLE_ANIM_HIT;
	}
	else if (shootAnimationTimer > 0.0f)
	{
		nextAnimationKind = VEHICLE_ANIM_SHOOT;
	}
	else if (velocityX != 0.0f)
	{
		nextAnimationKind = VEHICLE_ANIM_MOVE;
	}

	if (nextAnimationKind != currentAnimationKind)
	{
		currentAnimationKind = nextAnimationKind;
		currentAnimationFrame = 0;
		animationTimer = 0.0f;
		setAnimationFrame(getAnimation(currentAnimationKind, usingUphillAnimation), 0);
		return;
	}

	VehicleAnimation& animation = getAnimation(currentAnimationKind, usingUphillAnimation);
	animationTimer += deltaTime;
	if (animationTimer >= animation.frameDuration)
	{
		animationTimer = 0.0f;
		setAnimationFrame(animation, currentAnimationFrame + 1);
	}
}

void Vehicle::updateCannonAnimation(float deltaTime)
{
	if (!usingCannonSprite || cannonTimer <= 0.0f)
	{
		return;
	}

	cannonTimer -= deltaTime;
	if (cannonTimer <= 0.0f)
	{
		cannonTimer = 0.0f;
		return;
	}

	cannonFrameTimer += deltaTime;
	if (cannonFrameTimer >= cannonAnimation.frameDuration)
	{
		cannonFrameTimer = 0.0f;
		setCannonFrame(currentCannonFrame + 1);
	}
}

void Vehicle::handleWeaponInput(EntityManager& entityManager, float deltaTime)
{
	weapon.update(deltaTime);

	if (!occupied || isDead())
	{
		return;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::J))
	{
		bool canFireNow = weapon.canFire();
		if (canFireNow)
		{
			float cannonY = usingUphillAnimation ? y + 16.0f : y + 28.0f;
			float bulletX = facingRight ? x + width + 48.0f : x - 48.0f;
			float bulletY = cannonY;
			entityManager.addEntity(new VehicleBullet(bulletX, bulletY, facingRight));
			weapon.restartCooldown();

			VehicleAnimation& shootAnimation = getAnimation(VEHICLE_ANIM_SHOOT, usingUphillAnimation);
			shootAnimationTimer = shootAnimation.frameDuration * static_cast<float>(shootAnimation.frameCount);
			currentAnimationKind = VEHICLE_ANIM_SHOOT;
			currentAnimationFrame = 0;
			animationTimer = 0.0f;
			setAnimationFrame(shootAnimation, 0);
			cannonTimer = cannonAnimation.frameDuration * static_cast<float>(cannonAnimation.frameCount);
			cannonFrameTimer = 0.0f;
			setCannonFrame(0);
		}
	}
}

void Vehicle::takeDamage(int damage)
{
	if (damage <= 0 || isDead() || DeveloperMode::isEnabled())
	{
		return;
	}

	health -= damage;
	damageFlashTimer = 0.18f;
	VehicleAnimation& hitAnimation = getAnimation(VEHICLE_ANIM_HIT, usingUphillAnimation);
	hitAnimationTimer = hitAnimation.frameDuration * static_cast<float>(hitAnimation.frameCount);
	currentAnimationKind = VEHICLE_ANIM_HIT;
	currentAnimationFrame = 0;
	animationTimer = 0.0f;
	setAnimationFrame(hitAnimation, 0);

	if (health <= 0)
	{
		health = 0;
		deactivate();
	}
}

bool Vehicle::canMount(const PlayerSoldier& player) const
{
	if (occupied || isDead() || player.isDead() || player.isRidingVehicle())
	{
		return false;
	}

	float distanceX = player.getCenterX() - getCenterX();
	float distanceY = player.getCenterY() - getCenterY();
	return std::abs(distanceX) <= 120.0f && std::abs(distanceY) <= 115.0f;
}

void Vehicle::mount(PlayerSoldier& player)
{
	if (!canMount(player))
	{
		return;
	}

	occupied = true;
	applyDriverBonuses(&player);
	player.setRidingVehicle(true);
	player.setVelocity(0.0f, 0.0f);
	player.setPosition(getSeatX(), getSeatY());
}

void Vehicle::dismount(PlayerSoldier& player)
{
	if (!occupied)
	{
		return;
	}

	occupied = false;
	applyDriverBonuses(0);
	player.setRidingVehicle(false);
	player.setVelocity(0.0f, 0.0f);

	float exitX = facingRight ? x - player.getWidth() - 8.0f : x + width + 8.0f;
	if (exitX < 0.0f)
	{
		exitX = x + width + 8.0f;
	}
	if (exitX + player.getWidth() > movementMaxX)
	{
		exitX = x - player.getWidth() - 8.0f;
	}

	player.setPosition(exitX, y + height - player.getHeight());
}

void Vehicle::ejectPlayer(PlayerSoldier& player)
{
	if (!occupied)
	{
		return;
	}

	bool driverSurvives = player.survivesVehicleDestruction();
	occupied = false;
	applyDriverBonuses(0);
	player.setRidingVehicle(false);
	player.setVelocity(0.0f, 0.0f);
	player.setPosition(x + width / 2.0f - player.getWidth() / 2.0f, y + height - player.getHeight());
	if (!driverSurvives)
	{
		player.handleVehicleDestruction();
	}
}

void Vehicle::updateVisualPosition()
{
	body.setPosition(x, y + 48.0f);
	cabin.setPosition(x + 30.0f, y + 28.0f);
	rearWheel.setPosition(x + 14.0f, y + 66.0f);
	frontWheel.setPosition(x + 66.0f, y + 66.0f);

	if (usingSprite)
	{
		float drawX = x + width / 2.0f - static_cast<float>(currentFrameWidth) * spriteScale / 2.0f;
		float drawY = y + height - static_cast<float>(currentFrameHeight) * spriteScale;
		drawY += static_cast<float>(currentFrameBottomPadding - 2) * spriteScale;

		if (facingRight)
		{
			vehicleSprite.setScale(spriteScale, spriteScale);
			vehicleSprite.setPosition(drawX, drawY);
		}
		else
		{
			vehicleSprite.setScale(-spriteScale, spriteScale);
			vehicleSprite.setPosition(drawX + static_cast<float>(currentFrameWidth) * spriteScale, drawY);
		}
	}

	if (usingCannonSprite)
	{
		float cannonY = usingUphillAnimation ? y + 16.0f : y + 28.0f;
		if (facingRight)
		{
			cannonSprite.setScale(cannonScale, cannonScale);
			cannonSprite.setPosition(x + width - 8.0f, cannonY);
		}
		else
		{
			cannonSprite.setScale(-cannonScale, cannonScale);
			cannonSprite.setPosition(x + 8.0f, cannonY);
		}
	}

	if (facingRight)
	{
		turret.setPosition(x + 58.0f, y + 42.0f);
	}
	else
	{
		turret.setPosition(x - 8.0f, y + 42.0f);
	}
}

void Vehicle::updateColors()
{
	if (usingSprite)
	{
		if (damageFlashTimer > 0.0f)
		{
			vehicleSprite.setColor(sf::Color(255, 130, 130));
		}
		else
		{
			vehicleSprite.setColor(sf::Color::White);
		}
		return;
	}

	if (damageFlashTimer > 0.0f)
	{
		body.setFillColor(sf::Color(230, 70, 55));
		cabin.setFillColor(sf::Color(255, 120, 90));
		turret.setFillColor(sf::Color(255, 120, 90));
		return;
	}

	if (occupied)
	{
		body.setFillColor(sf::Color(72, 125, 78));
		cabin.setFillColor(sf::Color(96, 160, 98));
		turret.setFillColor(sf::Color(64, 100, 68));
	}
	else
	{
		body.setFillColor(sf::Color(82, 100, 76));
		cabin.setFillColor(sf::Color(105, 120, 92));
		turret.setFillColor(sf::Color(66, 78, 60));
	}
}

void Vehicle::draw(sf::RenderWindow& window)
{
	if (!visible)
	{
		return;
	}

	if (usingSprite)
	{
		window.draw(vehicleSprite);
		if (cannonTimer > 0.0f && usingCannonSprite)
		{
			window.draw(cannonSprite);
		}
	}
	else
	{
		window.draw(body);
		window.draw(cabin);
		window.draw(turret);
		window.draw(rearWheel);
		window.draw(frontWheel);
	}

	float healthRatio = static_cast<float>(health) / static_cast<float>(maxHealth);
	if (healthRatio < 0.0f)
	{
		healthRatio = 0.0f;
	}

	sf::RectangleShape healthBack;
	healthBack.setPosition(x, y - 12.0f);
	healthBack.setSize(sf::Vector2f(width, 6.0f));
	healthBack.setFillColor(sf::Color(70, 20, 20));
	window.draw(healthBack);

	sf::RectangleShape healthFront;
	healthFront.setPosition(x, y - 12.0f);
	healthFront.setSize(sf::Vector2f(width * healthRatio, 6.0f));
	healthFront.setFillColor(sf::Color(80, 230, 100));
	window.draw(healthFront);
}

void Vehicle::setActiveLevel(Level* level)
{
	activeLevel = level;
}

void Vehicle::setMovementMaxX(float maxX)
{
	if (maxX > width)
	{
		movementMaxX = maxX;
	}
}

bool Vehicle::isOccupied() const
{
	return occupied;
}

bool Vehicle::isFacingRight() const
{
	return facingRight;
}

int Vehicle::getMaxHealth() const
{
	return maxHealth;
}

float Vehicle::getSeatX() const
{
	return x + width / 2.0f - 26.0f;
}

float Vehicle::getSeatY() const
{
	return y + height - 96.0f;
}
