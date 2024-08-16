#include "pch.h"
#include "BallManager.h"
#include "Ball.h"

BallManager::BallManager(Point2f startPos, float ballSize)
	: m_Pos{ startPos }
	, m_Active{ false }
	, m_pLevel{ 1 }
	, m_TimeToNextballSpawn{ 0.f }
	, m_MaxNumBallsOnScreen{ 10 }
	, m_pBalls{ std::vector<Ball*>(m_MaxNumBallsOnScreen) }
	, m_FirstBallIdx{ 0 }
	, m_LastBallIdx{ int(m_pBalls.size() - 1) }
	, m_BallSizes{ std::vector<float>{50.f, 100.f, 200.f} }
	, m_DistBetweenBalls{ 100.f }
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
			if (i == m_FirstBallIdx)
				pBall->m_Color = Color4f{ 0.192, 0.592, 0.82, 1.f };
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
	if (IsBallPastLighter(m_pBalls[m_FirstBallIdx], lighterData))
	{
		m_pBalls[m_FirstBallIdx]->SetState(Ball::State::Missed);
		SetNextBallActive();
	}

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

bool BallManager::IsBallHit(const Lighter::Data& lighterData)
{
	Ball* pBall{ m_pBalls[m_FirstBallIdx] };
	bool isBallHit{ pBall && IsBallInLighter(pBall, lighterData) };
	if (isBallHit)
		pBall->SetState(Ball::State::Caught);
	else
		pBall->SetState(Ball::State::Missed);

	SetNextBallActive();
	return isBallHit;
}

const std::vector<float>& BallManager::GetBallSizes()
{
	return m_BallSizes;
}

bool BallManager::IsBallInLighter(Ball* pBall, const Lighter::Data& lighterData)
{
	return  pBall && pBall->m_Rad > lighterData.innerRadius && pBall->m_Rad < lighterData.outerRadius &&
			pBall->m_YPos + pBall->m_Rad < lighterData.pos.y + lighterData.outerRadius &&
			pBall->m_YPos + pBall->m_Rad > lighterData.pos.y + lighterData.innerRadius;
}

bool BallManager::IsBallPastLighter(Ball* pBall, const Lighter::Data& lighterData)
{
	return pBall && pBall->m_YPos - pBall->m_Rad < lighterData.pos.y - lighterData.outerRadius;
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

	float ballSize{ m_BallSizes[rand() % m_BallSizes.size()] };
	m_pBalls[newBallIdx] = new Ball(ballSize, m_Pos.y, 150.f);
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
