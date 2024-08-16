#pragma once
#include "Lighter.h"

//Forward declarations
class BallManager;

class Ball
{
public:
	Ball(float size, float yPos, float speed);
	void Draw() const;
	void Update(float dt, const Lighter::Data& lighterData);
	float GetTimeToSolve() const;
private:
	//Members
	bool m_Active;
	float m_Rad;
	float m_YPos;
	float m_Speed;
	float m_TimeToSolve;

	friend class BallManager;
};

