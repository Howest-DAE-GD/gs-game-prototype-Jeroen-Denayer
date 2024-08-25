#pragma once
#include "MiniGame.h"

class SetRotationGame final
	:public MiniGame
{
public:
	SetRotationGame(int difficulty, const DrawData& drawData);

	virtual void Draw(Point2f pos) const override;
	virtual void Update(float dt, const GameData::Input& input, GameData::Feedback& feedback) override;
	virtual void Init(int difficulty, bool activate = false) override;
private:
	struct Config
	{
		float validAngleDeviation;
	};

	virtual void Click(GameData::Feedback& feedback) override;
	virtual void ConfigureDifficulty(int difficulty) override;
	virtual void CalculateTimeToComplete() override;

	float m_ValidAngle;
	float m_SelectorAngle;
	float m_SelectorRotSpeed;

	Config m_Config;
};

