#include "pch.h"
#include "Ball.h"

Ball::Ball(float rad, float yPos, float speed)
	: m_Rad{ rad }
	, m_YPos{ yPos }
	, m_Speed{ speed }
{
}

void Ball::Draw() const
{
	utils::DrawEllipse(Point2f{ 0.f, m_YPos }, m_Rad, m_Rad);
}

void Ball::Update(float dt, const Lighter::Data& lighterData)
{
	m_YPos += -m_Speed * dt;	
}