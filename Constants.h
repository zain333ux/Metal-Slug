#pragma once

class Constants
{
public:
	static const int SCREEN_WIDTH = 1600;
	static const int SCREEN_HEIGHT = 900;
	static const int FRAME_LIMIT = 60;
	static const int TILE_SIZE = 64;
	static const int GROUND_Y = 760;
	static const int SURVIVAL_LEVEL_COUNT = 3;
	static const int SOLDIER_STATE_IDLE = 0;
	static const int SOLDIER_STATE_RUNNING = 1;
	static const int SOLDIER_STATE_JUMPING = 2;
	static const int SOLDIER_STATE_FALLING = 3;
	static const int PLAYER_ANIM_IDLE = 0;
	static const int PLAYER_ANIM_RUN = 1;
	static const int PLAYER_ANIM_JUMP = 2;
	static const int PLAYER_ANIM_FIRE = 3;
	static const int PLAYER_ANIM_MELEE = 4;
	static const int PLAYER_ANIM_AIM_UP = 4;
	static const int PLAYER_AIM_UP_START_FRAME = 5;
	static const int PLAYER_FRAME_SIZE = 96;

	static constexpr float GRAVITY = 1800.0f;
	static constexpr float PLAYER_MOVE_SPEED = 330.0f;
	static constexpr float PLAYER_JUMP_SPEED = 720.0f;
	static constexpr float ENEMY_MOVE_SPEED = 120.0f;
	static constexpr float ENEMY_STOP_DISTANCE = 95.0f;
	static constexpr float PLAYER_BULLET_SPEED = 850.0f;
	static constexpr float ENEMY_BULLET_SPEED = 260.0f;
	static constexpr float PLAYER_FIRE_COOLDOWN = 0.22f;
	static constexpr float MELEE_COOLDOWN = 0.5f;
	static constexpr float GRENADE_COOLDOWN = 0.8f;
	static constexpr float ROCKET_COOLDOWN = 1.15f;
	static constexpr float WORLD_WIDTH_LEVEL_1 = 3600.0f;
	static constexpr float WORLD_WIDTH_LEVEL_2 = 4400.0f;
	static constexpr float WORLD_WIDTH_LEVEL_3 = 5200.0f;
	static constexpr float BOSS_WORLD_WIDTH = 2600.0f;
};
