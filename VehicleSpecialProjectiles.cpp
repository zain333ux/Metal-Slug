#include "VehicleSpecialProjectiles.h"

#include "Constants.h"
#include "VehicleTextureManager.h"

namespace
{
	void pushHorizontalStrip(FrameRect* out, int& count, const int* widths, int n, int sheetH, int refW)
	{
		int xAcc = 0;
		for (int i = 0; i < n; i += 1)
		{
			int ww = widths[i];
			int ox = (refW - ww) / 2;
			out[count] = FrameRect{xAcc, 0, ww, sheetH, ox, 0};
			count += 1;
			xAcc += ww;
		}
	}

	sf::Texture enemyVehicleRocketTexture;
	bool enemyVehicleRocketTextureLoaded = false;

	bool loadEnemyVehicleRocketTexture()
	{
		if (!enemyVehicleRocketTextureLoaded)
		{
			sf::Image image;
			if (image.loadFromFile("Sprites/Clean/Bazooka_bullet.png"))
			{
				image.createMaskFromColor(sf::Color::White);
				image.createMaskFromColor(sf::Color(255, 0, 255));
				enemyVehicleRocketTextureLoaded = enemyVehicleRocketTexture.loadFromImage(image);
			}
		}
		return enemyVehicleRocketTextureLoaded;
	}
} // namespace

// --- Bradley arcing missile -------------------------------------------------

BradleyArcMissileProjectile::BradleyArcMissileProjectile(float startX, float startY, float vx, float vy)
	: BallisticProjectile(startX, startY, vx, vy, 32, 145.0f, 0.62f, false)
{
	lastAscending = vy < 0.0f;
	width = 26.0f;
	height = 26.0f;
	body.setSize(sf::Vector2f(width, height));
	body.setFillColor(sf::Color::Transparent);
	body.setOutlineColor(sf::Color::Transparent);
	body.setOutlineThickness(0.0f);

	const sf::Texture* tex = VehicleTextureManager::instance().getTexture(VehicleTextureId::BradleyRocketUp);
	static const int bW[] = {82, 82, 83, 82, 83, 83};
	static FrameRect bFrames[6];
	static bool bGeo = false;
	if (tex != nullptr && !bGeo)
	{
		int c = 0;
		pushHorizontalStrip(bFrames, c, bW, 6, 74, 83);
		bGeo = true;
	}

	if (tex != nullptr && bGeo)
	{
		rocketAnim.setTexture(tex);
		rocketAnim.setFrames(bFrames, 6);
		rocketAnim.setFrameTime(0.075f);
		rocketAnim.setLoop(true);
		rocketAnim.setScale(1.45f, 1.45f);
		rocketAnim.reset();
	}
	syncAnimationDirection();
}

void BradleyArcMissileProjectile::syncAnimationDirection()
{
	lastAscending = velocityY < 0.0f;
	rocketAnim.setPlayBackward(!lastAscending);
}

void BradleyArcMissileProjectile::update(float deltaTime)
{
	BallisticProjectile::update(deltaTime);
	syncAnimationDirection();
	rocketAnim.update(deltaTime);
}

void BradleyArcMissileProjectile::draw(sf::RenderWindow& window)
{
	const sf::Texture* tex = VehicleTextureManager::instance().getTexture(VehicleTextureId::BradleyRocketUp);
	if (tex != nullptr && rocketAnim.getFrameCount() > 0)
	{
		rocketAnim.setFacingRight(velocityX >= 0.0f);
		float anchorCx = x + width * 0.5f;
		float anchorBottom = y + height;
		rocketAnim.drawAtAnchor(window, anchorCx, anchorBottom);
	}
	else
	{
		Projectile::draw(window);
	}
}

// --- Slug Flyer rocket -------------------------------------------------------

