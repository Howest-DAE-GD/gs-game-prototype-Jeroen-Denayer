#pragma once
#include "MiniGame.h"

//forward declarations

class SelectColorGame final
	:public MiniGame
{
public:
	SelectColorGame(int difficulty);

	virtual void Draw(Point2f pos, float innerRad, float outerRad, float centerRadius) const override;
	virtual void Update(float dt, const GameData::Input& input, GameData::Feedback& feedback) override;
	virtual void Init(int difficulty, bool activate = false) override;
private:
	struct ColorRegion
	{
		bool activated;
		int colorIdx;
		float angle;
		float angleDeviation;
	};

	struct Config
	{
		float selectorRotSpeed;
		float angleDeviation;
	};

	virtual void Click(GameData::Feedback& feedback) override;
	virtual void ConfigureDifficulty(int difficulty) override;
	virtual void CalculateTimeToComplete() override;

	//Members
	int m_ValidColorIdx;
	int m_MaxNumColorRegions;
	int m_MinNumColorRegions;
	int m_NumUsedColorRegions;
	std::vector<ColorRegion> m_ColorRegions;

	float m_SelectorAngle;
	int m_SelectorRotDir;

	Config m_Config;

	static const std::vector<Color3f> s_Colors;
};

