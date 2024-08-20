#pragma once
#include "MiniGame.h"

class SelectColorGame final
	:public MiniGame
{
public:
	SelectColorGame();

	virtual void Draw(Point2f pos, float innerRad, float outerRad, float centerRadius) const override;
	virtual void Update(float dt, bool pressedLeft, bool pressedRight) override;
	virtual void Click() override;
	virtual void Init(bool activate = false) override;
private:
	struct ColorRegion
	{
		bool activated;
		int colorIdx;
		float angle;
		float angleDeviation;
	};

	//Functions

	//Members
	int m_ValidColorIdx;
	int m_MaxNumColorRegions;
	int m_MinNumColorRegions;
	int m_NumUsedColorRegions;
	std::vector<ColorRegion> m_ColorRegions;

	float m_SelectorAngle;
	float m_SelectorRotSpeed;

	static const std::vector<Color3f> s_Colors;
};

