#include "pch.h"
#include "Ball.h"
#include "BallManager.h"
#include "MiniGame.h"

float Ball::s_FadeTime{ 0.5f };

Ball::Ball(float size, const Point2f& pos, float speed, MiniGame* pMiniGame)
	: m_State{ State::Idle }
	, m_Points{ 0 }
	, m_Rad{ size / 2.f }
	, m_Pos{ pos }
	, m_Speed{ speed }
	, m_TimeToComplete{ 3.f }
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
	utils::SetGlobalAlpha(m_Color.a);

	float innerRad{ m_Rad * 0.5f };

	m_pMiniGame->Draw(m_Pos, innerRad, m_Rad, innerRad);

	utils::SetColor(m_Color);
	utils::DrawEllipse(m_Pos, innerRad, innerRad, 2.f);
	utils::DrawEllipse(m_Pos, m_Rad, m_Rad, 2.f);

	utils::UseGlobalAlpha(false);
}

void Ball::Update(float dt, const GameData::Input& input, GameData::Feedback& feedback)
{
	m_Pos.y += -m_Speed * dt;

	switch (m_State)
	{
	case State::Active:
	{
		m_pMiniGame->Update(dt, input, feedback);
		if (m_pMiniGame->GetState() == MiniGame::State::Completed)
		{
			m_Points += feedback.totalPoints;
			SetState(State::Completed);
		}
		else if (m_pMiniGame->GetState() == MiniGame::State::Failed)
			SetState(State::Missed);
		break;
	}
	case State::Completed: case State::Missed:
		m_TimeSinceCompletion += dt;
		float alpha{ (s_FadeTime - m_TimeSinceCompletion) / s_FadeTime };
		if (alpha < 0.f)
			alpha = 0.f;
		m_Color.a = alpha;
		break;
	}
}

float Ball::GetTimeToComplete() const
{
	return m_pMiniGame->GetTimeToComplete();
}

void Ball::SetState(State newState)
{
	m_State = newState;
	switch (m_State)
	{
	case State::Active:
		m_pMiniGame->Activate();
		break;
	case State::Completed:
		m_Color = Color4f{ 0.f, 1.f, 0.f, 1.f };
		break;
	case State::Missed:
		m_Color = Color4f{ 1.f, 0.f, 0.f, 1.f };
		break;
	}
}
