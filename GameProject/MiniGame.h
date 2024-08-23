#pragma once
#include "GameData.h"

//Forward declarations

class MiniGame
{
public:
	enum class State
	{
		Idle, Active, Completed, Failed
	};

	enum class Type
	{
		SetRotation, SelectColor, SpiralGates
	};

	struct Feedback
	{
		bool loseLife;
	};

	virtual void Draw(Point2f pos, float innerRad, float outerRad, float centerRadius) const = 0;
	virtual void Update(float dt, const GameData::Input& input, GameData::Feedback& feedback) = 0;
	virtual void Click(GameData::Feedback& feedback) = 0;
	virtual void Init(bool activate = false) = 0;

	void Activate();
	State GetState() const;
protected:
	MiniGame(Type type, int difficulty, int maxDifficulty, int numPlaythroughsToComplete = 1);

	Type m_Type;
	State m_State;
	int m_MaxDifficulty;
	int m_Difficulty;
	int m_Points;
	int m_NumPlaythroughs;
	int m_NumPlaythroughsToComplete;
	float m_MaxTimeToComplete;
};

