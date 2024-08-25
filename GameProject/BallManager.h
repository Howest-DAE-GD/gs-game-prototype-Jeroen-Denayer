#pragma once
#include "Ball.h"
#include "GameData.h"

//Forward declarations

class BallManager final
{
public:
	BallManager(const Rectf& viewport, float ballSize, float deadLineHeight);
	~BallManager();

	void Start();
	void Draw() const;
	void Update(float dt, GameData::Input& input, GameData::Feedback& feedback);
	void IncreaseDifficulty();
private:
	//Functions
	void CreateNewBall();
	void DrawDeadline() const;
	std::vector<MiniGame*>* GetMiniGamesVector(int numMiniGames);
	float CalculateMiniGamesTime(const std::vector<MiniGame*>& miniGames);

	//Members
	Rectf m_Viewport;
	Point2f m_Pos;
	bool m_Active;
	int m_Difficulty;
	int m_MaxNumBallsOnScreen;
	std::vector<Ball*> m_pBalls;
	int m_FirstBallIdx;
	int m_LastBallIdx; 
	float m_BallSize;
	float m_DeadlineHeight;
	int m_MinNumMiniGamesPerBall;
	int m_MaxNumMiniGamesPerBall;
};

