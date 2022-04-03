#pragma once

#include "Bullet.h"
#include <glm/glm.hpp>

#define PLAYER_ROTATE_SPEED	0.6f
#define PLAYER_MOVE_SPEED	0.05f
#define PLAYER_VERTEX_COUNT	3

class Player{
public:
	glm::vec3 m_position;
	glm::vec3 m_acceleration;
	float m_rotation;
	float m_rotationAcceleration;
	glm::vec3 m_direction;
	Bullet m_bullet;
	unsigned char m_color[3];
	glm::vec3 m_vertex[3];
	glm::vec3 m_BBVertex[8];

	Player();
	~Player();
	virtual void update();
	virtual void draw();
	void shoot();
	void setColor(unsigned char _r, unsigned char _g, unsigned _b);
};

extern const glm::vec3 PlayerVertex[PLAYER_VERTEX_COUNT];
extern const glm::vec3 BoundingBox[8];
