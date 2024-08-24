#pragma once
#include "BaseGame.h"
#include "GameData.h"

//Forward declarations
class BallManager;
class Lighter;
class UI;

class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float dt ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

private:
	//Functions
	void Start();
	void CheckFeedback(const GameData::Feedback& feedback);
	void IncreaseScore(int addedScore);
	void DecreaseLives(int numLives);

	//Members
	const Rectf& m_Viewport;
	BallManager* m_pBallManager;
	Lighter* m_pLighter;
	UI* m_pUI;
	int m_Score;
	int m_Difficulty;
	std::vector<int> m_PointsToIncreaseDifficulty;
	int m_StartLives;
	int m_MaxLives;
	int m_Lives;
	float m_TimeSinceLastPress;
	float m_TimeToLoseSingleLife;

	bool m_PressingSpace;
};