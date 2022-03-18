#include "Player.h"
#include <Windows.h>
#include <gl/GL.h>

using namespace glm;

static glm::vec3 PlayerVertex[3] = {
	{0, 0, -1.0f},
	{-0.5f, 0, 1.0f},
	{0.5f, 0, 1.0f}
};

Player::Player()
	: m_position(0,0,0)
	, m_rotate(0)
{
}
Player::~Player()
{

}

void Player::update()
{

}

void Player::draw()
{
	
}