#pragma once
#include <glm/glm.hpp>

#define BULLET_SPEED	0.8f

class Bullet
{
public:
	bool m_enable;
	glm::vec3 m_position;
	glm::vec3 m_speed;
	glm::vec3 m_direction;

	Bullet();
	void update();
	void draw();
};

