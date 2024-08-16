#include "pch.h"
#include "Game.h"
#include "BallManager.h"
#include "Lighter.h"
#include "UI.h"

Game::Game( const Window& window ) 
	:BaseGame{ window }
	, m_pBallManager{ nullptr }
	, m_pLighter{ nullptr }
	, m_pUI{ new UI(GetViewPort()) }
	, m_Score{ 0 }
	, m_StartLives{ 5 }
	, m_MaxLives{ m_StartLives }
	, m_Lives{ m_StartLives }
	, m_TimeSinceLastPress{ 0.f }
	, m_TimeToLoseSingleLife{ 5.f }
{
	Start();
}

Game::~Game( )
{
	delete m_pBallManager;
	delete m_pLighter;
	delete m_pUI;
}

void Game::Update( float dt )
{
	m_TimeSinceLastPress += dt;
	if (m_TimeSinceLastPress > m_TimeToLoseSingleLife)
	{
		DecreaseLives(1);
		m_TimeSinceLastPress = 0.f;
	}

	m_pBallManager->Update(dt, m_pLighter->GetData());
	// Check keyboard state
	//const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
	//if ( pStates[SDL_SCANCODE_RIGHT] )
	//{
	//	std::cout << "Right arrow key is down\n";
	//}
	//if ( pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_UP])
	//{
	//	std::cout << "Left and up arrow keys are down\n";
	//}
}

void Game::Draw() const
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//Center x-axis on screen
	Rectf vp{ GetViewPort() };

	glPushMatrix();
	glTranslatef(vp.width / 2, 0.f, 0);
	glScalef(1.f, 1.f, 1.f);
	//#############

	m_pBallManager->Draw();
	m_pLighter->Draw();

	//#############
	glPopMatrix();

	float percOfLastLifeRemaining{ (m_TimeToLoseSingleLife - m_TimeSinceLastPress) / m_TimeToLoseSingleLife };
	m_pUI->Draw(m_Score, m_Lives, m_MaxLives, percOfLastLifeRemaining);
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	switch (e.keysym.sym)
	{
	case SDLK_SPACE:
		m_TimeSinceLastPress = 0.f;
		if (m_pBallManager->IsBallHit(m_pLighter->GetData()))
			IncreaseScore(1);
		else
			DecreaseLives(1);
		break;
	case SDLK_UP:
		m_pLighter->IncreaseSize();
		break;
	case SDLK_DOWN:
		m_pLighter->DecreaseSize();
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
	m_Score = 0;
	m_MaxLives = m_StartLives;
	m_Lives = m_StartLives;

	if (m_pBallManager)
		delete m_pBallManager;
	m_pBallManager = new BallManager(Point2f{ 0.f, GetViewPort().height}, 200.f);
	m_pBallManager->Start();

	if (m_pLighter)
		delete m_pLighter;
	m_pLighter = new Lighter(Point2f{ 0.f, GetViewPort().height * 0.3f }, m_pBallManager->GetBallSizes());
}

void Game::IncreaseScore(int addedScore)
{
	m_Score += addedScore;
}

void Game::DecreaseLives(int numLives)
{
	m_Lives -= numLives;
	if (m_Lives <= 0)
		Start();
}
