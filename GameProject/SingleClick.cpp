#include "pch.h"
#include "SingleClick.h"
#include "Ball.h"

SingleClick::SingleClick(Ball* pBall)
	: CatchMechanism(CatchMechanism::Type::SingleClick, pBall)
	, m_HasRotation{ true }
	, m_Angle{ (rand() % 360) * float(M_PI) / 180.f }
	, m_AngleDeviation{ 25.f * float(M_PI) / 180.f }
{
}

void SingleClick::Draw() const
{
	float yPos{ m_pBall->GetYPos() };
	float rad{ m_pBall->GetRadius() };

	if (m_HasRotation)
	{
		float fromAngle{ m_Angle + m_AngleDeviation };
		float tillAngle{ float(m_Angle + 2 * M_PI) - m_AngleDeviation };
		utils::DrawArc(Point2f{ 0.f, yPos }, rad, rad, fromAngle, tillAngle);
		float linePercOfRad{ 0.1f };
		float innerScale{ rad * (1.f - linePercOfRad) };
		float outerScale{ rad * (1.f + linePercOfRad) };
		Point2f p0{std::cosf(fromAngle) * innerScale, yPos + std::sinf(fromAngle) * innerScale};
		Point2f p1{std::cosf(fromAngle) * outerScale, yPos + std::sinf(fromAngle) * outerScale };
		Point2f p2{std::cosf(tillAngle) * innerScale, yPos + std::sinf(tillAngle) * innerScale};
		Point2f p3{std::cosf(tillAngle) * outerScale, yPos + std::sinf(tillAngle) * outerScale };
		utils::DrawLine(p0, p1);
		utils::DrawLine(p2, p3);
	}
	else
	{
		utils::DrawEllipse(Point2f{ 0.f, yPos }, rad, rad, 2.f);
	}
}

void SingleClick::Update(float dt, const Lighter::Data& lighterData)
{
	float yPos{ m_pBall->GetYPos() };
	float rad{ m_pBall->GetRadius() };
	float distBetweenCenters{  lighterData.pos.y - yPos };
	if (distBetweenCenters > (lighterData.outerRadius - lighterData.innerRadius) * 0.5f)
		m_State = State::Missed;
}

void SingleClick::ReceiveInput(const Lighter::Data& lighterData)
{
	float yPos{ m_pBall->GetYPos() };
	float rad{ m_pBall->GetRadius() };
	float distBetweenCenters{ std::abs(yPos - lighterData.pos.y) };
	bool distOk{ distBetweenCenters < (lighterData.outerRadius - lighterData.innerRadius) * 0.5f };

	bool angleOk{ true };
	if (m_HasRotation)
	{
		angleOk = utils::SmallestAngleBetween2Angles(m_Angle, lighterData.angle) < m_AngleDeviation;
	}

	if (distOk && angleOk)
		m_State = State::Caught;
	else
		m_State = State::Missed;
}

