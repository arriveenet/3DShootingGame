#include "Field.h"
#include <Windows.h>
#include <gl/GL.h>

Field g_field;

Field::Field()
{

}

void Field::draw()
{
	glColor3ub(0xff, 0xff, 0xff);
	glBegin(GL_LINES);
	for (int i = -35; i < 36; i += 2) {
		glVertex3i(i, 0, -35);
		glVertex3i(i, 0, 35);
		glVertex3i(-35, 0, i);
		glVertex3i(35, 0, i);
	}
	glEnd();
}

bool Field::isInField(const glm::vec3& _position)
{
	if((_position.x < 35)
		&& (_position.x > -35)
		&& (_position.z < 35)
		&& (_position.z > -35))
		return true;

	return false;
}