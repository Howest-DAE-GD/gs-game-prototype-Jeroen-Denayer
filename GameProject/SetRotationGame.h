#pragma once
#include "MiniGame.h"

class SetRotationGame final
	:public MiniGame
{
public:
	SetRotationGame();

	virtual void Draw(Point2f pos, float innerRad, float outerRad, float centerRadius) const override;
	virtual void Update(float dt, bool pressedLeft, bool pressedRight) override;
	virtual void Click() override;
	virtual void Init(bool activate = false) override;
private:
	float m_ValidAngle;
	float m_ValidAngleDeviation;
	float m_SelectorAngle;
	float m_SelectorRotSpeed;
};

