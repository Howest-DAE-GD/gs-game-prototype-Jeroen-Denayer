#include "pch.h"
#include "Game.h"
#include "BallManager.h"
#include "UI.h"
#include "Spiral.h"

Game::Game( const Window& window ) 
	:BaseGame{ window }
	, m_Viewport{ GetViewPort() }
	, m_GameOver{}
	, m_pBallManager{ nullptr }
	, m_pLighter{ nullptr }
	, m_pUI{ new UI(m_Viewport) }
	, m_Multiplier{}
	, m_Score{}
	, m_Difficulty{ 0 }
	, m_PointsToIncreaseDifficulty{ std::vector<int>{50, 100, 200} }
	, m_StartLives{ 5 }
	, m_MaxLives{ m_StartLives }
	, m_Lives{ m_StartLives }
	, m_TimeSinceLastPress{ 0.f }
	, m_TimeToLoseSingleLife{ 8.f }
	, m_PressingSpace{ false }
{
	Start();
}

Game::~Game( )
{
	delete m_pBallManager;
	delete m_pUI;
}

void Game::Update( float dt )
{
	if (m_GameOver)
		return;

	m_TimeSinceLastPress += dt;
	if (m_TimeSinceLastPress > m_TimeToLoseSingleLife)
	{
		DecreaseLives(1);
		m_TimeSinceLastPress = 0.f;
	}

	// Check keyboard state
	const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
	GameData::Input input{};
	if (pStates[SDL_SCANCODE_LEFT])
		input.pressedLeft = true;
	if (pStates[SDL_SCANCODE_RIGHT])
		input.pressedRight = true;
	if (pStates[SDL_SCANCODE_SPACE])
	{
		if (!m_PressingSpace)
		{
			m_TimeSinceLastPress = 0.f;
			m_PressingSpace = true;
			input.pressedSpace = true;
		}
	}
	else
	{
		m_PressingSpace = false;
	}
	GameData::Feedback feedback{};
	m_pBallManager->Update(dt, input, feedback);
	CheckFeedback(feedback);
}

void Game::Draw() const
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	m_pBallManager->Draw();

	//Arrow Demo
	//utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
	//Point2f center{ m_Viewport.width / 2.f, m_Viewport.height / 2.f };
	//glPushMatrix();
	//glTranslatef(center.x, center.y, 0);
	//utils::DrawLine(Point2f{ -100.f, 0.f }, Point2f{ 100.f, 0.f });
	//utils::DrawLine(Point2f{ 0.f, 100.f }, Point2f{ 0.f, -100.f });
	//float outerRad{ 150.f };
	//float innerRad{ 120.f };
	//Spiral::ArcInfo arcInfo{ Point2f{0.f, 0.f}, innerRad, outerRad, 45.f, 135.f};
	//Spiral::DrawArcBoundary(arcInfo);

	//float rad{ 100.f };
	//float angle{ 30.f };
	//float width{ 0.f };
	//Spiral::ArrowShapeDef arrowShapeDef{ 20.f, 15.f, 0.f, false, Spiral::ArrowDrawMode::base };
	//Spiral::DrawFilledArrowOnArc(arcInfo, 90.f, innerRad, arrowShapeDef, false);
	//glPopMatrix();

	//Draw UI
	float percOfLastLifeRemaining{ (m_TimeToLoseSingleLife - m_TimeSinceLastPress) / m_TimeToLoseSingleLife };
	m_pUI->Draw(m_Score, m_Multiplier, m_Lives, m_MaxLives, percOfLastLifeRemaining);
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	switch (e.keysym.sym)
	{
	case SDLK_r:
		Start();
		break;
	}
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::Start()
{
	m_GameOver = false;
	m_Score = 0;
	m_Multiplier = 1;
	m_MaxLives = m_StartLives;
	m_Lives = m_StartLives;

	if (m_pBallManager)
		delete m_pBallManager;
	float ballSize{ m_Viewport.width * 0.75f };
	float deadLineHeight{ m_pUI->GetHeight() + ballSize };
	m_pBallManager = new BallManager(m_Viewport, ballSize, deadLineHeight);
	m_pBallManager->Start();
}

void Game::CheckFeedback(const GameData::Feedback& feedback)
{
	if (feedback.failedMiniGame)
		DecreaseLives(1);
	if (feedback.finishedBall)
	{
		IncreaseScore(feedback.totalBallPoints);
		if (feedback.perfectBall)
			++m_Multiplier;
		else
			m_Multiplier = std::max(1, int(m_Multiplier / 2.f));
	}
}

void Game::IncreaseScore(int addedScore)
{
	m_Score += addedScore * m_Multiplier;

	if (m_Difficulty < m_PointsToIncreaseDifficulty.size() && m_Score > m_PointsToIncreaseDifficulty[m_Difficulty])
	{
		++m_Difficulty;
		m_pBallManager->IncreaseDifficulty();
	}
}

void Game::DecreaseLives(int numLives)
{
	m_Lives -= numLives;
	if (m_Lives <= 0)
		m_GameOver = true;
}
