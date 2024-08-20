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
		SetRotation, SelectColor
	};

	virtual void Draw(Point2f pos, float innerRad, float outerRad, float centerRadius) const = 0;
	virtual void Update(float dt, bool pressedLeft, bool pressedRight) = 0;
	virtual void Click() = 0;
	virtual void Init(bool activate = false) = 0;

	void Activate();
	State GetState() const;
	int GetPoints() const;
	//int GetNumMissClicks() const;
protected:
	MiniGame(Type type, int numPlaythroughsToComplete = 1);

	Type m_Type;
	State m_State;
	int m_Difficulty;
	int m_Points;
	int m_NumPlaythroughs;
	int m_NumPlaythroughsToComplete;
	float m_MaxTimeToComplete;

	//int m_NumMissClicks;
};

