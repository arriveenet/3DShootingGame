#pragma once
#include "Player.h"

enum {
	ENEMY_STATE_UP,
	ENEMY_STATE_DOWN,
	ENEMY_STATE_ROTATE_LEFT,
	ENEMY_STATE_ROTATE_RIGHT,
	ENEMY_STATE_MAX
};

const char enemy_state_string[][32] = {
	"ENEMY_STATE_UP",
	"ENEMY_STATE_DOWN",
	"ENEMY_STATE_ROTATE_LEFT",
	"ENEMY_STATE_ROTATE_RIGHT",
	"ENEMY_STATE_MAX" };

class Enemy :
	public Player
{
public:
	bool m_dead;
	unsigned int m_count;
	int m_state;

	Enemy();
	~Enemy();
	void update();
	void draw();
};

extern Enemy g_enemy;
