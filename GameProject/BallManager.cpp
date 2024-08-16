#include "pch.h"
#include "BallManager.h"
#include "Ball.h"

BallManager::BallManager(Point2f startPos, float ballSize)
	: m_Pos{ startPos }
	, m_Active{ false }
	, m_pLevel{ 1 }
	, m_TimeToNextballSpawn{ 0.f }
	, m_MaxNumBallsOnScreen{ 5 }
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
				utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
			else
				utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
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
		SetNextBallActive();

	//Spawn new ball
	m_TimeToNextballSpawn -= dt;
	if (m_TimeToNextballSpawn <= 0.f)
	{
		//If a ball is already ready, activate it
		if (m_pBalls[m_LastBallIdx])
			m_pBalls[m_LastBallIdx]->m_Active = true;
		else //if there is no ball ready, first create a new one and then immediatly activate it
		{
			CreateNewBall();
			m_pBalls[m_LastBallIdx]->m_Active = true;
		}

		//Already create the next ball
		CreateNewBall();
	}
}

bool BallManager::IsBallHit(const Lighter::Data& lighterData)
{
	Ball* pBall{ m_pBalls[m_FirstBallIdx] };
	bool isBallHit{ pBall && IsBallInLighter(pBall, lighterData) };
	if (isBallHit)
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
	if (m_pBalls[newBallIdx])//Remove old ball if it exists
		delete m_pBalls[newBallIdx];

	float ballSize{ m_BallSizes[rand() % m_BallSizes.size()] };
	m_pBalls[newBallIdx] = new Ball(ballSize, m_Pos.y + ballSize / 2.f, 150.f);

	++m_LastBallIdx %= m_pBalls.size();

	m_TimeToNextballSpawn = 2.f;
}

void BallManager::SetNextBallActive()
{
	++m_FirstBallIdx %= m_pBalls.size();
	m_pBalls[m_FirstBallIdx]->m_Active = true;
	//If the newly activated ball is the last one, create a new ball
	if (m_FirstBallIdx == m_LastBallIdx)
		CreateNewBall();
}
