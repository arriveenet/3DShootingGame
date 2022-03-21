#pragma once
#include "Player.h"
class Enemy :
	public Player
{
public:
	Enemy();
	~Enemy();
	void update();
	void draw();
};

extern Enemy g_enemy;
