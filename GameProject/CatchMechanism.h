#pragma once
#include "Lighter.h"

//Forward declarations
class Ball;

class CatchMechanism
{
public:
	enum class State
	{
		Active, Caught, Missed
	};

	enum class Type
	{
		Base, SingleClick
	};
protected:
	CatchMechanism(Type type, Ball* pBall);
	virtual ~CatchMechanism();
	virtual void Draw() const = 0;
	virtual void Update(float dt, const Lighter::Data& lighterData) = 0;
	virtual void ReceiveInput(const Lighter::Data& lighterData) = 0;

	State GetState() const;

	Type m_Type;
	State m_State;
	Ball* m_pBall;

	friend class Ball;
};

