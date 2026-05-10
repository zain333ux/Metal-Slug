#include "Level.h"

#include "Constants.h"

#include <cmath>


using namespace std;
using namespace sf;

Level::Level(int newLevelNumber, float newWorldWidth, bool newCampaignGenerated)
{
	loaded = false;
	backgroundLoaded = false;
	fullBiomeBackground = false;
	collisionMaskLoaded = false;
	blocksLoaded = false;
	levelNumber = newLevelNumber;
	campaignGenerated = newCampaignGenerated;
	worldWidth = newWorldWidth;
	worldHeight = static_cast<float>(Constants::SCREEN_HEIGHT);
	if (worldWidth <= 0)
	{
		worldWidth = Constants::WORLD_WIDTH_LEVEL_1;
	}
	backgroundTextureRect = IntRect(0, 0, 0, 0);
	groundProfileSpacing = 4;
}

Level::~Level()
{
}

void Level::load()
{
	loadBackground();

	if (grassTexture.loadFromFile("Sprites/blocks/grass_block_side.png") &&
		dirtTexture.loadFromFile("Sprites/blocks/dirt.png") &&
		stoneTexture.loadFromFile("Sprites/blocks/stone.png"))
	{
		blocksLoaded = true;
	}

	if (!campaignGenerated)
	{
		buildSurvivalPlatforms();
	}

	loaded = true;
}

bool Level::loadBackground()
{
	if (!campaignGenerated && levelNumber >= 1 && levelNumber <= 4 &&
		loadFullBiomeBackground("Sprites/Clean/3biomes.png", "Sprites/Clean/CollisionFinal.png"))
	{
		return true;
	}

	if (!campaignGenerated && levelNumber >= 1 && levelNumber <= 3 &&
		loadFullBiomeBackground("Sprites/Clean/full_biome-terrain.png", "Sprites/Clean/full_biome-collision.png"))
	{
		return true;
	}

	if (backgroundTexture.loadFromFile("Sprites/Clean/background_level1.png"))
	{
		Vector2u textureSize = backgroundTexture.getSize();
		backgroundTextureRect = IntRect(0, 0, static_cast<int>(textureSize.x), static_cast<int>(textureSize.y));
		backgroundSprite.setTexture(backgroundTexture);
		backgroundSprite.setTextureRect(backgroundTextureRect);
		backgroundLoaded = true;
		return true;
	}

	return false;
}

bool Level::loadFullBiomeBackground(const char* fileName, const char* collisionMaskFileName)
{
	Image image;
	if (!image.loadFromFile(fileName))
	{
		return false;
	}

	int contentTop = 0;
	int contentHeight = static_cast<int>(image.getSize().y);
	findImageContentY(image, contentTop, contentHeight);
	if (!backgroundTexture.loadFromImage(image))
	{
		return false;
	}

	fullBiomeBackground = true;
	worldHeight = Constants::WORLD_HEIGHT;
	setupFullBiomeRect(contentTop, contentHeight);
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setTextureRect(backgroundTextureRect);
	backgroundLoaded = true;
	loadCollisionMask(collisionMaskFileName);
	return true;
}

bool Level::loadCollisionMask(const char* fileName)
{
	if (!collisionMaskImage.loadFromFile(fileName))
	{
		collisionMaskLoaded = false;
		return false;
	}

	Vector2u maskSize = collisionMaskImage.getSize();
	Vector2u backgroundSize = backgroundTexture.getSize();
	if (maskSize.x != backgroundSize.x || maskSize.y != backgroundSize.y)
	{
		collisionMaskLoaded = false;
		return false;
	}

	collisionMaskLoaded = true;
	buildGroundProfileFromMask();
	buildWaterBlocksFromMask();
	return groundProfile.getSize() > 0;
}

