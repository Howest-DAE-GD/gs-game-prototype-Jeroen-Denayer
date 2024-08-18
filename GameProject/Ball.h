#pragma once
#include "Lighter.h"
#include "CatchMechanism.h"

//Forward declarations
class BallManager;

class Ball
{
public:
	enum class State
	{
		Idle, Active, Caught, Missed, Completed
	};

	Ball(float size, float yPos, float speed, int points, CatchMechanism::Type catchMechanismType);
	~Ball();

	void Draw() const;
	void Update(float dt, const Lighter::Data& lighterData);
	void ReceiveInput(const Lighter::Data& lighterData);

	float GetTimeToSolve() const;
	float GetYPos() const;
	float GetRadius() const;
private:
	//Functions
	void SetState(State newState);

	//Members
	State m_State;
	int m_Points;
	float m_Rad;
	float m_YPos;
	float m_Speed;
	float m_TimeToSolve;
	float m_TimeSinceCompletion;
	Color4f m_Color;

	CatchMechanism* m_pCatchMechanism;

	static float s_FadeTime;

	friend class BallManager;
};

