#include "pch.h"
#include "Ball.h"
#include "BallManager.h"
#include "MiniGame.h"

float Ball::s_FadeTime{ 0.5f };

Ball::Ball(float size, float yPos, float speed, int points, MiniGame* pMiniGame)
	: m_State{ State::Idle }
	, m_Points{ points }
	, m_Rad{ size / 2.f }
	, m_YPos{ yPos }
	, m_Speed{ speed }
	, m_TimeToSolve{ 3.f }
	, m_TimeSinceCompletion{ 0.f }
	, m_Color{ Color4f{1.f, 1.f, 1.f, 1.f} }
	, m_pMiniGame{ pMiniGame }
{
}

Ball::~Ball()
{
	delete m_pMiniGame;
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
	m_pMiniGame->Draw(m_YPos, m_Rad);

	utils::DrawEllipse(Point2f{ 0.f, m_YPos }, m_Rad, m_Rad, 2.f);
}

void Ball::Update(float dt, bool pressedLeft, bool pressedRight)
{
	switch (m_State)
	{
	case State::Idle:
		return;
	case State::Active:
		m_pMiniGame->Update(dt, pressedLeft, pressedRight);
		if (m_pMiniGame->GetState() == MiniGame::State::Completed)
			SetState(State::Caught);
		else if (m_pMiniGame->GetState() == MiniGame::State::Failed)
			SetState(State::Missed);
		break;
	case State::Caught:
		break;
	case State::Completed: case State::Missed:
		m_TimeSinceCompletion += dt;
		break;
	}

	if (m_State != State::Caught)
		m_YPos += -m_Speed * dt;
}

void Ball::Click()
{
	switch (m_State)
	{
	case State::Active:
		m_pMiniGame->Click();
		if (m_pMiniGame->GetState() == MiniGame::State::Completed)
			SetState(State::Caught);
		else if (m_pMiniGame->GetState() == MiniGame::State::Failed)
			SetState(State::Missed);
		break;
	} 
}

float Ball::GetTimeToSolve() const
{
	return m_TimeToSolve;
}

float Ball::GetYPos() const
{
	return m_YPos;
}

float Ball::GetRadius() const
{
	return m_Rad;
}

void Ball::SetState(State newState)
{
	m_State = newState;
	switch (m_State)
	{
	case State::Active:
		m_pMiniGame->Activate();
		break;
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