void Level::setupFullBiomeRect(int contentTop, int contentHeight)
{
	Vector2u textureSize = backgroundTexture.getSize();
	int sourceWidth = static_cast<int>(textureSize.x);
	int sourceHeight = static_cast<int>(textureSize.y);
	if (sourceWidth <= 0 || sourceHeight <= 0)
	{
		backgroundTextureRect = IntRect(0, 0, 0, 0);
		return;
	}

	float sectionStart = 0;
	float sectionWidth = Constants::WORLD_WIDTH_LEVEL_1;
	if (levelNumber == 4)
	{
		sectionStart = 0;
		sectionWidth = Constants::FULL_BIOME_WORLD_WIDTH;
	}
	else if (levelNumber == 2)
	{
		sectionStart = Constants::WORLD_WIDTH_LEVEL_1;
		sectionWidth = Constants::WORLD_WIDTH_LEVEL_2;
	}
	else if (levelNumber == 3)
	{
		sectionStart = Constants::WORLD_WIDTH_LEVEL_1 + Constants::WORLD_WIDTH_LEVEL_2;
		sectionWidth = Constants::WORLD_WIDTH_LEVEL_3;
	}

	int left = static_cast<int>(static_cast<float>(sourceWidth) * sectionStart / Constants::FULL_BIOME_WORLD_WIDTH + 0.5f);
	int width = static_cast<int>(static_cast<float>(sourceWidth) * sectionWidth / Constants::FULL_BIOME_WORLD_WIDTH + 0.5f);
	if (left < 0)
	{
		left = 0;
	}
	if (width < 1)
	{
		width = sourceWidth;
	}
	if (left + width > sourceWidth)
	{
		width = sourceWidth - left;
	}

	if (contentTop < 0)
	{
		contentTop = 0;
	}
	if (contentHeight < 1)
	{
		contentHeight = sourceHeight;
	}
	if (contentTop + contentHeight > sourceHeight)
	{
		contentHeight = sourceHeight - contentTop;
	}

	backgroundTextureRect = IntRect(left, contentTop, width, contentHeight);
}

void Level::findImageContentY(const Image& image, int& contentTop, int& contentHeight) const
{
	Vector2u imageSize = image.getSize();
	int minY = static_cast<int>(imageSize.y);
	int maxY = -1;

	for (unsigned int y = 0; y < imageSize.y; y += 2)
	{
		for (unsigned int x = 0; x < imageSize.x; x += 8)
		{
			Color pixel = image.getPixel(x, y);
			bool almostWhite = pixel.r > 245 && pixel.g > 245 && pixel.b > 245;
			bool almostBlack = pixel.r < 8 && pixel.g < 8 && pixel.b < 8;
			bool emptyPixel = pixel.a == 0 || almostWhite || almostBlack;
			if (!emptyPixel)
			{
				if (static_cast<int>(y) < minY)
				{
					minY = static_cast<int>(y);
				}
				if (static_cast<int>(y) > maxY)
				{
					maxY = static_cast<int>(y);
				}
			}
		}
	}

	if (maxY < minY)
	{
		contentTop = 0;
		contentHeight = static_cast<int>(imageSize.y);
		return;
	}

	contentTop = minY;
	contentHeight = maxY - minY + 1;
}

void Level::buildGroundProfileFromMask()
{
	groundProfile.clear();

	if (!collisionMaskLoaded || backgroundTextureRect.width <= 0 || backgroundTextureRect.height <= 0)
	{
		return;
	}

	int sampleCount = static_cast<int>(worldWidth / static_cast<float>(groundProfileSpacing)) + 2;
	float previousGroundY = static_cast<float>(Constants::GROUND_Y);
	int startY = backgroundTextureRect.top;
	int endY = backgroundTextureRect.top + backgroundTextureRect.height;
	int maskHeight = static_cast<int>(collisionMaskImage.getSize().y);
	if (endY > maskHeight)
	{
		endY = maskHeight;
	}

	for (int sample = 0; sample < sampleCount; sample += 1)
	{
		float worldX = static_cast<float>(sample * groundProfileSpacing);
		if (worldX > worldWidth)
		{
			worldX = worldWidth;
		}

		int sourceX = getSourceXFromWorldX(worldX);
		float groundY = previousGroundY;
		bool foundGround = false;
		for (int sourceY = endY - 1; sourceY >= startY; sourceY -= 1)
		{
			if (isGroundMaskPixel(collisionMaskImage.getPixel(static_cast<unsigned int>(sourceX), static_cast<unsigned int>(sourceY))))
			{
				groundY = getWorldYFromSourceY(sourceY);
				foundGround = true;
				break;
			}
		}

		if (foundGround)
		{
			previousGroundY = groundY;
		}
		groundProfile.pushBack(groundY);
	}
}

