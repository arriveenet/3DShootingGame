#pragma once
#include <glm/glm.hpp>

class Player{
public:
	glm::vec3 m_position;
	float m_rotate;

	Player();
	~Player();
	void update();
	void draw();
};

