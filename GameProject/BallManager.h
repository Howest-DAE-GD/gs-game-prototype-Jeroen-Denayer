#pragma once
#include <vector>
#include "Lighter.h"
#include "Ball.h"

//Forward declarations

class BallManager final
{
public:
	BallManager(Point2f startPos, float ballSize);
	~BallManager();

	void Start();
	void Draw() const;
	void Update(float dt, const Lighter::Data& lighterData);
	bool IsBallHit(const Lighter::Data& lighterData);
	const std::vector<float>& GetBallSizes();
private:
	//Functions
	bool IsBallInLighter(Ball* pBall, const Lighter::Data& lighterData);
	bool IsBallPastLighter(Ball* pBall, const Lighter::Data& lighterData);
	void CreateNewBall();
	void SetNextBallActive();

	//Members
	Point2f m_Pos;
	bool m_Active;
	int m_pLevel;
	float m_TimeToNextballSpawn;
	int m_MaxNumBallsOnScreen;
	std::vector<Ball*> m_pBalls;
	int m_FirstBallIdx;
	int m_LastBallIdx;

	std::vector<float> m_BallSizes;
	float m_DistBetweenBalls;
};

