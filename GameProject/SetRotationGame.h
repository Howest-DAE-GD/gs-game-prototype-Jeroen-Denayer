#pragma once
#include "MiniGame.h"

class SetRotationGame final
	:public MiniGame
{
public:
	SetRotationGame(int difficulty);

	virtual void Draw(Point2f pos, float innerRad, float outerRad, float centerRadius) const override;
	virtual void Update(float dt, const GameData::Input& input, GameData::Feedback& feedback) override;
	virtual void Click(GameData::Feedback& feedback) override;
	virtual void Init(bool activate = false) override;
private:
	virtual void ConfigureDifficulty(int difficulty) override;
	virtual void CalculateTimeToComplete() override;

	float m_ValidAngle;
	float m_ValidAngleDeviation;
	float m_SelectorAngle;
	float m_SelectorRotSpeed;

	static const std::vector<float> s_AngleDeviationPerDifficulty;
};

