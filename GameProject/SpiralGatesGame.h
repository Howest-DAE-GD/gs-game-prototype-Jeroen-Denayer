#pragma once
#include "MiniGame.h"
#include "Spiral.h"

class SpiralGatesGame final
	:public MiniGame
{
public:
	SpiralGatesGame(int difficulty);

	virtual void Draw(Point2f pos, float innerRad, float outerRad, float centerRadius) const override;
	virtual void Update(float dt, const GameData::Input& input, GameData::Feedback& feedback) override;
	virtual void Init(int difficulty, bool activate = false) override;
private:
	struct Gate
	{
		float startAngle;
		float endAngle;
		bool isOpen;
		bool bounceBackWhenClosed;
	};

	struct Config
	{
		int minNumGates;
		float selectorRotSpeed;
		float startEmptyArcLength;
		float finishArcLength;
		float gateArcLength;
		float minAngleBetweenGates;
		float maxAngleBetweenGates;
	};

	//Functions
	virtual void Click(GameData::Feedback& feedback) override;
	virtual void ConfigureDifficulty(int difficulty) override;
	virtual void CalculateTimeToComplete() override;
	bool IsSelectorPastAngle(float angle) const;
	bool SelectorOverlapsGate(const Gate& gate) const;

	//Members
	int m_MaxNumGates;
	int m_NumUsedGates;
	std::vector<Gate> m_Gates;
	float m_StartAngle;
	float m_EndAngle;
	float m_Loops;
	int m_SpiralDir;

	float m_SelectorAngle;
	int m_SelectorNextGateIdx;

	Config m_Config;
};