bool Level::isGroundMaskPixel(Color pixel) const
{
	return pixel.a > 0 && pixel.g > 180 && pixel.r < 120 && pixel.b < 120;
}

bool Level::isWaterMaskPixel(Color pixel) const
{
	return pixel.a > 0 && pixel.b > 180 && pixel.r < 120 && pixel.g < 120;
}

int Level::getSourceXFromWorldX(float worldX) const
{
	if (worldX < 0)
	{
		worldX = 0;
	}
	if (worldX > worldWidth)
	{
		worldX = worldWidth;
	}

	int sourceX = backgroundTextureRect.left;
	if (worldWidth > 0 && backgroundTextureRect.width > 0)
	{
		sourceX += static_cast<int>(worldX * static_cast<float>(backgroundTextureRect.width) / worldWidth);
	}

	int maxX = backgroundTextureRect.left + backgroundTextureRect.width - 1;
	if (sourceX > maxX)
	{
		sourceX = maxX;
	}
	if (sourceX < backgroundTextureRect.left)
	{
		sourceX = backgroundTextureRect.left;
	}

	return sourceX;
}

int Level::getSourceYFromWorldY(float worldY) const
{
	if (worldY < 0)
	{
		worldY = 0;
	}
	if (worldY > worldHeight)
	{
		worldY = worldHeight;
	}

	int sourceY = backgroundTextureRect.top;
	if (worldHeight > 0 && backgroundTextureRect.height > 0)
	{
		sourceY += static_cast<int>(worldY * static_cast<float>(backgroundTextureRect.height) / worldHeight);
	}

	int maxY = backgroundTextureRect.top + backgroundTextureRect.height - 1;
	if (sourceY > maxY)
	{
		sourceY = maxY;
	}
	if (sourceY < backgroundTextureRect.top)
	{
		sourceY = backgroundTextureRect.top;
	}

	return sourceY;
}

float Level::getWorldYFromSourceY(int sourceY) const
{
	if (backgroundTextureRect.height <= 0)
	{
		return static_cast<float>(Constants::GROUND_Y);
	}

	float relativeY = static_cast<float>(sourceY - backgroundTextureRect.top) / static_cast<float>(backgroundTextureRect.height);
	if (relativeY < 0)
	{
		relativeY = 0;
	}
	if (relativeY > 1)
	{
		relativeY = 1;
	}

	return relativeY * worldHeight;
}

char Level::getCollisionTileAt(float worldX, float worldY) const
{
	if (!collisionMaskLoaded || backgroundTextureRect.width <= 0 || backgroundTextureRect.height <= 0)
	{
		return ' ';
	}

	int sourceX = getSourceXFromWorldX(worldX);
	int sourceY = getSourceYFromWorldY(worldY);
	Color pixel = collisionMaskImage.getPixel(static_cast<unsigned int>(sourceX), static_cast<unsigned int>(sourceY));
	if (isGroundMaskPixel(pixel))
	{
		return 'g';
	}
	if (isWaterMaskPixel(pixel))
	{
		return 'w';
	}

	return ' ';
}

bool Level::isWaterInBounds(float left, float right, float top, float bottom) const
{
	if (!collisionMaskLoaded)
	{
		return false;
	}

	if (right < left)
	{
		float temp = left;
		left = right;
		right = temp;
	}
	if (bottom < top)
	{
		float temp = top;
		top = bottom;
		bottom = temp;
	}

	float centerX = (left + right) * 0.5f;
	float centerY = (top + bottom) * 0.5f;
	float sampleXs[3];
	sampleXs[0] = left + 4;
	sampleXs[1] = centerX;
	sampleXs[2] = right - 4;

	float sampleYs[4];
	sampleYs[0] = top + 4;
	sampleYs[1] = centerY;
	sampleYs[2] = bottom - 4;
	sampleYs[3] = bottom + 8;

	for (int xIndex = 0; xIndex < 3; xIndex += 1)
	{
		for (int yIndex = 0; yIndex < 4; yIndex += 1)
		{
			if (getCollisionTileAt(sampleXs[xIndex], sampleYs[yIndex]) == 'w')
			{
				return true;
			}
		}
	}

	return false;
}

