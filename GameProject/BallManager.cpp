#include "pch.h"
#include "BallManager.h"
#include "Ball.h"
#include "CatchMechanism.h"
#include "SingleClick.h"

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
}

void BallManager::Draw() const
{
	for (int i{}; i < m_pBalls.size(); ++i)
	{
		Ball* pBall{ m_pBalls[i] };
		if (pBall)
		{
			//if (i == m_FirstBallIdx)
			//	pBall->m_Color = Color4f{ 0.192f, 0.592f, 0.82f, 1.f };
			pBall->Draw();
		}
	}
}

void BallManager::Update(float dt, const Lighter::Data& lighterData)
{
	if (!m_Active)
		return;

	//Update existing balls
	for (Ball* pBall : m_pBalls)
		if (pBall)
			pBall->Update(dt, lighterData);

	//Check if first ball is below the lighter
	if (m_pBalls[m_FirstBallIdx] && m_pBalls[m_FirstBallIdx]->m_State == Ball::State::Missed)
		SetNextBallActive();

	//Spawn new ball
	m_TimeToNextballSpawn -= dt;
	if (m_TimeToNextballSpawn <= 0.f)
	{
		//If there is no ball raedy, activate it
		if (!m_pBalls[m_LastBallIdx])
			CreateNewBall(); 
		//activate the ball
		m_pBalls[m_LastBallIdx]->SetState(Ball::State::Active);

		//Already create the next ball
		CreateNewBall();
	}
}

void BallManager::ReceiveInput(const Lighter::Data& lighterData)
{
	Ball* pBall{ m_pBalls[m_FirstBallIdx] };
	pBall->ReceiveInput(lighterData);
	SetHitData();

	if (pBall->m_State == Ball::State::Completed || pBall->m_State == Ball::State::Missed)
		SetNextBallActive();
}

const std::vector<float>& BallManager::GetBallSizes()
{
	return m_BallSizes;
}

const BallManager::HitData& BallManager::GetHitData() const
{
	return m_HitData;
}

void BallManager::CreateNewBall()
{
	int newBallIdx{ int((m_LastBallIdx + 1) % m_pBalls.size()) };
	//If the next ball should be placed where the first ball is then return because the first ball should not be deleted
	if (newBallIdx == m_FirstBallIdx && m_pBalls[m_FirstBallIdx])
		return;

	//Remove old ball if it exists
	if (m_pBalls[newBallIdx])
		delete m_pBalls[newBallIdx];

	int ballSizeIdx{ int(rand() % m_BallSizes.size()) };
	float ballSize{ m_BallSizes[ballSizeIdx] };
	int score{ ballSizeIdx + 1 };

	m_pBalls[newBallIdx] = new Ball(ballSize, m_Pos.y, 150.f, score, CatchMechanism::Type::SingleClick);
	++m_LastBallIdx %= m_pBalls.size();

	m_TimeToNextballSpawn = m_pBalls[m_LastBallIdx]->m_TimeToSolve;
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
	m_HitData.completed = pBall->m_State == Ball::State::Completed;
	if (m_HitData.completed)
	{
		m_HitData.score = pBall->m_Points;
	}
}
