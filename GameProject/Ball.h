#pragma once
#include "Lighter.h"

//Forward declarations
class BallManager;

class Ball
{
public:
	enum class State
	{
		Idle, Active, Caught, Missed, Completed
	};

	Ball(float size, float yPos, float speed);
	void Draw() const;
	void Update(float dt, const Lighter::Data& lighterData);
	float GetTimeToSolve() const;
private:
	//Functions
	void SetState(State newState);

	//Members
	State m_State;
	float m_TimeSinceCompletion;
	int m_Points;
	float m_Rad;
	float m_YPos;
	float m_Speed;
	float m_TimeToSolve;
	Color4f m_Color;

	static float s_FadeTime;

	friend class BallManager;
};