SlugFlyerRocketProjectile::SlugFlyerRocketProjectile(float startX, float startY, bool facingRight, bool airborne)
{
	damage = 35;
	lifeTime = 2.0f;
	width = 36.0f;
	height = 14.0f;
	explosive = true;
	blastRadius = 190.0f;
	deferUsed = false;
	phase = PHASE_FLY;
	markFiredWhileAirborne(airborne);
	setPosition(startX, startY);

	if (facingRight)
	{
		setVelocity(650.0f, 0.0f);
	}
	else
	{
		setVelocity(-650.0f, 0.0f);
	}

	body.setSize(sf::Vector2f(width, height));
	body.setFillColor(sf::Color::Transparent);
	body.setOutlineColor(sf::Color::Transparent);
	body.setOutlineThickness(0.0f);

	const sf::Texture* tex = VehicleTextureManager::instance().getTexture(VehicleTextureId::SlugFlyerRocket);
	static const int flyW[] = {33, 33, 33, 33, 33, 33, 41, 42, 43, 43};
	static FrameRect flyFrames[10];
	static const int smokeW[] = {26, 26, 26, 26, 23};
	static FrameRect smokeFrames[5];
	static bool slugRocketGeoDone = false;
	if (tex != nullptr && !slugRocketGeoDone)
	{
		int fc = 0;
		pushHorizontalStrip(flyFrames, fc, flyW, 10, 23, 43);
		int flyTotal = flyFrames[9].x + flyFrames[9].w;

		int sc = 0;
		pushHorizontalStrip(smokeFrames, sc, smokeW, 5, 23, 26);
		for (int s = 0; s < 5; s += 1)
		{
			smokeFrames[s].x += flyTotal;
		}
		slugRocketGeoDone = true;
	}

	if (tex != nullptr && slugRocketGeoDone)
	{
		flyAnim.setTexture(tex);
		flyAnim.setFrames(flyFrames, 10);
		flyAnim.setFrameTime(0.045f);
		flyAnim.setLoop(true);
		flyAnim.setScale(2.05f, 2.05f);
		flyAnim.setFacingRight(facingRight);
		flyAnim.reset();

		smokeAnim.setTexture(tex);
		smokeAnim.setFrames(smokeFrames, 5);
		smokeAnim.setFrameTime(0.058f);
		smokeAnim.setLoop(false);
		smokeAnim.setScale(2.05f, 2.05f);
		smokeAnim.setFacingRight(facingRight);
		smokeAnim.reset();
		smokeAnim.setFinished(true);
	}
}

void SlugFlyerRocketProjectile::beginSmokeAtImpact()
{
	phase = PHASE_SMOKE;
	setVelocity(0.0f, 0.0f);
	damage = 0;
	explosive = false;
	blastRadius = 0.0f;
	smokeAnim.setFinished(false);
	smokeAnim.reset();
	width = 12.0f;
	height = 12.0f;
	body.setPosition(x, y);
}

bool SlugFlyerRocketProjectile::deferProjectileDeactivateAfterEnemyHit()
{
	if (deferUsed || phase != PHASE_FLY)
	{
		return false;
	}

	deferUsed = true;
	beginSmokeAtImpact();
	return true;
}

bool SlugFlyerRocketProjectile::shouldIgnorePlayerProjectileVsEnemyChecks() const
{
	return phase == PHASE_SMOKE;
}

void SlugFlyerRocketProjectile::update(float deltaTime)
{
	if (!isActive())
	{
		return;
	}

	if (phase == PHASE_SMOKE)
	{
		smokeAnim.update(deltaTime);
		body.setPosition(x, y);
		if (smokeAnim.isFinished())
		{
			deactivate();
		}
		return;
	}

	lifeTime -= deltaTime;
	if (lifeTime <= 0.0f)
	{
		deactivate();
		return;
	}

	Projectile::update(deltaTime);
	flyAnim.update(deltaTime);

	float maxBound = Constants::WORLD_WIDTH_LEVEL_3 + 1600.0f;
	if (x + width < -400.0f || x > maxBound || y + height < -200.0f || y > 2200.0f)
	{
		deactivate();
	}
}

void SlugFlyerRocketProjectile::draw(sf::RenderWindow& window)
{
	float anchorCx = x + width * 0.5f;
	float anchorBottom = y + height;
	const sf::Texture* tex = VehicleTextureManager::instance().getTexture(VehicleTextureId::SlugFlyerRocket);

	if (tex != nullptr)
	{
		if (phase == PHASE_SMOKE)
		{
			smokeAnim.drawAtAnchor(window, anchorCx, anchorBottom);
		}
		else
		{
			flyAnim.drawAtAnchor(window, anchorCx, anchorBottom);
		}
	}
	else
	{
		body.setFillColor(sf::Color(255, 120, 50));
		Projectile::draw(window);
	}
}

// --- Flying Tara bomb -------------------------------------------------------

FlyingTaraBombProjectile::FlyingTaraBombProjectile(float startX, float startY, float throwVelocityX, float throwVelocityY)
	: BallisticProjectile(startX, startY, throwVelocityX, throwVelocityY, 20, 115.0f, 0.78f, false)
{
	width = 36.0f;
	height = 22.0f;
	body.setSize(sf::Vector2f(width, height));
	body.setFillColor(sf::Color::Transparent);
	body.setOutlineColor(sf::Color::Transparent);
	body.setOutlineThickness(0.0f);
	visualScale = 3.2f;

	const sf::Texture* tex = VehicleTextureManager::instance().getTexture(VehicleTextureId::TaraRocket);
	static FrameRect fr;
	static bool ready = false;
	if (!ready && tex != nullptr)
	{
		fr = FrameRect{0, 0, 49, 10, 0, 0};
		ready = true;
	}
	if (tex != nullptr && ready)
	{
		bombAnim.setTexture(tex);
		bombAnim.setFrames(&fr, 1);
		bombAnim.setFrameTime(0.2f);
		bombAnim.setLoop(true);
		bombAnim.setScale(visualScale, visualScale);
		bombAnim.setFacingRight(true);
		bombAnim.reset();
	}
}

