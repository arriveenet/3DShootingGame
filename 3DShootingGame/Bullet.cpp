#include "Bullet.h"
#include <Windows.h>
#include <gl/GL.h>

using namespace glm;

Bullet::Bullet()
	: m_enable(false)
	, m_position(0)
	, m_speed(0)
{}

void Bullet::update()
{
	if (!m_enable)
		return;

	m_position -= m_direction * BULLET_SPEED;
}

void Bullet::draw()
{
	if (!m_enable)
		return;

	glColor3ub(0xff, 0xff, 0x00);
	glPointSize(5.f);
	glBegin(GL_POINTS);
	{
		glVertex3fv((GLfloat*)&m_position);
	}
	glEnd();
}