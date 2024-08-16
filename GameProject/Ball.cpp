#include "pch.h"
#include "Ball.h"

float Ball::s_FadeTime{ 0.5f };

Ball::Ball(float size, float yPos, float speed)
	: m_State{ State::Idle }
	, m_TimeSinceCompletion{ 0.f }
	, m_Points{ 1 }
	, m_Rad{ size / 2.f }
	, m_YPos{ yPos }
	, m_Speed{ speed }
	, m_TimeToSolve{ 2.f }
	, m_Color{ Color4f{1.f, 1.f, 1.f, 1.f} }
{
}

void Ball::Draw() const
{
	Color4f color{ m_Color };

	switch (m_State)
	{
	case State::Completed: case State::Missed:
		float alpha{ (s_FadeTime - m_TimeSinceCompletion) / s_FadeTime };
		if (alpha < 0.f)
			alpha = 0.f;
		color.a = alpha;
		break;
	}

	utils::SetColor(color);
	utils::DrawEllipse(Point2f{ 0.f, m_YPos }, m_Rad, m_Rad, 2.f);
}

void Ball::Update(float dt, const Lighter::Data& lighterData)
{
	if (m_State == State::Idle)
		return;

	m_YPos += -m_Speed * dt;

	if (m_State == State::Completed || m_State == State::Missed)
		m_TimeSinceCompletion += dt;
}

float Ball::GetTimeToSolve() const
{
	return m_TimeToSolve;
}

void Ball::SetState(State newState)
{
	m_State = newState;
	switch (m_State)
	{
	case State::Caught:
		SetState(State::Completed);
		break;
	case State::Completed:
		m_Color = Color4f{ 0.f, 1.f, 0.f, 1.f };
		break;
	case State::Missed:
		m_Color = Color4f{ 1.f, 0.f, 0.f, 1.f };
		break;
	}
}
