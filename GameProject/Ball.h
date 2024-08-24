#pragma once
#include "GameData.h"

//Forward declarations
class BallManager;
class MiniGame;

class Ball
{
public:
	enum class State
	{
		Idle, Active, Completed, Missed
	};



	Ball(const Point2f& pos, float size, float idleSpeed, float activeSpeed, MiniGame* pMiniGame);
	~Ball();

	void Draw() const;
	void Update(float dt, const GameData::Input& input, GameData::Feedback& feedback);

	float GetTimeToComplete() const;
private:
	//Functions
	void SetState(State newState);
	void UpdateSpeed(float dt);

	//Members
	State m_State;
	int m_Points;
	float m_Rad;
	Point2f m_Pos;
	float m_Acc;
	float m_Speed;
	float m_TargetSpeed;
	float m_ActiveSpeed;
	float m_TimeToComplete;
	float m_TimeSinceCompletion;
	Color4f m_Color;

	MiniGame* m_pMiniGame;

	static float s_FadeTime;

	friend class BallManager;
};

