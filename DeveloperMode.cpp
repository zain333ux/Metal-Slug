#include "DeveloperMode.h"

#include <SFML/Window/Keyboard.hpp>

bool DeveloperMode::enabled = false;
int DeveloperMode::pressCount = 0;
float DeveloperMode::pressTimer = 0.0f;
bool DeveloperMode::previousKeyDown = false;

void DeveloperMode::update(float deltaTime)
{
	if (pressTimer > 0.0f)
	{
		pressTimer -= deltaTime;
	}
	else
	{
		pressCount = 0;
	}

	bool keyDown = sf::Keyboard::isKeyPressed(sf::Keyboard::F1);
	if (keyDown && !previousKeyDown)
	{
		if (pressTimer <= 0.0f)
		{
			pressCount = 0;
		}

		pressCount += 1;
		pressTimer = 2.0f;

		if (pressCount >= 3)
		{
			enabled = !enabled;
			pressCount = 0;
			pressTimer = 0.0f;
		}
	}

	previousKeyDown = keyDown;
}

bool DeveloperMode::isEnabled()
{
	return enabled;
}
