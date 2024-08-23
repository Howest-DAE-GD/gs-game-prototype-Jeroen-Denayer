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
	virtual void Click(GameData::Feedback& feedback) override;
	virtual void Init(bool activate = false) override;
private:
	struct ColorRegion
	{
		bool activated;
		int colorIdx;
		float angle;
		float angleDeviation;
	};

	virtual void ConfigureDifficulty(int difficulty) override;
	virtual void CalculateTimeToComplete() override;

	//Members
	int m_ValidColorIdx;
	int m_MaxNumColorRegions;
	int m_MinNumColorRegions;
	int m_NumUsedColorRegions;
	std::vector<ColorRegion> m_ColorRegions;

	float m_SelectorAngle;
	float m_SelectorRotSpeed;
	int m_SelectorRotDir;

	static const std::vector<Color3f> s_Colors;
	static const std::vector<float> s_AngleDeviationPerDifficulty;
	static const std::vector<float> s_SelectorRotSpeedPerDifficulty;
};

