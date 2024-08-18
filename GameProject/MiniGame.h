#pragma once
class MiniGame
{
public:
	enum class State
	{
		Idle, Active, Completed, Failed
	};

	enum class Type
	{
		Rotation
	};

	virtual void Draw(float yPos, float rad) const = 0;
	virtual void Update(float dt, bool pressedLeft, bool pressedRight) = 0;
	virtual void Click() = 0;
	void Activate();
	State GetState() const;
protected:
	MiniGame(Type type);

	Type m_Type;
	State m_State;
	int m_Points;
	float m_MaxTimeToComplete;
};

