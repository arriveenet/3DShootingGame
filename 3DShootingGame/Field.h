#pragma once
#include <glm/glm.hpp>

class Field
{
public:
	Field();
	void draw();
	bool isInField(const glm::vec3& _position);
};

extern Field g_field;
