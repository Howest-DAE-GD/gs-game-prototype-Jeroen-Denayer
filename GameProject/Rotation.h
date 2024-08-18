#pragma once
#include "MiniGame.h"

class Rotation final
	:public MiniGame
{
public:
	Rotation();

	virtual void Draw(float yPos, float rad) const override;
	virtual void Update(float dt, bool pressedLeft, bool pressedRight) override;
	virtual void Click() override;
private:
	float m_ValidAngle;
	float m_ValidAngleDeviation;
	float m_Angle;
	float m_RotSpeed;
};

