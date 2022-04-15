#pragma once

#include "Bullet.h"
#include <glm/glm.hpp>

#define PLAYER_ROTATE_SPEED		0.6f
#define PLAYER_MOVE_SPEED		0.05f
#define PLAYER_VERTEX_COUNT		3
#define PLAYER_BB_VERTEX_COUNT	8

class Player{
public:
	Player();
	~Player();
	virtual void update();
	virtual void draw();
	void shoot();
	void setColor(unsigned char _r, unsigned char _g, unsigned _b);
	glm::vec3 getPosition() const;
	float getRotation() const;

protected:
	glm::vec3 m_position;
	glm::vec3 m_acceleration;
	float m_rotation;
	float m_rotationAcceleration;
	glm::vec3 m_direction;
	Bullet m_bullet;
	unsigned char m_color[3];
	glm::vec3 m_vertex[3];
	glm::vec3 m_BBVertex[8];
};

extern Player g_player;
extern const glm::vec3 PlayerVertex[PLAYER_VERTEX_COUNT];
extern const glm::vec3 BoundingBox[PLAYER_BB_VERTEX_COUNT];
