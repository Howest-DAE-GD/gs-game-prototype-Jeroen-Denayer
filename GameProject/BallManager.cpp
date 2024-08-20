#include "pch.h"
#include "BallManager.h"
#include "Ball.h"
#include "CatchMechanism.h"
#include "SingleClick.h"
#include "SetRotationGame.h"
#include "SelectColorGame.h"

BallManager::BallManager(Point2f startPos, float ballSize)
	: m_Pos{ startPos }
	, m_Active{ false }
	, m_pLevel{ 1 }
	, m_TimeToNextballSpawn{ 0.f }
	, m_MaxNumBallsOnScreen{ 10 }
	, m_pBalls{ std::vector<Ball*>(m_MaxNumBallsOnScreen) }
	, m_FirstBallIdx{ 0 }
	, m_LastBallIdx{ int(m_pBalls.size() - 1) }
	, m_BallSizes{ std::vector<float>{200.f, 100.f, 50.f} }
	, m_HitData{}
{
}

BallManager::~BallManager()
{
	for (Ball* pBall : m_pBalls)
		if (pBall)
			delete pBall;
}

void BallManager::Start()
{
	m_Active = true;
	CreateNewBall(true);
	CreateNewBall();
}

void BallManager::Draw() const
{
	for (int i{}; i < m_pBalls.size(); ++i)
	{
		Ball* pBall{ m_pBalls[i] };
		if (pBall)
			pBall->Draw();
	}
}

void BallManager::Update(float dt, float deadlineHeight, bool pressedLeft, bool pressedRight)
{
	if (!m_Active)
		return;

	//Update existing balls
	for (Ball* pBall : m_pBalls)
		if (pBall)
			pBall->Update(dt, deadlineHeight, pressedLeft, pressedRight);

	if (m_pBalls[m_FirstBallIdx]->m_State == Ball::State::Completed || m_pBalls[m_FirstBallIdx]->m_State == Ball::State::Missed)
		SetNextBallActive();
}

void BallManager::Click()
{
	Ball* pBall{ m_pBalls[m_FirstBallIdx] };
	pBall->Click();
	SetHitData();
}

const std::vector<float>& BallManager::GetBallSizes()
{
	return m_BallSizes;
}

const BallManager::HitData& BallManager::GetHitData() const
{
	return m_HitData;
}

void BallManager::CreateNewBall(bool activate)
{
	++m_LastBallIdx %= m_pBalls.size();
	//If the next ball should be placed where the first ball is then return because the first ball should not be deleted
	if (m_LastBallIdx == m_FirstBallIdx && m_pBalls[m_FirstBallIdx])
		return;

	//Remove old ball if it exists
	if (m_pBalls[m_LastBallIdx])
		delete m_pBalls[m_LastBallIdx];

	int ballSizeIdx{ int(rand() % m_BallSizes.size()) };
	float ballSize{ m_BallSizes[0] };

	MiniGame* pMiniGame{};
	int typeIdx{ rand() % 2 };
	switch (MiniGame::Type(typeIdx))
	{
	case MiniGame::Type::SelectColor:
		pMiniGame = new SelectColorGame();
		break;
	case MiniGame::Type::SetRotation:
		pMiniGame = new SetRotationGame();
		break;
	}

	m_pBalls[m_LastBallIdx] = new Ball(ballSize, m_Pos, 100.f, pMiniGame);
	if (activate)
		m_pBalls[m_LastBallIdx]->SetState(Ball::State::Active);

	//m_TimeToNextballSpawn = m_pBalls[m_LastBallIdx]->m_TimeToSolve;
}

void BallManager::SetNextBallActive()
{
	++m_FirstBallIdx %= m_pBalls.size();
	m_pBalls[m_FirstBallIdx]->SetState(Ball::State::Active);
	//If the newly activated ball is the last one, create a new ball
	if (m_FirstBallIdx == m_LastBallIdx)
		CreateNewBall();
}

void BallManager::SetHitData()
{
	Ball* pBall{ m_pBalls[m_FirstBallIdx] };
	if (pBall->m_State == Ball::State::Completed)
	{
		m_HitData.totalBallScore = pBall->GetPoints();
	}
	else
	{
		m_HitData.totalBallScore = 0;
	}
}
