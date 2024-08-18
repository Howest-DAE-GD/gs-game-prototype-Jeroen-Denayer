#pragma once

//Forward declarations
class BallManager;
class MiniGame;

class Ball
{
public:
	enum class State
	{
		Idle, Active, Caught, Missed, Completed
	};

	Ball(float size, float yPos, float speed, int points, MiniGame* pMiniGame);
	~Ball();

	void Draw() const;
	void Update(float dt, bool pressedLeft, bool pressedRight);
	void Click();

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

	MiniGame* m_pMiniGame;

	static float s_FadeTime;

	friend class BallManager;
};

