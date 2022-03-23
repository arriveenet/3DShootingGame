#pragma once
#include "Player.h"
class Enemy :
	public Player
{
public:
	bool m_dead;

	Enemy();
	~Enemy();
	void update();
	void draw();
};

extern Enemy g_enemy;
