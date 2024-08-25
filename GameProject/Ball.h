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
		Idle, Active, Finished
	};

	Ball(const Point2f& pos, float size, float idleSpeed, float activeSpeed, std::vector<MiniGame*>* pMiniGames);
	~Ball();

	void Draw() const;
	void Update(float dt, const GameData::Input& input, GameData::Feedback& feedback);
private:
	//Functions
	void DrawMiniGameIndicators(float innerRad) const;
	void SetState(State newState);
	void UpdateSpeed(float dt);
	void ActivateNextMiniGame();
	bool CompletedAllMiniGames();

	//Members
	State m_State;
	int m_Points;
	float m_Rad;
	Point2f m_Pos;
	float m_Acc;
	float m_Speed;
	float m_TargetSpeed;
	float m_ActiveSpeed;
	float m_DeadlineHeight;
	float m_TimeToComplete;
	float m_TimeSinceCompletion;
	Color4f m_Color;

	int m_ActiveMiniGameIdx;
	std::vector<MiniGame*>* m_pMiniGames;

	static float s_FadeTime;

	friend class BallManager;
};

