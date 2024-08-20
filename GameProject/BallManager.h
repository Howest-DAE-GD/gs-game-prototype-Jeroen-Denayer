#pragma once
#include "Ball.h"

//Forward declarations

class BallManager final
{
public:
	struct HitData
	{
		int totalBallScore;
		int numMissClicks;
	};

	BallManager(Point2f startPos, float ballSize);
	~BallManager();

	void Start();
	void Draw() const;
	void Update(float dt, float deadlineHeight, bool pressedLeft, bool pressedRight);
	void Click();
	const std::vector<float>& GetBallSizes();
	const HitData& GetHitData() const;
private:
	//Functions
	void CreateNewBall(bool activate = false);
	void SetNextBallActive();
	void SetHitData();

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

	HitData m_HitData;
};

