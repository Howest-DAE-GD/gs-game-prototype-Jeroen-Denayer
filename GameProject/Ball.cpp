#include "pch.h"
#include "Ball.h"
#include "BallManager.h"
#include "MiniGame.h"
#include "Spiral.h"

float Ball::s_FadeTime{ 0.5f };

Ball::Ball(const Point2f& pos, float size, float idleSpeed, float activeSpeed, std::vector<MiniGame*>* pMiniGames)
	: m_State{ State::Idle }
	, m_Points{ 0 }
	, m_Rad{ size / 2.f }
	, m_Pos{ pos }
	, m_Acc{ 200.f }
	, m_Speed{ idleSpeed }
	, m_TargetSpeed{ m_Speed }
	, m_ActiveSpeed{ activeSpeed }
	, m_TimeToComplete{ 3.f }
	, m_TimeSinceCompletion{ 0.f }
	, m_Color{ Color4f{1.f, 1.f, 1.f, 1.f} }
	, m_ActiveMiniGameIdx{ 0 }
	, m_pMiniGames{ pMiniGames }
{
}

Ball::~Ball()
{
	if (m_pMiniGames)
		for (MiniGame* pMiniGame : *m_pMiniGames)
			delete pMiniGame;
} 

void Ball::Draw() const
{
	utils::SetGlobalAlpha(m_Color.a);

	utils::SetColor(Color3f{0.15f, 0.15f, 0.15f });
	utils::FillEllipse(m_Pos, m_Rad, m_Rad);

	float innerRad{ m_Rad * 0.5f };
	MiniGame* pMiniGame{ (*m_pMiniGames)[m_ActiveMiniGameIdx] };
	pMiniGame->Draw(m_Pos, innerRad, m_Rad, innerRad);

	DrawMiniGameIndicators(m_Rad);

	utils::SetColor(m_Color);
	utils::DrawEllipse(m_Pos, innerRad, innerRad, 2.f);
	utils::DrawEllipse(m_Pos, m_Rad, m_Rad, 2.f);

	utils::UseGlobalAlpha(false);
}

void Ball::Update(float dt, const GameData::Input& input, GameData::Feedback& feedback)
{
	if (m_Speed != m_TargetSpeed)
		UpdateSpeed(dt);

	m_Pos.y += -m_Speed * dt;

	switch (m_State)
	{
	case State::Active:
	{
		MiniGame* pMiniGame{ (*m_pMiniGames)[m_ActiveMiniGameIdx] };
		pMiniGame->Update(dt, input, feedback);
		if (pMiniGame->GetState() == MiniGame::State::Completed || pMiniGame->GetState() == MiniGame::State::Failed)
		{
			m_Points += pMiniGame->GetPoints();
			ActivateNextMiniGame(feedback);
		}
		break;
	}
	case State::Finished:
		m_TimeSinceCompletion += dt;
		float alpha{ (s_FadeTime - m_TimeSinceCompletion) / s_FadeTime };
		if (alpha < 0.f)
			alpha = 0.f;
		m_Color.a = alpha;
		break;
	}
}

void Ball::DrawMiniGameIndicators(float innerRad) const
{
	int numMiniGames{ int(m_pMiniGames->size()) };
	float height{ 10.f };
	float outerRad{ innerRad + height };
	float arcLengthPerIndicator{ 20.f };
	float totalArcLegth{ numMiniGames * arcLengthPerIndicator };

	float indicatorAngle{ 270 };
	float startAngle{ indicatorAngle - totalArcLegth / 2.f };
	float endAngle{ indicatorAngle + totalArcLegth / 2.f };

	//Fill in completed miniGames
	for (int i{}; i < m_pMiniGames->size(); ++i)
	{
		const MiniGame* pMiniGame{ (*m_pMiniGames)[i] };
		switch (pMiniGame->GetState())
		{
		case MiniGame::State::Idle:
			utils::SetColor(Color3f{ 0.4f, 0.4f, 0.4f });
			break;
		case MiniGame::State::Active:
			utils::SetColor(Color3f{ 0.75f, 0.75f, 0.75f });
			break;
		case MiniGame::State::Completed:
			utils::SetColor(Color3f{ 0.192f, 0.722f, 0.192f });
			break;
		case MiniGame::State::Failed:
			utils::SetColor(Color3f{ 1.f, 0.f, 0.f });
			break;
		}

		float startAngleIndicator{ startAngle + i * arcLengthPerIndicator };
		float endAngleIndicator{ startAngle + (i + 1) * arcLengthPerIndicator };
		Spiral::DrawFilledArc(m_Pos, innerRad, outerRad, utils::Radians(startAngleIndicator), utils::Radians(endAngleIndicator));
	}

	//Draw lines creating separate boxes
	utils::SetColor(Color3f{ 1.f, 1.f, 1.f });
	for (int i{}; i < numMiniGames + 1; ++i)
	{
		float angle{ startAngle + i * arcLengthPerIndicator };
		Point2f p0{m_Pos.x + innerRad * std::cosf(utils::Radians(angle)), m_Pos.y + innerRad * std::sinf(utils::Radians(angle)) };
		Point2f p1{m_Pos.x + outerRad * std::cosf(utils::Radians(angle)), m_Pos.y + outerRad * std::sinf(utils::Radians(angle)) };
		utils::DrawLine(p0, p1);
	}

	//Draw arc connecting all the lines
	utils::DrawArc(m_Pos, outerRad, outerRad, utils::Radians(startAngle), utils::Radians(endAngle));
}

void Ball::SetState(State newState)
{
	m_State = newState;
	switch (m_State)
	{
	case State::Active:
	{
		MiniGame* pMiniGame{ (*m_pMiniGames)[m_ActiveMiniGameIdx] };
		pMiniGame->Activate();
		m_TargetSpeed = m_ActiveSpeed;
		break;
	}
	}
}

void Ball::UpdateSpeed(float dt)
{
	//move m_Speed towards m_TargetSpeed
	float acc{ m_Acc };
	//Point acc in direction of m_TargetSpeed
	if (m_TargetSpeed > m_Speed)
		acc = std::abs(m_Acc);
	else //m_TargetSpeed < m_Speed
		acc = -std::abs(m_Acc);

	m_Speed += acc * dt;
	if ((acc > 0.f && m_Speed > m_TargetSpeed) || (acc < 0.f && m_Speed < m_TargetSpeed))
		m_Speed = m_TargetSpeed;
}

void Ball::ActivateNextMiniGame(GameData::Feedback& feedback)
{
	if (m_ActiveMiniGameIdx < m_pMiniGames->size() - 1)
	{
		++m_ActiveMiniGameIdx;
		MiniGame* pMiniGame{ (*m_pMiniGames)[m_ActiveMiniGameIdx] };
		pMiniGame->Activate();
	}
	else //all minigames completed
	{
		feedback.finishedBall = true;
		feedback.totalBallPoints = m_Points;
		bool perfectBall{ true };
		for (const MiniGame* pMiniGame : *m_pMiniGames)
		{
			if (pMiniGame->GetState() == MiniGame::State::Failed)
			{
				perfectBall = false;
				break;
			}
		}
		feedback.perfectBall = perfectBall;
		SetState(State::Finished);
	}
}
