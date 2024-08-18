#include "pch.h"
#include "Rotation.h"

Rotation::Rotation()
	:MiniGame(MiniGame::Type::Rotation)
	, m_ValidAngle{ float(rand() % 360) }
	, m_ValidAngleDeviation{ 25.f }
	, m_Angle{ float(rand() % 360) }
	, m_RotSpeed{ 180.f }
{
}

void Rotation::Draw(float yPos, float rad) const
{
	//Draw 2 lines to indicate the valid angle region
	float fromAngle{ m_ValidAngle + m_ValidAngleDeviation };
	float tillAngle{ float(m_ValidAngle + 2 * M_PI) - m_ValidAngleDeviation };
	utils::DrawArc(Point2f{ 0.f, yPos }, rad, rad, fromAngle, tillAngle);
	float linePercOfRad{ 0.5f };
	float innerRad{ rad * (1.f - linePercOfRad) };
	Point2f p0{ std::cosf(utils::Radians(fromAngle)) * innerRad	, yPos + std::sinf(utils::Radians(fromAngle)) * innerRad };
	Point2f p1{ std::cosf(utils::Radians(fromAngle)) * rad		, yPos + std::sinf(utils::Radians(fromAngle)) * rad };
	Point2f p2{ std::cosf(utils::Radians(tillAngle)) * innerRad	, yPos + std::sinf(utils::Radians(tillAngle)) * innerRad };
	Point2f p3{ std::cosf(utils::Radians(tillAngle)) * rad		, yPos + std::sinf(utils::Radians(tillAngle)) * rad };
	utils::DrawLine(p0, p1, 2.f);
	utils::DrawLine(p2, p3, 2.f);

	//Draw the player controlled line
	Point2f p4{ std::cosf(utils::Radians(m_Angle)) * rad, yPos + std::sinf(utils::Radians(m_Angle)) * rad };
	utils::DrawLine(Point2f{ 0.f, yPos }, p4);
}

void Rotation::Update(float dt, bool pressedLeft, bool pressedRight)
{
	if (m_State == State::Idle)
		return;

	if (pressedLeft)
	{
		m_Angle += m_RotSpeed * dt;
		if (m_Angle > 360)
			m_Angle = 0.f;
	}
	if (pressedRight)
	{
		m_Angle -= m_RotSpeed * dt;
		if (m_Angle < 0.f)
			m_Angle = 360.f;
	}
}

void Rotation::Click()
{
	if (utils::SmallestAngleBetween2Angles(m_Angle, m_ValidAngle) < m_ValidAngleDeviation)
		m_State = State::Completed;
	else
		m_State = State::Failed;
}
