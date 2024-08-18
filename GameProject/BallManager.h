#pragma once
#include <vector>
#include "Lighter.h"
#include "Ball.h"

//Forward declarations

class BallManager final
{
public:
	struct HitData
	{
		bool completed;
		int score;
	};

	BallManager(Point2f startPos, float ballSize);
	~BallManager();

	void Start();
	void Draw() const;
	void Update(float dt, const Lighter::Data& lighterData);
	void ReceiveInput(const Lighter::Data& lighterData);
	const std::vector<float>& GetBallSizes();
	const HitData& GetHitData() const;
	static CatchMechanism* CreateCatchMechanism(CatchMechanism::Type type, Ball* pBall);
private:
	//Functions
	void CreateNewBall();
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

