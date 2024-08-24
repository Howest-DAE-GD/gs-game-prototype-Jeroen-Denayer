#include "pch.h"
#include "Game.h"
#include "BallManager.h"
#include "Ball.h"
#include "CatchMechanism.h"
#include "SingleClick.h"
#include "SetRotationGame.h"
#include "SelectColorGame.h"
#include "SpiralGatesGame.h"

BallManager::BallManager(const Rectf& viewport, float ballSize, float deadLineHeight)
	: m_Viewport{ viewport }
	, m_Pos{ Point2f{viewport.width / 2.f, viewport.height + ballSize} }
	, m_Active{ false }
	, m_Difficulty{ 0 }
	, m_MaxNumBallsOnScreen{ 10 }
	, m_pBalls{ std::vector<Ball*>(m_MaxNumBallsOnScreen) }
	, m_FirstBallIdx{ 0 }
	, m_LastBallIdx{ int(m_pBalls.size() - 1) }
	, m_BallSize{ ballSize }
	, m_DeadlineHeight{ deadLineHeight }
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
	CreateNewBall();
}

void BallManager::Draw() const
{
	DrawDeadline();

	for (Ball* pBall : m_pBalls)
		if (pBall)
			pBall->Draw();
}

void BallManager::Update(float dt, const GameData::Input& input, GameData::Feedback& feedback)
{
	if (!m_Active)
		return;

	//Update existing balls
	for (Ball* pBall : m_pBalls)
	{
		if (!pBall)
			continue;

		pBall->Update(dt, input, feedback);
		float topPos{ pBall->m_Pos.y + pBall->m_Rad };
		if (pBall->m_State == Ball::State::Idle && topPos < m_Viewport.height)
			pBall->SetState(Ball::State::Active);
		else if (pBall->m_State == Ball::State::Active && topPos < m_DeadlineHeight)
			pBall->SetState(Ball::State::Missed);
	}

	if (m_pBalls[m_FirstBallIdx]->m_State == Ball::State::Completed || m_pBalls[m_FirstBallIdx]->m_State == Ball::State::Missed)
		CreateNewBall();
}

void BallManager::IncreaseDifficulty()
{
	++m_Difficulty;
}

void BallManager::CreateNewBall()
{
	//std::cout << "Create new ball: " << std::endl;
	++m_LastBallIdx %= m_pBalls.size();
	//If the next ball should be placed where the first ball is then return because the first ball should not be deleted
	if (m_LastBallIdx == m_FirstBallIdx && m_pBalls[m_FirstBallIdx])
		return;

	//Remove old ball if it exists
	if (m_pBalls[m_LastBallIdx])
		delete m_pBalls[m_LastBallIdx];

	//Choose a random minigame
	MiniGame* pMiniGame{};
	int typeIdx{ rand() % 3 };
	switch (MiniGame::Type(typeIdx))
	{
	case MiniGame::Type::SelectColor:
		pMiniGame = new SelectColorGame(m_Difficulty);
		break;
	case MiniGame::Type::SetRotation:
		pMiniGame = new SetRotationGame(m_Difficulty);
		break;
	case MiniGame::Type::SpiralGates:
		pMiniGame = new SpiralGatesGame(m_Difficulty);
		break;
	}

	//Calculate speed needed
	float maxSpeed{ 200.f };
	float speed{ 100.f };
	if (pMiniGame)
	{
		float timeToComplete{ pMiniGame->GetTimeToComplete() };
		float distToTravel{ std::abs(m_Pos.y - m_DeadlineHeight) };
		speed = distToTravel / timeToComplete;
	}
	speed = std::min(speed, maxSpeed);

	m_pBalls[m_LastBallIdx] = new Ball(m_BallSize, m_Pos, speed, pMiniGame);
	m_FirstBallIdx = m_LastBallIdx;
}

void BallManager::SetNextBallActive()
{
	++m_FirstBallIdx %= m_pBalls.size();
	m_pBalls[m_FirstBallIdx]->SetState(Ball::State::Active);
}

void BallManager::DrawDeadline() const
{
	utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
	utils::DrawLine(Point2f{ 0.f, m_DeadlineHeight }, Point2f{ m_Viewport.width, m_DeadlineHeight });

	float triangleHeight{ 20.f };
	float triangleWidth{ 10.f };
	utils::FillPolygon(std::vector<Point2f>{Point2f{ 0.f, m_DeadlineHeight + triangleWidth * 0.5f }, Point2f{ triangleHeight, m_DeadlineHeight }, Point2f{ 0.f,m_DeadlineHeight - triangleWidth * 0.5f }});
	utils::FillPolygon(std::vector<Point2f>{Point2f{ m_Viewport.width, m_DeadlineHeight + triangleWidth * 0.5f }, Point2f{ m_Viewport.width - triangleHeight, m_DeadlineHeight }, Point2f{ m_Viewport.width,m_DeadlineHeight - triangleWidth * 0.5f }});
}