float Level::getWaterSurfaceYAt(float x) const
{
	if (!collisionMaskLoaded || backgroundTextureRect.width <= 0 || backgroundTextureRect.height <= 0)
	{
		return worldHeight + 1;
	}

	int sourceX = getSourceXFromWorldX(x);
	int startY = backgroundTextureRect.top;
	int endY = backgroundTextureRect.top + backgroundTextureRect.height;
	int maskHeight = static_cast<int>(collisionMaskImage.getSize().y);
	if (endY > maskHeight)
	{
		endY = maskHeight;
	}

	for (int sourceY = startY; sourceY < endY; sourceY += 1)
	{
		if (isWaterMaskPixel(collisionMaskImage.getPixel(static_cast<unsigned int>(sourceX), static_cast<unsigned int>(sourceY))))
		{
			return getWorldYFromSourceY(sourceY);
		}
	}

	return worldHeight + 1;
}

bool Level::isAquaticBiome(float x, float y) const
{
	float waterY = getWaterSurfaceYAt(x);
	return waterY <= worldHeight && y >= waterY + 4;
}

bool Level::isAerialBiome(float x, float y) const
{
	float groundY = getMainGroundYAt(x);
	return groundY < worldHeight * 0.60f && y < groundY - 80;
}

bool Level::isPlainsBiome(float x, float y) const
{
	return !isAquaticBiome(x, y) && !isAerialBiome(x, y);
}

bool Level::hasWaterInRect(const FloatRect& rect) const
{
	if (!collisionMaskLoaded)
	{
		return false;
	}

	float sampleXs[3];
	sampleXs[0] = rect.left + 4;
	sampleXs[1] = rect.left + rect.width * 0.5f;
	sampleXs[2] = rect.left + rect.width - 4;

	float sampleYs[3];
	sampleYs[0] = rect.top + 4;
	sampleYs[1] = rect.top + rect.height * 0.5f;
	sampleYs[2] = rect.top + rect.height - 4;

	for (int xIndex = 0; xIndex < 3; xIndex += 1)
	{
		for (int yIndex = 0; yIndex < 3; yIndex += 1)
		{
			if (getCollisionTileAt(sampleXs[xIndex], sampleYs[yIndex]) == 'w')
			{
				return true;
			}
		}
	}

	return false;
}

void Level::buildWaterBlocksFromMask()
{
	waterBlocks.clear();
	if (!collisionMaskLoaded || backgroundTextureRect.width <= 0 || backgroundTextureRect.height <= 0)
	{
		return;
	}

	float blockSize = static_cast<float>(Constants::TILE_SIZE);
	for (float y = 0; y < worldHeight; y += blockSize)
	{
		float blockHeight = blockSize;
		if (y + blockHeight > worldHeight)
		{
			blockHeight = worldHeight - y;
		}

		for (float x = 0; x < worldWidth; x += blockSize)
		{
			float blockWidth = blockSize;
			if (x + blockWidth > worldWidth)
			{
				blockWidth = worldWidth - x;
			}

			FloatRect block(x, y, blockWidth, blockHeight);
			if (hasWaterInRect(block))
			{
				waterBlocks.pushBack(block);
			}
		}
	}
}

float Level::getMaskedGroundYAt(float x) const
{
	if (!collisionMaskLoaded || groundProfile.getSize() == 0)
	{
		return static_cast<float>(Constants::GROUND_Y);
	}

	if (x < 0)
	{
		x = 0;
	}
	if (x > worldWidth)
	{
		x = worldWidth;
	}

	float samplePosition = x / static_cast<float>(groundProfileSpacing);
	int leftIndex = static_cast<int>(samplePosition);
	int rightIndex = leftIndex + 1;
	if (leftIndex < 0)
	{
		leftIndex = 0;
	}
	if (rightIndex >= groundProfile.getSize())
	{
		rightIndex = groundProfile.getSize() - 1;
	}

	float blend = samplePosition - static_cast<float>(leftIndex);
	float leftY = groundProfile.get(leftIndex);
	float rightY = groundProfile.get(rightIndex);
	return leftY + (rightY - leftY) * blend;
}

