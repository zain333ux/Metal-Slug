#include "DeveloperMode.h"

#include <SFML/Window/Keyboard.hpp>


using namespace std;
using namespace sf;

bool DeveloperMode::enabled = false;
int DeveloperMode::pressCount = 0;
float DeveloperMode::pressTimer = 0;
bool DeveloperMode::previousKeyDown = false;

void DeveloperMode::update(float deltaTime)
{
	if (pressTimer > 0)
	{
		pressTimer -= deltaTime;
	}
	else
	{
		pressCount = 0;
	}

	bool keyDown = Keyboard::isKeyPressed(Keyboard::F1);
	if (keyDown && !previousKeyDown)
	{
		if (pressTimer <= 0)
		{
			pressCount = 0;
		}

		pressCount += 1;
		pressTimer = 2;

		if (pressCount >= 3)
		{
			enabled = !enabled;
			pressCount = 0;
			pressTimer = 0;
		}
	}

	previousKeyDown = keyDown;
}

bool DeveloperMode::isEnabled()
{
	return enabled;
}