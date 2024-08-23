#pragma once
#include "Ball.h"
#include "GameData.h"

//Forward declarations

class BallManager final
{
public:
	BallManager(Point2f startPos, float ballSize, float deadLineHeight);
	~BallManager();

	void Start();
	void Draw() const;
	void Update(float dt, const GameData::Input& input, GameData::Feedback& feedback);
	void IncreaseDifficulty();
	const std::vector<float>& GetBallSizes();
private:
	//Functions
	void CreateNewBall(bool activate = false);
	void SetNextBallActive();

	//Members
	Point2f m_Pos;
	bool m_Active;
	int m_Difficulty;
	float m_TimeToNextballSpawn;
	int m_MaxNumBallsOnScreen;
	std::vector<Ball*> m_pBalls;
	int m_FirstBallIdx;
	int m_LastBallIdx; 
	float m_DeadLineHeight;

	std::vector<float> m_BallSizes;
};