float Level::getMaskedLandingY(float left, float right, float previousBottom, float currentBottom) const
{
	if (!collisionMaskLoaded || backgroundTextureRect.width <= 0 || backgroundTextureRect.height <= 0)
	{
		return getMaskedGroundYAt((left + right) * 0.5f);
	}

	float centerX = (left + right) * 0.5f;
	float landingY = getMaskedGroundYAt(centerX);
	if (currentBottom < previousBottom)
	{
		return landingY;
	}

	float scanTop = previousBottom - 10;
	float scanBottom = currentBottom + 8;
	if (scanTop > scanBottom)
	{
		float temp = scanTop;
		scanTop = scanBottom;
		scanBottom = temp;
	}

	if (scanTop < 0)
	{
		scanTop = 0;
	}
	if (scanBottom > worldHeight)
	{
		scanBottom = worldHeight;
	}

	float sampleXs[3];
	sampleXs[0] = left + 6;
	sampleXs[1] = centerX;
	sampleXs[2] = right - 6;

	bool foundSurface = false;
	float bestSurfaceY = landingY;
	for (int i = 0; i < 3; i += 1)
	{
		int sourceX = getSourceXFromWorldX(sampleXs[i]);
		for (float worldY = scanTop; worldY <= scanBottom; worldY += 2)
		{
			float relativeY = worldY / worldHeight;
			if (relativeY < 0)
			{
				relativeY = 0;
			}
			if (relativeY > 1)
			{
				relativeY = 1;
			}

			int sourceY = backgroundTextureRect.top + static_cast<int>(relativeY * static_cast<float>(backgroundTextureRect.height));
			int maxY = backgroundTextureRect.top + backgroundTextureRect.height - 1;
			if (sourceY > maxY)
			{
				sourceY = maxY;
			}
			if (sourceY < backgroundTextureRect.top)
			{
				sourceY = backgroundTextureRect.top;
			}

			if (isGroundMaskPixel(collisionMaskImage.getPixel(static_cast<unsigned int>(sourceX), static_cast<unsigned int>(sourceY))))
			{
				float surfaceY = getWorldYFromSourceY(sourceY);
				if (!foundSurface || surfaceY < bestSurfaceY)
				{
					bestSurfaceY = surfaceY;
				}
				foundSurface = true;
				break;
			}
		}
	}

	if (foundSurface)
	{
		return bestSurfaceY;
	}

	return landingY;
}

void Level::update(float deltaTime)
{
	(void)deltaTime;
}

void Level::draw(RenderWindow& window)
{
	if (backgroundLoaded)
	{
		if (backgroundTextureRect.width > 0 && backgroundTextureRect.height > 0)
		{
			backgroundSprite.setScale(worldWidth / static_cast<float>(backgroundTextureRect.width),
				worldHeight / static_cast<float>(backgroundTextureRect.height));
		}
		window.draw(backgroundSprite);
	}

	drawWaterBlocks(window);

	if (blocksLoaded && !fullBiomeBackground)
	{
		if (campaignGenerated)
		{
			drawGeneratedTerrain(window);
		}
		else
		{
			drawTileRow(window, grassTexture, Constants::GROUND_Y);
			drawTileRow(window, dirtTexture, Constants::GROUND_Y + Constants::TILE_SIZE);
			drawTileRow(window, stoneTexture, Constants::GROUND_Y + Constants::TILE_SIZE * 2);
		}

	}
	else if (!fullBiomeBackground)
	{
		RectangleShape ground;
		ground.setPosition(0, static_cast<float>(Constants::GROUND_Y));
		ground.setSize(Vector2f(static_cast<float>(Constants::SCREEN_WIDTH), static_cast<float>(Constants::SCREEN_HEIGHT - Constants::GROUND_Y)));
		ground.setFillColor(Color(90, 70, 45, 120));
		window.draw(ground);
	}

	if (blocksLoaded)
	{
		for (int i = 0; i < platforms.getSize(); i += 1)
		{
			drawPlatform(window, platforms.get(i));
		}
	}
}

