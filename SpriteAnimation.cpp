#include "SpriteAnimation.h"

SpriteAnimation::SpriteAnimation()
{
	texturePtr = nullptr;
	frameCount = 0;
	currentFrameIndex = 0;
	frameTime = 0.12f;
	elapsed = 0.0f;
	loopAnimation = true;
	finishedAnimation = false;
	scaleX = 1.0f;
	scaleY = 1.0f;
	playBackward = false;
	facingRight = true;
	tint = sf::Color::White;
}

void SpriteAnimation::setTexture(const sf::Texture* texture)
{
	texturePtr = texture;
	if (texturePtr != nullptr)
	{
		sprite.setTexture(*texturePtr);
	}
}

void SpriteAnimation::setFrames(const FrameRect* rects, int count)
{
	frameCount = count;
	if (frameCount > MAX_FRAMES)
	{
		frameCount = MAX_FRAMES;
	}
	for (int i = 0; i < frameCount; i += 1)
	{
		frames[i] = rects[i];
	}
	currentFrameIndex = 0;
	finishedAnimation = false;
	elapsed = 0.0f;
}

void SpriteAnimation::setFrameTime(float seconds)
{
	if (seconds > 0.0f)
	{
		frameTime = seconds;
	}
}

void SpriteAnimation::setLoop(bool loop)
{
	loopAnimation = loop;
}

void SpriteAnimation::setPlayBackward(bool backward)
{
	playBackward = backward;
}

void SpriteAnimation::setScale(float sx, float sy)
{
	scaleX = sx;
	scaleY = sy;
	if (scaleX <= 0.0f)
	{
		scaleX = 0.01f;
	}
	if (scaleY <= 0.0f)
	{
		scaleY = 0.01f;
	}
}

void SpriteAnimation::setFacingRight(bool right)
{
	facingRight = right;
}

void SpriteAnimation::setTint(const sf::Color& color)
{
	tint = color;
}

void SpriteAnimation::reset()
{
	currentFrameIndex = playBackward ? (frameCount > 0 ? frameCount - 1 : 0) : 0;
	elapsed = 0.0f;
	finishedAnimation = false;
}

void SpriteAnimation::update(float dt)
{
	if (texturePtr == nullptr || frameCount <= 0 || finishedAnimation)
	{
		return;
	}

	elapsed += dt;
	if (elapsed < frameTime)
	{
		return;
	}

	elapsed = 0.0f;

	if (playBackward)
	{
		currentFrameIndex -= 1;
		if (currentFrameIndex < 0)
		{
			if (loopAnimation)
			{
				currentFrameIndex = frameCount - 1;
			}
			else
			{
				currentFrameIndex = 0;
				finishedAnimation = true;
			}
		}
	}
	else
	{
		currentFrameIndex += 1;
		if (currentFrameIndex >= frameCount)
		{
			if (loopAnimation)
			{
				currentFrameIndex = 0;
			}
			else
			{
				currentFrameIndex = frameCount - 1;
				finishedAnimation = true;
			}
		}
	}
}

void SpriteAnimation::applyToSprite()
{
	if (texturePtr == nullptr || frameCount <= 0)
	{
		return;
	}

	const FrameRect& fr = frames[currentFrameIndex];
	sprite.setTexture(*texturePtr);
	sprite.setTextureRect(sf::IntRect(fr.x, fr.y, fr.w, fr.h));
	sprite.setColor(tint);
}

void SpriteAnimation::draw(sf::RenderWindow& window)
{
	if (texturePtr == nullptr || frameCount <= 0)
	{
		return;
	}

	applyToSprite();
	window.draw(sprite);
}

void SpriteAnimation::drawAtAnchor(sf::RenderWindow& window, float anchorCentreX, float anchorBottomY)
{
	if (texturePtr == nullptr || frameCount <= 0)
	{
		return;
	}

	applyToSprite();
	const FrameRect& fr = frames[currentFrameIndex];
	float fw = static_cast<float>(fr.w);
	float fh = static_cast<float>(fr.h);
	float ox = static_cast<float>(fr.offsetX);
	float oy = static_cast<float>(fr.offsetY);

	float drawY = anchorBottomY - fh * scaleY + oy * scaleY;
	float drawX = anchorCentreX - fw * scaleX * 0.5f + ox * scaleX;

	if (facingRight)
	{
		sprite.setScale(scaleX, scaleY);
		sprite.setPosition(drawX, drawY);
	}
	else
	{
		sprite.setScale(-scaleX, scaleY);
		sprite.setPosition(drawX + fw * scaleX, drawY);
	}

	window.draw(sprite);
}

bool SpriteAnimation::isFinished() const
{
	return finishedAnimation;
}

void SpriteAnimation::setFinished(bool value)
{
	finishedAnimation = value;
}

int SpriteAnimation::getFrameCount() const
{
	return frameCount;
}

