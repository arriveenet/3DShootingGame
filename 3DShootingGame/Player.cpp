#include "Player.h"
#include "Enemy.h"
#include "window.h"
#include "font.h"

#include <Windows.h>
#include <gl/GL.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/intersect.hpp>

using namespace glm;

const vec3 PlayerVertex[PLAYER_VERTEX_COUNT] = {
	{0, 0, -1.0f},
	{-0.5f, 0, 1.0f},
	{0.5f, 0, 1.0f}
};

const vec3 BoundingBox[8] = {
	{-0.5, -0.5, -1},
	{-0.5, -0.5, 1},
	{0.5, -0.5, 1},
	{0.5, -0.5, -1},
	{-0.5, 0.5, -1},
	{-0.5, 0.5, 1},
	{0.5, 0.5, 1},
	{0.5, 0.5, -1}
};

int faces[12][2] = {
	{0,1},
	{1,2},
	{2,3},
	{3,0},
	{4,5},
	{5,6},
	{6,7},
	{7,4},
	{4,0},
	{5,1},
	{6,2},
	{7,3}
};

int triangle[8][3] = {
	{2,5,1},
	{5,2,0},
	{3,0,2},
	{0,3,7},
	{3,4,0},
	{4,3,7},
	{0,5,1},
	{5,0,4}
};

Player::Player()
	: m_position(0,0,0)
	, m_rotation(0)
	, m_rotationAcceleration(0)
	, m_acceleration(0)
	, m_direction(0)
	, m_bullet()
	, m_color{ 0x00, 0xff, 0x00 }
{
	for (int i = 0; i < PLAYER_VERTEX_COUNT; i++)
		m_vertex[i] = PlayerVertex[i];
	for (int i = 0; i < 8; i++)
		m_BBVertex[i] = BoundingBox[i];
}

Player::~Player()
{

}

void Player::update()
{
	m_bullet.update();

	if (g_keys['D']) {
		m_rotationAcceleration -= PLAYER_ROTATE_SPEED;
	}

	if (g_keys['A']) {
		m_rotationAcceleration += PLAYER_ROTATE_SPEED;
	}

	if (g_keys['W']) {
		m_acceleration.x -= sinf(radians(m_rotation)) * PLAYER_MOVE_SPEED;
		m_acceleration.z -= cosf(radians(m_rotation)) * PLAYER_MOVE_SPEED;
	}

	if (g_keys['S']) {
		m_acceleration.x += sinf(radians(m_rotation)) * PLAYER_MOVE_SPEED;
		m_acceleration.z += cosf(radians(m_rotation)) * PLAYER_MOVE_SPEED;
	}

	if (g_keys[' ']) {
		shoot();
	}

	if (m_rotation > 360)
		m_rotation = 0;
	else if (m_rotation < 0)
		m_rotation = 360;
	m_rotationAcceleration *= 0.72f;
	m_rotation += m_rotationAcceleration;
	m_acceleration *= 0.68f;
	m_position += m_acceleration;

	m_direction.x = sinf(radians(m_rotation));
	m_direction.z = cosf(radians(m_rotation));
	m_direction = normalize(m_direction);

	mat4 m = mat4(1);
	m = translate(m, vec3(m_position));
	m = rotate(m, radians(m_rotation), vec3(0, 1, 0));
	for(int i = 0; i < PLAYER_VERTEX_COUNT; i++)
		m_vertex[i] = m * vec4(PlayerVertex[i], 1);
	for (int i = 0; i < 8; i++)
		m_BBVertex[i] = m * vec4(BoundingBox[i], 1);

	if (m_bullet.m_enable) {
		vec3 position;
		for (int i = 0; i < 8; i++) {
			if (intersectLineTriangle(
				m_bullet.m_position,	// genType const & orig
				m_bullet.m_position - m_bullet.m_direction * BULLET_SPEED,	// genType const & dir
				g_enemy.m_BBVertex[triangle[i][0]],	// genType const & vert0
				g_enemy.m_BBVertex[triangle[i][1]],	// genType const & vert1
				g_enemy.m_BBVertex[triangle[i][2]],	// genType const & vert2
				position	// genType & position
			)) {
				printf("position: %f,%f,%f\n", position.x, position.y, position.z);
				//g_enemy.m_dead = true;
			}
		}
	}
}

void Player::draw()
{
	// Draw bullet
	m_bullet.draw();

	// Draw bounding box
	glColor3ub(0x00, 0xff, 0xff);
	for (int i = 0; i < 12; i++) {
		glBegin(GL_LINES);
		glVertex3fv((GLfloat*)&m_BBVertex[faces[i][0]]);
		glVertex3fv((GLfloat*)&m_BBVertex[faces[i][1]]);
		glEnd();
	}

	{
		glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);// GLbitfield mask
		glPushAttrib(GL_ALL_ATTRIB_BITS);// GLbitfield mask
		glEnableClientState(GL_VERTEX_ARRAY);// GLenum array
		glDisable(GL_TEXTURE_2D);// GLenum cap

		// Draw player
		glColor3ub(m_color.r, m_color.g, m_color.b);
		glVertexPointer(
			3,				// GLint size
			GL_FLOAT,		// GLenum type
			0,				// GLsizei stride
			&m_vertex);		// const GLvoid * pointer
		glDrawArrays(
			GL_TRIANGLES,	// GLenum mode
			0,				// GLint first
			3);				// GLsizei count
		glPopAttrib();

		glPopAttrib();
		glPopClientAttrib();
	}
}

void Player::shoot()
{
	if (m_bullet.m_enable)
		return;
	m_bullet.m_enable = true;
	m_bullet.m_position = m_position;
	m_bullet.m_position2 = m_position - m_direction * BULLET_SPEED;
	m_bullet.m_direction = m_direction;
}