void FlyingTaraBombProjectile::draw(sf::RenderWindow& window)
{
	const sf::Texture* tex = VehicleTextureManager::instance().getTexture(VehicleTextureId::TaraRocket);
	if (tex != nullptr && bombAnim.getFrameCount() > 0)
	{
		float anchorCx = x + width * 0.5f;
		float anchorBottom = y + height;
		bombAnim.drawAtAnchor(window, anchorCx, anchorBottom);
	}
	else
	{
		BallisticProjectile::draw(window);
	}
}

// --- Enemy submarine torpedo ------------------------------------------------

EnemySubTorpedoProjectile::EnemySubTorpedoProjectile(float startX, float startY, float velocityX, float velocityY)
	: BallisticProjectile(startX, startY, velocityX, velocityY, 24, 110.0f, 0.0f, false)
{
	width = 48.0f;
	height = 12.0f;
	body.setSize(sf::Vector2f(width, height));
	body.setFillColor(sf::Color::Transparent);
	body.setOutlineColor(sf::Color::Transparent);
	body.setOutlineThickness(0.0f);
	visualScale = 3.0f;

	const sf::Texture* tex = VehicleTextureManager::instance().getTexture(VehicleTextureId::EnemySubBullet);
	static FrameRect fr = FrameRect{0, 0, 21, 2, 0, 0};
	if (tex != nullptr)
	{
		bulletAnim.setTexture(tex);
		bulletAnim.setFrames(&fr, 1);
		bulletAnim.setFrameTime(0.2f);
		bulletAnim.setLoop(true);
		bulletAnim.setScale(visualScale, visualScale);
		bulletAnim.setFacingRight(velocityX >= 0.0f);
		bulletAnim.reset();
	}
}

void EnemySubTorpedoProjectile::draw(sf::RenderWindow& window)
{
	const sf::Texture* tex = VehicleTextureManager::instance().getTexture(VehicleTextureId::EnemySubBullet);
	if (tex != nullptr && bulletAnim.getFrameCount() > 0)
	{
		float cx = velocityX >= 0.0f ? x + width : x;
		float anchorBottom = y + height * 0.5f + 9.0f;
		bool face = velocityX >= 0.0f;
		bulletAnim.setFacingRight(face);
		bulletAnim.drawAtAnchor(window, cx, anchorBottom);
	}
	else
	{
		BallisticProjectile::draw(window);
	}
}

// --- Straight enemy vehicle rocket -----------------------------------------

EnemyStraightRocketProjectile::EnemyStraightRocketProjectile(float startX, float startY, bool newFacingRight)
{
	damage = 24;
	lifeTime = 2.4f;
	width = 34.0f;
	height = 18.0f;
	explosive = false;
	blastRadius = 0.0f;
	facingRight = newFacingRight;
	setPlayerOwned(false);
	setPosition(startX, startY);
	setVelocity(facingRight ? 470.0f : -470.0f, 0.0f);

	body.setSize(sf::Vector2f(width, height));
	body.setFillColor(sf::Color(240, 100, 50));
	body.setOutlineThickness(0.0f);

	const sf::Texture* tex = loadEnemyVehicleRocketTexture() ? &enemyVehicleRocketTexture : nullptr;
	static FrameRect rocketFrames[4];
	static bool rocketGeoReady = false;
	if (!rocketGeoReady)
	{
		rocketFrames[0] = FrameRect{0, 0, 13, 14, 0, 0};
		rocketFrames[1] = FrameRect{13, 0, 13, 14, 0, 0};
		rocketFrames[2] = FrameRect{26, 0, 13, 14, 0, 0};
		rocketFrames[3] = FrameRect{39, 0, 14, 14, 0, 0};
		rocketGeoReady = true;
	}

	if (tex != nullptr && rocketGeoReady)
	{
		rocketAnim.setTexture(tex);
		rocketAnim.setFrames(rocketFrames, 4);
		rocketAnim.setFrameTime(0.08f);
		rocketAnim.setLoop(true);
		rocketAnim.setScale(2.0f, 2.0f);
		rocketAnim.setFacingRight(facingRight);
		rocketAnim.reset();
	}
}

void EnemyStraightRocketProjectile::update(float deltaTime)
{
	Projectile::update(deltaTime);
	rocketAnim.update(deltaTime);

	if (x + width < -200.0f || x > Constants::WORLD_WIDTH_LEVEL_3 + 2500.0f)
	{
		deactivate();
	}
}

void EnemyStraightRocketProjectile::draw(sf::RenderWindow& window)
{
	const sf::Texture* tex = loadEnemyVehicleRocketTexture() ? &enemyVehicleRocketTexture : nullptr;
	if (tex != nullptr && rocketAnim.getFrameCount() > 0)
	{
		rocketAnim.setFacingRight(facingRight);
		rocketAnim.drawAtAnchor(window, x + width * 0.5f, y + height);
	}
	else
	{
		Projectile::draw(window);
	}
}
