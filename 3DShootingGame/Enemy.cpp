#include "Enemy.h"
#include <glm/gtx/transform.hpp>

using namespace glm;

Enemy g_enemy;

Enemy::Enemy()
	:m_dead(false)
{
	m_position = vec3(5, 0, 5);
	m_color = { 0xff, 0x00, 0x00 };
}

Enemy::~Enemy()
{
}

void Enemy::update()
{
	if (m_dead)
		return;

	mat4 m = mat4(1);
	m = translate(m, vec3(m_position));
	m = rotate(m, radians(m_rotation), vec3(0, 1, 0));
	for (int i = 0; i < PLAYER_VERTEX_COUNT; i++)
		m_vertex[i] = m * vec4(PlayerVertex[i], 1);
	for (int i = 0; i < 8; i++)
		m_BBVertex[i] = m * vec4(BoundingBox[i], 1);
}

void Enemy::draw()
{
	if (m_dead)
		return;

	Player::draw();
}