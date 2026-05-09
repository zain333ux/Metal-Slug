#pragma once

#include <SFML/Graphics.hpp>

struct FrameRect
{
	int x;
	int y;
	int w;
	int h;
	int offsetX;
	int offsetY;
};

/// Time-based sprite sheet animation without setOrigin; positions use logical anchor + per-frame offsets.
class SpriteAnimation
{
public:
	static const int MAX_FRAMES = 48;

private:
	const sf::Texture* texturePtr;
	sf::Sprite sprite;
	FrameRect frames[MAX_FRAMES];
	int frameCount;
	int currentFrameIndex;
	float frameTime;
	float elapsed;
	bool loopAnimation;
	bool finishedAnimation;
	float scaleX;
	float scaleY;
	bool playBackward;
	bool facingRight;
	sf::Color tint;

public:
	SpriteAnimation();

	void setTexture(const sf::Texture* texture);
	void setFrames(const FrameRect* rects, int count);
	void setFrameTime(float seconds);
	void setLoop(bool loop);
	void setPlayBackward(bool backward);
	void setScale(float sx, float sy);
	void setFacingRight(bool right);
	void setTint(const sf::Color& color);
	void reset();

	void update(float dt);
	void applyToSprite();
	void draw(sf::RenderWindow& window);

	/// Anchor: bottom-centre of logical hitbox in world space (pixels).
	void drawAtAnchor(sf::RenderWindow& window, float anchorCentreX, float anchorBottomY);

	bool isFinished() const;
	void setFinished(bool value);
	int getFrameCount() const;
	int getCurrentFrameIndex() const;
	int getCurrentFrameWidth() const;
	int getCurrentFrameHeight() const;
};
