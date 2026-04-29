#pragma once

class Constants
{
public:
	static const int SCREEN_WIDTH = 1600;
	static const int SCREEN_HEIGHT = 900;
	static const int FRAME_LIMIT = 60;
	static const int TILE_SIZE = 64;
	static const int GROUND_Y = 760;
	static const int SOLDIER_STATE_IDLE = 0;
	static const int SOLDIER_STATE_RUNNING = 1;
	static const int SOLDIER_STATE_JUMPING = 2;
	static const int SOLDIER_STATE_FALLING = 3;
	static const int PLAYER_ANIM_IDLE = 0;
	static const int PLAYER_ANIM_RUN = 1;
	static const int PLAYER_ANIM_JUMP = 2;
	static const int PLAYER_ANIM_FIRE = 3;
	static const int PLAYER_ANIM_MELEE = 4;
	static const int PLAYER_FRAME_SIZE = 96;

	static constexpr float GRAVITY = 1800.0f;
	static constexpr float PLAYER_MOVE_SPEED = 330.0f;
	static constexpr float PLAYER_JUMP_SPEED = 720.0f;
	static constexpr float ENEMY_MOVE_SPEED = 120.0f;
	static constexpr float ENEMY_STOP_DISTANCE = 95.0f;
	static constexpr float BULLET_SPEED = 850.0f;
	static constexpr float PLAYER_FIRE_COOLDOWN = 0.22f;
};
