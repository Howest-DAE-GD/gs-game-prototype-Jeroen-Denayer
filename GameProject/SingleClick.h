#pragma once
#include "CatchMechanism.h"

class SingleClick final
	: public CatchMechanism
{
public:
	SingleClick(Ball* pBall, bool hasRotation);

	virtual void Draw() const override;
	virtual void Update(float dt, const Lighter::Data& lighterData) override;
	virtual void ReceiveInput(const Lighter::Data& lighterData) override;
private:
	bool m_HasRotation;
	float m_Angle;
	float m_AngleDeviation;
};

