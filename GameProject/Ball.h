#pragma once

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

	Ball(float size, const Point2f&, float speed, MiniGame* pMiniGame);
	~Ball();

	void Draw() const;
	void Update(float dt, float deadlineHeight, bool pressedLeft, bool pressedRight);
	void Click();

	int GetPoints() const;
	float GetTimeToSolve() const;
private:
	//Functions
	void SetState(State newState);

	//Members
	State m_State;
	int m_Points;
	float m_Rad;
	Point2f m_Pos;
	float m_Speed;
	float m_TimeToSolve;
	float m_TimeSinceCompletion;
	Color4f m_Color;

	MiniGame* m_pMiniGame;

	static float s_FadeTime;

	friend class BallManager;
};