void Level::drawWaterBlocks(RenderWindow& window)
{
	(void)window;
	return;
<<<<<<< HEAD

	if (waterBlocks.getSize() == 0)
	{
		return;
	}

	RectangleShape waterBlock;
	waterBlock.setFillColor(Color(30, 120, 220, 24));
	for (int i = 0; i < waterBlocks.getSize(); i += 1)
	{
		const FloatRect& block = waterBlocks.get(i);
		waterBlock.setPosition(block.left, block.top);
		waterBlock.setSize(Vector2f(block.width, block.height));
		window.draw(waterBlock);
	}
=======
>>>>>>> 6d77c54b1816a0bae93ea8c1ad344f5e135692cd
}

void Level::drawTileRow(RenderWindow& window, Texture& texture, int y)
{
	Sprite tile;
	tile.setTexture(texture);

	Vector2u textureSize = texture.getSize();
	if (textureSize.x > 0 && textureSize.y > 0)
	{
		float scaleX = static_cast<float>(Constants::TILE_SIZE) / static_cast<float>(textureSize.x);
		float scaleY = static_cast<float>(Constants::TILE_SIZE) / static_cast<float>(textureSize.y);
		tile.setScale(scaleX, scaleY);
	}

	for (int x = 0; x < static_cast<int>(worldWidth); x += Constants::TILE_SIZE)
	{
		tile.setPosition(static_cast<float>(x), static_cast<float>(y));
		window.draw(tile);
	}
}

void Level::drawGeneratedTerrain(RenderWindow& window)
{
	RectangleShape column;
	column.setSize(Vector2f(static_cast<float>(Constants::TILE_SIZE), 1));

	for (int x = 0; x < static_cast<int>(worldWidth); x += Constants::TILE_SIZE)
	{
		float surfaceY = getGeneratedSurfaceY(static_cast<float>(x));
		column.setPosition(static_cast<float>(x), surfaceY);
		column.setSize(Vector2f(static_cast<float>(Constants::TILE_SIZE), static_cast<float>(Constants::SCREEN_HEIGHT) - surfaceY));
		column.setFillColor(Color(68, 88, 64));
		window.draw(column);

		Sprite grass;
		grass.setTexture(grassTexture);
		Vector2u textureSize = grassTexture.getSize();
		if (textureSize.x > 0 && textureSize.y > 0)
		{
			grass.setScale(static_cast<float>(Constants::TILE_SIZE) / static_cast<float>(textureSize.x),
				static_cast<float>(Constants::TILE_SIZE) / static_cast<float>(textureSize.y));
		}
		grass.setPosition(static_cast<float>(x), surfaceY);
		window.draw(grass);
	}
}

void Level::drawPlatform(RenderWindow& window, const FloatRect& platform)
{
	RectangleShape back;
	back.setPosition(platform.left, platform.top);
	back.setSize(Vector2f(platform.width, platform.height));
	back.setFillColor(Color(60, 48, 34));
	back.setOutlineColor(Color(20, 20, 20));
	back.setOutlineThickness(2);
	window.draw(back);

	if (!blocksLoaded)
	{
		return;
	}

	Sprite tile;
	tile.setTexture(grassTexture);
	Vector2u textureSize = grassTexture.getSize();
	if (textureSize.x > 0 && textureSize.y > 0)
	{
		tile.setScale(static_cast<float>(Constants::TILE_SIZE) / static_cast<float>(textureSize.x),
			static_cast<float>(Constants::TILE_SIZE) / static_cast<float>(textureSize.y));
	}

	for (int x = static_cast<int>(platform.left); x < static_cast<int>(platform.left + platform.width); x += Constants::TILE_SIZE)
	{
		tile.setPosition(static_cast<float>(x), platform.top);
		window.draw(tile);
	}
}

