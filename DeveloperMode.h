#pragma once

class DeveloperMode
{
private:
	static bool enabled;
	static int pressCount;
	static float pressTimer;
	static bool previousKeyDown;

public:
	static void update(float deltaTime);
	static bool isEnabled();
};
