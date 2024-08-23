#pragma once
#include "MiniGame.h"
#include "Spiral.h"

class SpiralGatesGame
	:public MiniGame
{
public:
	SpiralGatesGame(int difficulty);

	virtual void Draw(Point2f pos, float innerRad, float outerRad, float centerRadius) const override;
	virtual void Update(float dt, const GameData::Input& input, GameData::Feedback& feedback) override;
	virtual void Click(GameData::Feedback& feedback) override;
	virtual void Init(bool activate = false) override;
private:
	struct Gate
	{
		float startAngle;
		float endAngle;
		bool isOpen;
		bool closeAfterOpen;
		bool bounceBackWhenClosed;
	};

	//Members
	int m_MaxNumGates;
	int m_MinNumGates;
	int m_NumUsedGates;
	std::vector<Gate> m_Gates;
	float m_StartAngle;
	float m_EndAngle;
	float m_Loops;
	int m_SpiralDir;

	float m_SelectorAngle;
	float m_SelectorRotSpeed;
};

