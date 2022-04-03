#include "Enemy.h"
#include <glm/gtx/transform.hpp>

using namespace glm;

Enemy g_enemy;

Enemy::Enemy()
	:m_dead(false)
	,m_count(0)
	,m_state(ENEMY_STATE_UP)
{
	m_position = vec3(5, 0, 5);
	setColor(0xff, 0x00, 0x00);
}

Enemy::~Enemy()
{
}

void Enemy::update()
{
	if (m_dead)
		return;

	if (++m_count >= 144 * 2) {
		m_state = rand() % ENEMY_STATE_MAX;
		m_count = 0;
	}

	switch (m_state) {
	case ENEMY_STATE_UP:
		m_acceleration.x -= sinf(radians(m_rotation)) * PLAYER_MOVE_SPEED;
		m_acceleration.z -= cosf(radians(m_rotation)) * PLAYER_MOVE_SPEED;
		break;
	case ENEMY_STATE_DOWN:
		m_acceleration.x += sinf(radians(m_rotation)) * PLAYER_MOVE_SPEED;
		m_acceleration.z += cosf(radians(m_rotation)) * PLAYER_MOVE_SPEED;
		break;
	case ENEMY_STATE_ROTATE_LEFT:
		m_rotationAcceleration += PLAYER_ROTATE_SPEED;
		break;
	case ENEMY_STATE_ROTATE_RIGHT:
		m_rotationAcceleration -= PLAYER_ROTATE_SPEED;
		break;
	}

	if (m_rotation > 360)
		m_rotation = 0;
	else if (m_rotation < 0)
		m_rotation = 360;

	m_rotationAcceleration *= 0.72f;
	m_rotation += m_rotationAcceleration;

	m_acceleration *= 0.68f;
	m_position += m_acceleration;

	mat4 m = mat4(1);
	m = translate(m, vec3(m_position));
	m = rotate(m, radians(m_rotation), vec3(0, 1, 0));
	for (int i = 0; i < PLAYER_VERTEX_COUNT; i++)
		m_vertex[i] = m * vec4(PlayerVertex[i], 1);
	for (int i = 0; i < 8; i++)
		m_BBVertex[i] = m * vec4(BoundingBox[i], 1);

	if (m_position.x >= 34)
		m_position.x = 34;
	if (m_position.x <= -34)
		m_position.x = -34;
	if (m_position.z >= 34)
		m_position.z = 34;
	if (m_position.z <= -34)
		m_position.z = -34;
}

void Enemy::draw()
{
	if (m_dead)
		return;

	Player::draw();
}