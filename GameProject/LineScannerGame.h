#pragma once
#include "MiniGame.h"

class LineScannerGame
	:public MiniGame
{
public:
	LineScannerGame(int difficulty, const DrawData& drawData);

	virtual void Draw(const Point2f& pos) const override;
	virtual void Update(float dt, const GameData::Input& input, GameData::Feedback& feedback) override;
	virtual void Init(int difficulty, bool activate = false) override;
private:
	struct Config
	{
		float selectorSpeed;
	};

	virtual void Click(GameData::Feedback& feedback) override;
	virtual void ConfigureDifficulty(int difficulty) override;
	virtual void CalculateTimeToComplete() override;

	//Members
	int m_MinNumLines;
	int m_MaxNumLines;
	std::vector<float> m_Lines;

	int m_SelectorDir;
	float m_MaxSelectorPos;
	float m_SelectorPos;

	Config m_Config;
};

