#include "pch.h"
#include "Lighter.h"

Lighter::Lighter(Point2f startPos, const std::vector<float>& sizes)
	: m_Pos{ startPos }
	, m_LighterSizes{ sizes }
	, m_HitRadiusPerctageOfSize{ 0.2f }
	, m_CurSizeIdx{ int(m_LighterSizes.size() / 2.f) }
	, m_Angle{ 90.f * float(M_PI) / 180.f }
	, m_RotSpeed{ 180.f * float(M_PI) / 180.f }
{
}

void Lighter::Draw() const
{
	utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
	Data lighterData{ GetData() };
	utils::DrawEllipse(lighterData.pos, lighterData.innerRadius, lighterData.innerRadius);
	utils::DrawEllipse(lighterData.pos, lighterData.outerRadius, lighterData.outerRadius);

	float endPointRad{ lighterData.outerRadius * 1.1f };
	Point2f endPoint{ lighterData.pos.x + std::cosf(m_Angle) * endPointRad, lighterData.pos.y + std::sinf(m_Angle) * endPointRad };
	utils::DrawLine(lighterData.pos, endPoint);
}

void Lighter::IncreaseSize()
{
	if (m_CurSizeIdx > 0)
		--m_CurSizeIdx;
}

void Lighter::DecreaseSize()
{
	if (m_CurSizeIdx < m_LighterSizes.size() - 1)
		++m_CurSizeIdx;
}

void Lighter::IncreaseAngle(float dt)
{
	m_Angle += m_RotSpeed * dt;
	if (m_Angle > 2 * float(M_PI))
		m_Angle = 0.f;
}

void Lighter::DecreaseAngle(float dt)
{
	m_Angle -= m_RotSpeed * dt;
	if (m_Angle < 0.f)
		m_Angle = 2 * float(M_PI);
}

Lighter::Data Lighter::GetData() const
{
	float size{ m_LighterSizes[m_CurSizeIdx] };
	float radius{ size / 2.f };
	float innerRadius{ radius - size * m_HitRadiusPerctageOfSize };
	float outerRadius{ radius + size * m_HitRadiusPerctageOfSize };
	return Data{ m_Pos, size, innerRadius, outerRadius, m_Angle };
}