void Level::buildSurvivalPlatforms()
{
	float platformYOffset = fullBiomeBackground ? 260 : 0;

	if (levelNumber == 1)
	{
		platforms.pushBack(FloatRect(520, 690 + platformYOffset, 420, 34));
		platforms.pushBack(FloatRect(1080, 650 + platformYOffset, 380, 34));
		platforms.pushBack(FloatRect(1640, 615 + platformYOffset, 420, 34));
		platforms.pushBack(FloatRect(2260, 655 + platformYOffset, 440, 34));
		platforms.pushBack(FloatRect(2920, 700 + platformYOffset, 420, 34));
		return;
	}

	if (levelNumber == 2)
	{
		platforms.pushBack(FloatRect(460, 700 + platformYOffset, 360, 34));
		platforms.pushBack(FloatRect(960, 660 + platformYOffset, 340, 34));
		platforms.pushBack(FloatRect(1460, 620 + platformYOffset, 360, 34));
		platforms.pushBack(FloatRect(2040, 665 + platformYOffset, 420, 34));
		platforms.pushBack(FloatRect(2680, 615 + platformYOffset, 380, 34));
		platforms.pushBack(FloatRect(3340, 675 + platformYOffset, 460, 34));
		platforms.pushBack(FloatRect(3920, 705 + platformYOffset, 360, 34));
		return;
	}

	if (levelNumber == 3)
	{
		platforms.pushBack(FloatRect(460, 705 + platformYOffset, 340, 34));
		platforms.pushBack(FloatRect(900, 660 + platformYOffset, 320, 34));
		platforms.pushBack(FloatRect(1340, 620 + platformYOffset, 340, 34));
		platforms.pushBack(FloatRect(1820, 575 + platformYOffset, 340, 34));
		platforms.pushBack(FloatRect(2340, 625 + platformYOffset, 380, 34));
		platforms.pushBack(FloatRect(2920, 680 + platformYOffset, 420, 34));
		platforms.pushBack(FloatRect(3540, 635 + platformYOffset, 360, 34));
		platforms.pushBack(FloatRect(4140, 690 + platformYOffset, 420, 34));
		platforms.pushBack(FloatRect(4700, 710 + platformYOffset, 360, 34));
		return;
	}

	platforms.pushBack(FloatRect(520, 690 + platformYOffset, 420, 34));
	platforms.pushBack(FloatRect(1220, 650 + platformYOffset, 420, 34));
	platforms.pushBack(FloatRect(1880, 700 + platformYOffset, 420, 34));
}

void Level::extendIfNeeded(float playerX)
{
	if (!campaignGenerated)
	{
		return;
	}

	if (playerX > worldWidth - 1800)
	{
		worldWidth += 2800;
	}
}

float Level::getWorldWidth() const
{
	return worldWidth;
}

float Level::getWorldHeight() const
{
	return worldHeight;
}

float Level::getRightBoundary() const
{
	return worldWidth - 120;
}

float Level::getLandingY(float left, float right, float previousBottom, float currentBottom) const
{
	float landingY = getMainGroundYAt((left + right) * 0.5f);
	if (collisionMaskLoaded)
	{
		landingY = getMaskedLandingY(left, right, previousBottom, currentBottom);
	}

	for (int i = 0; i < platforms.getSize(); i += 1)
	{
		const FloatRect& platform = platforms.get(i);
		bool overlapsX = right > platform.left + 6 && left < platform.left + platform.width - 6;
		bool crossedTop = previousBottom <= platform.top + 24 && currentBottom >= platform.top;
		if (overlapsX && crossedTop && platform.top < landingY)
		{
			landingY = platform.top;
		}
	}

	return landingY;
}

float Level::getGroundYAt(float x) const
{
	float ground = getMainGroundYAt(x);
	for (int i = 0; i < platforms.getSize(); i += 1)
	{
		const FloatRect& platform = platforms.get(i);
		if (x >= platform.left && x <= platform.left + platform.width && platform.top < ground)
		{
			ground = platform.top;
		}
	}

	return ground;
}

float Level::getMainGroundYAt(float x) const
{
	if (collisionMaskLoaded)
	{
		return getMaskedGroundYAt(x);
	}

	if (campaignGenerated)
	{
		return getGeneratedSurfaceY(x);
	}

	return static_cast<float>(Constants::GROUND_Y);
}

float Level::getGeneratedSurfaceY(float x) const
{
	float wave = sin(x * 0.006f) * 60 + sin(x * 0.017f) * 24 + sin(x * 0.011f) * 18;
	float y = static_cast<float>(Constants::GROUND_Y) - 65 + wave;
	if (y < 500)
	{
		y = 500;
	}
	if (y > static_cast<float>(Constants::GROUND_Y))
	{
		y = static_cast<float>(Constants::GROUND_Y);
	}
	return y;
}